#pragma once

#include <list>
#include <unordered_map>
#include <optional>
#include <cstddef>
#include <functional>
#include <vector>
#include <deque>

namespace nsb::caches {

// LRU Cache (Least Frequently Used) 
template <class Key, class Value>
class Lru
{
public:
  using key_type = Key;
  using value_type = Value;
  using AllCache = std::vector<std::pair<Key, Value>>;
  using SlowGetValue = std::function<Value(const Key&)>;

  Lru(size_t maxSize, SlowGetValue slowGetValue);
  [[nodiscard]] Value get(const Key& key) const;
  [[nodiscard]] std::optional<Value> getFast(const Key& key) const;
  [[nodiscard]] AllCache getAll() const;
  [[nodiscard]] bool have(Key key) const;

  void clear();
  void remove(const Key& key);

  [[nodiscard]] size_t maxSize() const;
  [[nodiscard]] size_t size() const;

private:
  using ListIt = typename std::list<std::pair<Key, Value>>::iterator;

  const size_t m_maxSize;
  SlowGetValue m_slowGetValue;
  mutable std::list<std::pair<Key, Value>> m_cache;
  mutable std::unordered_map<Key, ListIt> m_hash;
};


// FIFO Cache (First In, First Out)
template <class Key, class Value>
class Fifo
{
public:
  using key_type = Key;
  using value_type = Value;
  using AllCache = std::vector<std::pair<Key, Value>>;
  using SlowGetValue = std::function<Value(const Key&)>;

  Fifo(size_t maxSize, SlowGetValue slowGetValue);
  [[nodiscard]] Value get(const Key& key) const;
  [[nodiscard]] std::optional<Value> getFast(const Key& key) const;
  [[nodiscard]] AllCache getAll() const;
  [[nodiscard]] bool have(Key key) const;

  void clear();
  void remove(const Key& key);

  [[nodiscard]] size_t maxSize() const;
  [[nodiscard]] size_t size() const;

private:
  using ListIt = typename std::deque<std::pair<Key, Value>>::iterator;

  const size_t m_maxSize;
  SlowGetValue m_slowGetValue;
  mutable std::deque<std::pair<Key, Value>> m_cache;
  mutable std::unordered_map<Key, ListIt> m_hash;
};

} // ~nsb::caches

#include "caches.hpp"
