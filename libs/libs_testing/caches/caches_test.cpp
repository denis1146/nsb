#include <gtest/gtest.h>
#include <charconv>
#include <string>
#include <array>

#include "caches.h"

namespace {

constexpr size_t MAX_SIZE = 3;
constexpr auto ERR_NO = std::errc{};

auto toString= [] (size_t value, std::errc& ec) -> std::string {
  constexpr size_t sz { 15 };
  std::array<char, sz> buf;
  const auto [ptr, err] = std::to_chars(buf.data(), buf.data() + buf.size(), value);
  ec = err;
  return err == std::errc{} ? std::string{ buf.data(), ptr } : "";
};

template<class Cache>
class CacheTest: public testing::Test
{
public:
  CacheTest()
  : m_cache{ MAX_SIZE, [this] (const auto& key) {
    if constexpr (std::is_same_v<typename Cache::value_type, size_t>) {
      m_ec = ERR_NO;
      return key;
    }
    else {
      return toString(key, m_ec);
    }
  }}
  {}

  std::errc m_ec { ERR_NO };
  Cache m_cache;
};

using CacheTypes = ::testing::Types<
  nsb::caches::Lru<size_t, std::string>
, nsb::caches::Lru<size_t, size_t>
, nsb::caches::Fifo<size_t, std::string>
, nsb::caches::Fifo<size_t, size_t>
>;
TYPED_TEST_SUITE(CacheTest, CacheTypes);


template<class Cache>
class CopyMoveOpCacheTest: public testing::Test
{
public:
  using CacheType = Cache;
  using ToStingFunc = std::string(*)(int);
  ToStingFunc toSting { &std::to_string };
};

using CopyMoveOpCacheTypes = ::testing::Types<
  nsb::caches::Lru<int, std::string>
, nsb::caches::Fifo<int, std::string>
>;

TYPED_TEST_SUITE(CopyMoveOpCacheTest, CopyMoveOpCacheTypes);

}

TYPED_TEST(CopyMoveOpCacheTest, copyConstructor)
{
  // Arrange
  using CacheType = typename TestFixture::CacheType;
  CacheType cache{ this->toSting };
  [[maybe_unused]] const auto v = cache.get(0);
  [[maybe_unused]] const auto v1 = cache.get(1);
  [[maybe_unused]] const auto v2 = cache.get(2);
  const auto vals = cache.getAll();
  const auto maxSize =cache.maxSize();

  // Act
  CacheType cache1{ cache };
  const auto vals1 = cache1.getAll();
  const auto maxSize1 =cache1.maxSize();

  // Assert
  ASSERT_EQ(vals1, vals);
  ASSERT_EQ(maxSize1, maxSize);
}

TYPED_TEST(CopyMoveOpCacheTest, moveConstructor)
{
  // Arrange
  using CacheType = typename TestFixture::CacheType;
  CacheType cache{ this->toSting };
  [[maybe_unused]] const auto v = cache.get(0);
  [[maybe_unused]] const auto v1 = cache.get(1);
  [[maybe_unused]] const auto v2 = cache.get(2);
  const auto vals = cache.getAll();
  const auto maxSize =cache.maxSize();

  // Act
  CacheType cache1{ std::move(cache) };
  const auto vals1 = cache1.getAll();
  const auto maxSize1 =cache1.maxSize();
  const auto isEmpty = cache.getAll().empty();
  const auto size = cache.size();

  // Assert
  ASSERT_EQ(vals1, vals);
  ASSERT_EQ(maxSize1, maxSize);
  ASSERT_TRUE(isEmpty);
  ASSERT_EQ(size, 0);
}

TYPED_TEST(CopyMoveOpCacheTest, copyAssignmentOperator)
{
  // Arrange
  using CacheType = typename TestFixture::CacheType;
  CacheType cache{ this->toSting };
  [[maybe_unused]] const auto v = cache.get(0);
  [[maybe_unused]] const auto v1 = cache.get(1);
  [[maybe_unused]] const auto v2 = cache.get(2);
  const auto vals = cache.getAll();
  const auto maxSize =cache.maxSize();
  constexpr size_t defaultMaxSize { 1 };

  // Act
  CacheType cache1{ defaultMaxSize, this->toSting };
  const auto defMaxSize1 =cache1.maxSize();
  cache1 = cache;
  const auto vals1 = cache1.getAll();
  const auto maxSize1 =cache1.maxSize();

  // Assert
  ASSERT_EQ(vals1, vals);
  ASSERT_EQ(maxSize1, maxSize);
  ASSERT_EQ(defMaxSize1, defaultMaxSize);
  ASSERT_NE(defMaxSize1, maxSize);
}

TYPED_TEST(CopyMoveOpCacheTest, moveAssignmentOperator)
{
  // Arrange
  using CacheType = typename TestFixture::CacheType;
  constexpr size_t defaultMaxSize { 1 };
  CacheType cache{ defaultMaxSize, this->toSting };
  [[maybe_unused]] const auto v = cache.get(0);
  [[maybe_unused]] const auto v1 = cache.get(1);
  [[maybe_unused]] const auto v2 = cache.get(2);
  const auto vals = cache.getAll();
  const auto maxSize =cache.maxSize();

  // Act
  CacheType cache1{ defaultMaxSize, this->toSting };
  cache1 = std::move(cache);

  const auto vals1 = cache1.getAll();
  const auto maxSize1 = cache1.maxSize();
  const auto isEmpty = cache.getAll().empty();
  const auto size = cache.size();

  // Assert
  ASSERT_EQ(vals1, vals);
  ASSERT_EQ(maxSize1, maxSize);
  ASSERT_TRUE(isEmpty);
  ASSERT_EQ(size, 0);
}

TYPED_TEST(CopyMoveOpCacheTest, constructor_slowGetValue)
{
  using CacheType = typename TestFixture::CacheType;

  CacheType cache{ this->toSting };

  ASSERT_EQ(cache.size(), 0);
}

TYPED_TEST(CopyMoveOpCacheTest, constructor_maxSize_slowGetValue)
{
  using CacheType = typename TestFixture::CacheType;
  constexpr size_t defaultMaxSize { 50 };

  CacheType cache{ defaultMaxSize, this->toSting };

  ASSERT_EQ(cache.maxSize(), defaultMaxSize);
  ASSERT_EQ(cache.size(), 0);
}

TYPED_TEST(CacheTest, get)
{
  // Arrange
  const auto& cache = this->m_cache;
  const auto& ec = this->m_ec;
  
  // Act
  const auto v1 = cache.get(1);
  const auto ec1 = ec;
  const auto v2 = cache.get(2);
  const auto ec2 = ec;
  const auto v3 = cache.get(3);
  const auto ec3 = ec;

  const auto v4 = cache.get(3);
  const auto ec4 = ec;
  const auto v5 = cache.get(5);
  const auto ec5 = ec;
  const auto v6 = cache.get(6);
  const auto ec6 = ec;

  // Assert
  if constexpr (std::is_same_v<decltype(cache.get(0)), std::string>) {
    ASSERT_STREQ(v1.c_str(), "1") << "++++++ CacheTest.get [value = std::string] +++++++ ";
    ASSERT_EQ(ec1, ERR_NO);
    ASSERT_STREQ(v2.c_str(), "2");
    ASSERT_EQ(ec2, ERR_NO);
    ASSERT_STREQ(v3.c_str(), "3");
    ASSERT_EQ(ec3, ERR_NO);
    ASSERT_STREQ(v4.c_str(), "3");
    ASSERT_EQ(ec4, ERR_NO);
    ASSERT_STREQ(v5.c_str(), "5");
    ASSERT_EQ(ec5, ERR_NO);
    ASSERT_STREQ(v6.c_str(), "6");
    ASSERT_EQ(ec6, ERR_NO);
  }
  else {
    ASSERT_EQ(v1, 1) << "++++++ CacheTest.get [value = size_t] +++++++ ";
    ASSERT_EQ(v2, 2);
    ASSERT_EQ(v3, 3);
    ASSERT_EQ(v4, 3);
    ASSERT_EQ(v5, 5);
    ASSERT_EQ(v6, 6);
  }
}

TYPED_TEST(CacheTest, getFast)
{ 
  // Arrange
  const auto& cache = this->m_cache;
  constexpr size_t key = 1000;

  // Act
  const auto vf0 = cache.getFast(key);
  const auto v1 = cache.get(key);
  const auto vf1 = cache.getFast(key);

  // Assert
  ASSERT_FALSE(vf0);
  ASSERT_TRUE(vf1);
  ASSERT_EQ(vf1, v1);
}

TYPED_TEST(CacheTest, getAll)
{
  // Arrange
  const auto& cache = this->m_cache;
  std::array<size_t, MAX_SIZE> keys{};

  // Act
  const auto ga0 = cache.getAll();
  for (size_t i = 0; i < MAX_SIZE; ++i) {
    [[maybe_unused]] auto _ = cache.get(i);
  }
  const auto ga1 = cache.getAll();
  for (size_t i = 0; i < keys.size() && i < ga1.size(); ++i)
    keys[i] = ga1[i].first;
  std::sort(begin(keys), end(keys));

  // Assert
  ASSERT_EQ(ga0.size(), 0);
  ASSERT_EQ(ga1.size(), MAX_SIZE);
  for (size_t i = 0; i < MAX_SIZE; ++i)
    ASSERT_EQ(keys[i], i);
}

TYPED_TEST(CacheTest, have)
{
  // Arrange
  const auto& cache = this->m_cache;
  std::array<bool, MAX_SIZE> hasValues{};

  // Act
  for (size_t i = 0; i < MAX_SIZE; ++i)
    [[maybe_unused]] auto _ = cache.get(i);

  for (size_t i = 0; i < MAX_SIZE; ++i)
    hasValues[i] = cache.have(i);
  
  // Assert
  for (const auto& has_value : hasValues)
    ASSERT_TRUE(has_value);

  const auto dist = MAX_SIZE + 10;
  for (size_t i = MAX_SIZE; i < dist; ++i)
    ASSERT_FALSE(cache.have(i));
}

TYPED_TEST(CacheTest, clear)
{
  // Arrange
  auto& cache = this->m_cache;

  // Act
  const auto sz0 = cache.size();
  [[maybe_unused]] auto _0 = cache.get(1);
  [[maybe_unused]] auto _1 = cache.get(10);
  [[maybe_unused]] auto _2 = cache.get(100);
  [[maybe_unused]] auto _3 = cache.get(1000);
  const auto sz1 = cache.size();
  cache.clear();
  const auto sz2 = cache.size();
  
  // Assert
  ASSERT_EQ(sz0, 0);
  ASSERT_NE(sz1, 0);
  ASSERT_EQ(sz2, 0);
}

TYPED_TEST(CacheTest, remove)
{
  // Arrange
  auto& cache = this->m_cache;
  constexpr size_t key0 = 1;
  constexpr size_t key1 = 2;

  // Act
  const auto sz0 = cache.size();
  [[maybe_unused]] auto _0 = cache.get(key0);
  [[maybe_unused]] auto _1 = cache.get(key1);
  const auto sz1 = cache.size();
  cache.remove(key0);
  const auto sz2 = cache.size();
  cache.remove(key1);
  const auto sz3 = cache.size();

  // Assert
  ASSERT_EQ(sz0, 0);
  ASSERT_EQ(sz1, 2);
  ASSERT_EQ(sz2, 1);
  ASSERT_EQ(sz3, 0);
}

TYPED_TEST(CacheTest, maxSize)
{
  // Arrange
  const auto& cache = this->m_cache;
  std::array<decltype(cache.maxSize()), MAX_SIZE + 2> maxSizes{};

  // Act
  for (size_t i = 0; i < maxSizes.size(); ++i) {
    [[maybe_unused]] auto _ = cache.get(i);
    maxSizes[i] = cache.maxSize();
  }

  // Assert
  for (size_t i = 0; i < maxSizes.size(); ++i)
    ASSERT_EQ(maxSizes[i], MAX_SIZE);
}

TYPED_TEST(CacheTest, size)
{
  // Arrange
  const auto& cache = this->m_cache;
  constexpr size_t addSize = 4;
  std::array<decltype(cache.size()), MAX_SIZE + addSize> sizes{};

  // Act
  for (size_t i = 0; i < sizes.size(); ++i) {
    sizes[i] = cache.size();      
    [[maybe_unused]] auto _ = cache.get(i);
  }

  // Assert
  ASSERT_EQ(sizes[0], 0);
  for (size_t i = 1; i < MAX_SIZE; ++i) {
    ASSERT_EQ(sizes[i], i);
  }
  for (size_t i = 1 + MAX_SIZE; i < sizes.size(); ++i) {
    ASSERT_EQ(sizes[i], MAX_SIZE);
  }
}
