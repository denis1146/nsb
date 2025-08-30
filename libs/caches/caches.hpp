namespace nsb::caches {

namespace {

constexpr size_t DEF_MAX_SIZE{ 5 };

}

// Lru
template <class Key, class Value>
Lru<Key, Value>::Lru(const Lru& rhs)
: Lru(rhs.m_maxSize, rhs.m_slowGetValue)
{
  m_cache = rhs.m_cache;
  
  auto it = std::begin(m_cache);
  auto itEnd  = std::end(m_cache);
  m_hash.reserve(m_cache.size());
  while (it != itEnd) {
    m_hash.emplace(it->first, it);
    it++;
  }
}

template <class Key, class Value>
Lru<Key, Value>& Lru<Key, Value>::operator=(const Lru<Key, Value>& rhs)
{
  if (this != &rhs) {
    auto tmp = rhs;
    *this = std::move(tmp);
  }
  return *this;
}

template <class Key, class Value>
Lru<Key, Value>::Lru(SlowGetValue slowGetValue)
: Lru(DEF_MAX_SIZE, slowGetValue)
{
}

template <class Key, class Value>
Lru<Key, Value>::Lru(size_t maxSize, SlowGetValue slowGetValue)
: m_maxSize{ maxSize }
, m_slowGetValue{ slowGetValue }
{
  assert(m_slowGetValue);
}

template <class Key, class Value>
Value Lru<Key, Value>::get(const Key& key) const
{
  auto hit = m_hash.find(key);
  if (hit == m_hash.end()) {
    if (size() != maxSize()) {
      m_cache.emplace_front(key, m_slowGetValue(key));
      m_hash.emplace(key, m_cache.begin());
    }
    else {
      auto node = m_hash.extract(m_cache.back().first);
      m_cache.splice(m_cache.begin(), m_cache, --m_cache.end());
      m_cache.front() = {key, m_slowGetValue(key)};
      node.key() = key;
      node.mapped() = m_cache.begin();
      m_hash.insert(std::move(node));
    }
  }
  else {
    auto eltit = hit->second;
    m_cache.splice(m_cache.begin(), m_cache, eltit);
  }
  return m_cache.front().second;
}

template <class Key, class Value>
std::optional<Value> Lru<Key, Value>::getFast(const Key& key) const
{
  auto it = m_hash.find(key);
  if (it != m_hash.end()) {
    const ListIt& lstIt = it->second;
    return { lstIt->second };
  }
  return std::nullopt;
}

template <class Key, class Value>
bool Lru<Key, Value>::have(Key key) const
{
  return m_hash.contains(key);
}

template <class Key, class Value>
typename Lru<Key, Value>::AllCache Lru<Key, Value>::getAll() const
{
  Lru<Key, Value>::AllCache ret;
  ret.reserve(size());
  std::copy(m_cache.cbegin(), m_cache.cend(), std::back_inserter(ret));
  return ret;
}

template <class Key, class Value>
void Lru<Key, Value>::clear()
{
  m_cache.clear();
  m_hash.clear();
}

template <class Key, class Value>
void Lru<Key, Value>::remove(const Key& key)
{
  auto it = m_hash.find(key);
  if (it != m_hash.end()) {
    m_cache.erase(it->second);
    m_hash.erase(it);
  }
}

template <class Key, class Value>
size_t Lru<Key, Value>::maxSize() const
{
  return m_maxSize;
}

template <class Key, class Value>
size_t Lru<Key, Value>::size() const
{
  return m_cache.size();
}

// ~Lru

// Fifo
template <class Key, class Value>
Fifo<Key, Value>::Fifo(const Fifo& rhs)
: Fifo(rhs.m_maxSize, rhs.m_slowGetValue)
{
  m_cache = rhs.m_cache;
  
  auto it = std::begin(m_cache);
  auto itEnd  = std::end(m_cache);
  m_hash.reserve(m_cache.size());
  while (it != itEnd) {
    m_hash.emplace(it->first, it);
    it++;
  }
}

template <class Key, class Value>
Fifo<Key, Value>& Fifo<Key, Value>::operator=(const Fifo<Key, Value>& rhs)
{
  if (this != &rhs) {
    auto tmp = rhs;
    *this = std::move(tmp);
  }
  return *this;
}

template <class Key, class Value>
Fifo<Key, Value>::Fifo(SlowGetValue slowGetValue)
: Fifo(DEF_MAX_SIZE, slowGetValue)
{
}

template <class Key, class Value>
Fifo<Key, Value>::Fifo(size_t maxSize, SlowGetValue slowGetValue)
: m_maxSize{ maxSize }
, m_slowGetValue{ slowGetValue }
{
  assert(m_slowGetValue);
}

template <class Key, class Value>
Value Fifo<Key, Value>::get(const Key& key) const
{
  if (auto val = getFast(key))
    return *val;

  if (size() != maxSize()) {
    m_cache.emplace_back(key, m_slowGetValue(key));
    m_hash.emplace(key, --m_cache.end());
  }
  else {
    auto node = m_hash.extract(m_cache.front().first);
    m_cache.pop_front();
    m_cache.emplace_back(key, m_slowGetValue(key));
    node.key() = key;
    node.mapped() = --m_cache.end();
    m_hash.insert(std::move(node));
  }
  return m_cache.back().second;
}

template <class Key, class Value>
std::optional<Value> Fifo<Key, Value>::getFast(const Key& key) const
{
  auto it = m_hash.find(key);
  if (it != m_hash.end()) {
    const ListIt& lstIt = it->second;
    return { lstIt->second };
  }
  return std::nullopt;
}

template <class Key, class Value>
typename Fifo<Key, Value>::AllCache Fifo<Key, Value>::getAll() const
{
  Fifo<Key, Value>::AllCache ret;
  ret.reserve(size());
  std::copy(m_cache.cbegin(), m_cache.cend(), std::back_inserter(ret));
  return ret;
}

template <class Key, class Value>
bool Fifo<Key, Value>::have(Key key) const
{
  return m_hash.contains(key);
}

template <class Key, class Value>
void Fifo<Key, Value>::clear()
{
  m_cache.clear();
  m_hash.clear();
}

template <class Key, class Value>
void Fifo<Key, Value>::remove(const Key& key)
{
  auto it = m_hash.find(key);
  if (it != m_hash.end()) {
    m_cache.erase(it->second);
    m_hash.erase(it);
  }
}

template <class Key, class Value>
size_t Fifo<Key, Value>::maxSize() const
{
  return m_maxSize;
}

template <class Key, class Value>
size_t Fifo<Key, Value>::size() const
{
  return m_cache.size();
}

// ~Fifo

} // ~nsb::caches
