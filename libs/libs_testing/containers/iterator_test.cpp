#include <gtest/gtest.h>

#include "containers/iterator.h"
#include "containers/Array.h"

TEST(IteratorTest, RangeAccessTypes)
{
  nsb::containers::Array<int, 1> a{10};
  std::vector v{0};

  testing::StaticAssertTypeEq<decltype(a)::iterator, decltype(nsb::containers::begin(a))>();
  testing::StaticAssertTypeEq<decltype(a)::const_iterator, decltype(nsb::containers::begin(std::as_const(a)))>();
  testing::StaticAssertTypeEq<decltype(a)::const_iterator, decltype(nsb::containers::cbegin(a))>();

  testing::StaticAssertTypeEq<decltype(v)::iterator, decltype(nsb::containers::begin(v))>();
  testing::StaticAssertTypeEq<decltype(v)::const_iterator, decltype(nsb::containers::begin(std::as_const(v)))>();
  testing::StaticAssertTypeEq<decltype(v)::const_iterator, decltype(nsb::containers::cbegin(v))>();

  testing::StaticAssertTypeEq<decltype(a)::iterator, decltype(nsb::containers::end(a))>();
  testing::StaticAssertTypeEq<decltype(a)::const_iterator, decltype(nsb::containers::end(std::as_const(a)))>();
  testing::StaticAssertTypeEq<decltype(a)::const_iterator, decltype(nsb::containers::cend(a))>();

  testing::StaticAssertTypeEq<decltype(v)::iterator, decltype(nsb::containers::end(v))>();
  testing::StaticAssertTypeEq<decltype(v)::const_iterator, decltype(nsb::containers::end(std::as_const(v)))>();
  testing::StaticAssertTypeEq<decltype(v)::const_iterator, decltype(nsb::containers::cend(v))>();

  testing::StaticAssertTypeEq<decltype(a)::reverse_iterator, decltype(nsb::containers::rbegin(a))>();
  testing::StaticAssertTypeEq<decltype(a)::const_reverse_iterator, decltype(nsb::containers::rbegin(std::as_const(a)))>();
  testing::StaticAssertTypeEq<decltype(a)::const_reverse_iterator, decltype(nsb::containers::crbegin(a))>();

  testing::StaticAssertTypeEq<decltype(v)::reverse_iterator, decltype(nsb::containers::rbegin(v))>();
  testing::StaticAssertTypeEq<decltype(v)::const_reverse_iterator, decltype(nsb::containers::rbegin(std::as_const(v)))>();
  testing::StaticAssertTypeEq<decltype(v)::const_reverse_iterator, decltype(nsb::containers::crbegin(v))>();

  testing::StaticAssertTypeEq<decltype(a)::reverse_iterator, decltype(nsb::containers::rend(a))>();
  testing::StaticAssertTypeEq<decltype(a)::const_reverse_iterator, decltype(nsb::containers::rend(std::as_const(a)))>();
  testing::StaticAssertTypeEq<decltype(a)::const_reverse_iterator, decltype(nsb::containers::crend(a))>();

  testing::StaticAssertTypeEq<decltype(v)::reverse_iterator, decltype(nsb::containers::rend(v))>();
  testing::StaticAssertTypeEq<decltype(v)::const_reverse_iterator, decltype(nsb::containers::rend(std::as_const(v)))>();
  testing::StaticAssertTypeEq<decltype(v)::const_reverse_iterator, decltype(nsb::containers::crend(v))>();
}

TEST(IteratorTest, RangeAccess)
{
  nsb::containers::Array<int, 5> a{10, 11, 12, 13, 14};
  int sum1{};
  int sum2{};
  int sum3{};
  int sum4{};
  int v1 = 42;
  int v2 = 555;
  auto itb = begin(a) + 1;
  auto itrb = rbegin(a) + 1;
  auto ite = end(a) - 2;
  auto itre = rend(a) - 2;

  sum1 = *begin(a) + *(end(a) - 1);
  sum2 = *cbegin(a) + *(cend(a) -1);
  sum3 = *rbegin(a) + *(rend(a) - 1);
  sum4 = *crbegin(a) + *(crend(a) - 1);

  *itb = v1;
  *itrb = v2;

  ASSERT_EQ(sum1, sum2);
  ASSERT_EQ(sum2, sum3);
  ASSERT_EQ(sum3, sum4);

  ASSERT_EQ(*itb, v1);
  ASSERT_EQ(*itrb, v2);

  ASSERT_EQ(*itb, *itre);
  ASSERT_EQ(*ite, *itrb);
}