namespace nsb::caches {

// Lru
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
    if (size() == maxSize()) {
      m_hash.erase(m_cache.back().first);
      m_cache.pop_back();
    }
    m_cache.emplace_front(key, m_slowGetValue(key));
    m_hash.emplace(key, m_cache.begin());
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

} // ~nsb::caches
