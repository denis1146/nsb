#include <gtest/gtest.h>
#include <type_traits>
#include <array>

#include "utility.h"
#include "containers/iterator.h"

TEST(utility, toUType)
{
  // Arrange
  constexpr auto c1 = 1;
  constexpr auto c2 = 2;
  constexpr auto c3 = 3;
  constexpr auto c4 = 4;
  constexpr auto c5 = 5;
  enum       E1 : uint8_t  { E1 = c1, };
  enum class E2            { E2 = c2, };
  enum class E3 : unsigned { E3 = c3, };
  enum class E4 : int      { E4 = c4, };
  enum class E5 : uint8_t  { E5 = c5, };

  // Act
  constexpr auto e1 = nsb::toUType(E1::E1);
  constexpr auto e2 = nsb::toUType(E2::E2);
  constexpr auto e3 = nsb::toUType(E3::E3);
  constexpr auto e4 = nsb::toUType(E4::E4);
  constexpr auto e5 = nsb::toUType(E5::E5);

  constexpr auto b1 = std::is_same_v<std::decay_t<decltype(e1)>, uint8_t>;
  constexpr auto b2 = std::is_same_v<std::decay_t<decltype(e2)>, int>;
  constexpr auto b3 = std::is_same_v<std::decay_t<decltype(e3)>, unsigned>;
  constexpr auto b4 = std::is_same_v<std::decay_t<decltype(e4)>, int>;
  constexpr auto b5 = std::is_same_v<std::decay_t<decltype(e5)>, uint8_t>;

  // Assert
  ASSERT_TRUE(b1);
  ASSERT_EQ(e1, c1);
  
  ASSERT_TRUE(b2);
  ASSERT_EQ(e2, c2);
  
  ASSERT_TRUE(b3);
  ASSERT_EQ(e3, c3);
  
  ASSERT_TRUE(b4);
  ASSERT_EQ(e4, c4);
  
  ASSERT_TRUE(b5);
  ASSERT_EQ(e5, c5);
}

TEST(utility, iterator_traits_t)
{
  using int_t = nsb::iterator_traits_t<decltype(std::declval<std::array<int, 0>>().begin())>;
  using double_t = nsb::iterator_traits_t<decltype(std::declval<std::vector<double>>().end())>;
  using long_long_t = nsb::iterator_traits_t<decltype(std::declval<std::vector<long long>>().cend())>;

  testing::StaticAssertTypeEq<int_t, int>();
  testing::StaticAssertTypeEq<double_t, double>();
  testing::StaticAssertTypeEq<long_long_t, long long>();
}