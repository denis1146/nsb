#include <gtest/gtest.h>

#include "type_traits.h"

TEST(TypeTraitsTest, IntegralConstant)
{
  constexpr int v = 42;
  using Int42 = nsb::integral_constant<int, v>;

  testing::StaticAssertTypeEq<Int42::value_type, int>();
  testing::StaticAssertTypeEq<Int42::type, Int42>();

  ASSERT_EQ(Int42::value, v);
  ASSERT_EQ(Int42{}(), v);
  ASSERT_EQ(int(Int42{}), v);
}

TEST(TypeTraitsTest, TrueFalseTypes)
{
  testing::StaticAssertTypeEq<nsb::true_type::value_type, bool>();
  testing::StaticAssertTypeEq<nsb::false_type::value_type, bool>();

  ASSERT_EQ(nsb::true_type::value, true);
  ASSERT_EQ(nsb::false_type::value, false);

  ASSERT_EQ(nsb::true_type{}(), true);
  ASSERT_EQ(nsb::false_type{}(), false);
}

TEST(TypeTraitsTest, IsSame)
{
  auto b1 = nsb::is_same<int, int>::value;
  auto b2 = nsb::is_same<int, bool>::value;

  auto b3 = nsb::is_same_v<int, int>;
  auto b4 = nsb::is_same_v<int, const int>;

  ASSERT_TRUE(b1);
  ASSERT_FALSE(b2);

  ASSERT_TRUE(b3);
  ASSERT_FALSE(b4);
}
