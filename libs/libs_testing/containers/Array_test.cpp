#include <gtest/gtest.h>

#include <utility>

#include "Array.h"

namespace {

using namespace nsb::containers;

class ArrayTest: public testing::Test
{
protected:
  // static void SetUpTestSuite()
  // {
  // }

  void SetUp() override
  {
    arr1 = { 0, 1, 2, 3, 4 };
    arr2 = { 5, 6, 7, 8, 9 };
    arr3 = { 0, 1 };
  }

  static constexpr std::size_t sz1{ 5 };
  using ArrayInt = Array<int, sz1>;
  ArrayInt arr1;
  ArrayInt arr2;
  Array<int, 2> arr3;
  Array<int, 0> arr4;
  
};

}

TEST(Array, StaticTypes)
{
  constexpr std::size_t sz = 20;
  using ArrayInt = nsb::containers::Array<int, sz>;
  ArrayInt a;

  ASSERT_EQ(a.data(), a.arr);
  ASSERT_EQ(a.data(), const_cast<const ArrayInt&>(a).data());

  testing::StaticAssertTypeEq<ArrayInt::type_array, int[sz]>();
  testing::StaticAssertTypeEq<ArrayInt::value_type, int>();
  testing::StaticAssertTypeEq<ArrayInt::size_type, std::size_t>();
  testing::StaticAssertTypeEq<ArrayInt::difference_type, std::ptrdiff_t>();
  testing::StaticAssertTypeEq<ArrayInt::reference, int&>();
  testing::StaticAssertTypeEq<ArrayInt::const_reference, const int&>();
  testing::StaticAssertTypeEq<ArrayInt::pointer, int*>();
  testing::StaticAssertTypeEq<ArrayInt::const_pointer, const int*>();
  testing::StaticAssertTypeEq<ArrayInt::iterator, int*>();
  testing::StaticAssertTypeEq<ArrayInt::const_iterator, const int*>();
  testing::StaticAssertTypeEq<ArrayInt::reverse_iterator, std::reverse_iterator<int*>>();
  testing::StaticAssertTypeEq<ArrayInt::const_reverse_iterator, std::reverse_iterator<const int*>>();
}

TEST_F(ArrayTest, ElementAccess)
{
  constexpr int v1 = 42;
  constexpr int v2 = 555;
  constexpr int v3 = 500;
  auto& lv = arr1[arr1.size() - 1];
  auto* p = arr1.data();
  
  arr1.front() = v1;
  lv = v2;
  arr1.back() = v2;
  p[3] = v3;

  ASSERT_EQ(arr1[0], v1);
  ASSERT_EQ(lv, v2);

  ASSERT_EQ(arr1.at(0), v1);
  ASSERT_EQ(arr1.at(arr1.size() - 1), v2);
  ASSERT_THROW(arr1.at(arr1.size()), std::out_of_range);

  ASSERT_EQ(arr1.front(), v1);
  ASSERT_EQ(arr1.back(), v2);

  ASSERT_EQ(p[1], 1);
  ASSERT_EQ(p[3], v3);
}

TEST_F(ArrayTest, Iterators)
{
  constexpr int v1 = 42;
  constexpr int v2 = 555;
  *arr1.begin() = v1;
  auto it = arr1.end() - 1;
  *it = v2;

  ASSERT_EQ(*arr1.begin(), v1);
  ASSERT_EQ(*(arr1.end() - 1), v2);

  ASSERT_EQ(*(std::as_const(arr1).begin()), v1);
  ASSERT_EQ(*((std::as_const(arr1)).end() - 1), v2);

  ASSERT_EQ(*arr1.cbegin(), v1);
  ASSERT_EQ(*(arr1.cend() - 1), v2);


  ASSERT_EQ(*arr1.rbegin(), v2);
  ASSERT_EQ(*(arr1.rend() - 1), v1);

  ASSERT_EQ(*(std::as_const(arr1)).rbegin(), v2);
  ASSERT_EQ(*(std::as_const(arr1).rend() - 1), v1);

  ASSERT_EQ(*arr1.rcbegin(), v2);
  ASSERT_EQ(*(arr1.rcend() - 1), v1);
}

TEST_F(ArrayTest, Capacity)
{
  ASSERT_FALSE(arr1.empty());
  ASSERT_TRUE(arr4.empty());

  ASSERT_EQ(arr1.size(), sz1);
  ASSERT_EQ(arr4.size(), 0);

  ASSERT_EQ(arr1.max_size(), sz1);
  ASSERT_EQ(arr4.max_size(), 0);

  ASSERT_EQ(arr1.static_size, sz1);
}

TEST_F(ArrayTest, Operations)
{
  constexpr int val = 42;
  const auto arr = arr1;

  arr2.fill(val);
  arr1.swap(arr2);

  for (const auto& v : arr1) {
      ASSERT_EQ(v, val);
  }
  
  for (ArrayInt::size_type i = 0; i < arr2.size(); ++i){
    ASSERT_EQ(arr2[i], arr[i]);
  }
}

TEST_F(ArrayTest, Comparisons)
{
  const auto arr = arr1;
  
  ASSERT_TRUE(arr1 != arr2);
  ASSERT_FALSE(arr1 == arr2);
  ASSERT_TRUE(arr1 == arr);
  ASSERT_FALSE(arr1 != arr);

  ASSERT_TRUE(arr1 < arr2);
  ASSERT_FALSE(arr1 > arr2);

  ASSERT_TRUE(arr1 <= arr);
  ASSERT_TRUE(arr1 <= arr2);
  ASSERT_FALSE(arr2 <= arr1);
  
  ASSERT_TRUE(arr1 >= arr);
  ASSERT_FALSE(arr1 >= arr2);
  ASSERT_TRUE(arr2 >= arr1);

  ASSERT_NE(arr1 <=> arr, std::weak_ordering::less);
  ASSERT_NE(arr1 <=> arr, std::weak_ordering::greater);
  ASSERT_EQ(arr1 <=> arr, std::weak_ordering::equivalent);

  ASSERT_EQ(arr1 <=> arr2, std::weak_ordering::less);
  ASSERT_NE(arr1 <=> arr2, std::weak_ordering::greater);
  ASSERT_NE(arr1 <=> arr2, std::weak_ordering::equivalent);

  ASSERT_NE(arr1 <=> arr, std::weak_ordering::less);
  ASSERT_NE(arr1 <=> arr, std::weak_ordering::greater);
  ASSERT_EQ(arr1 <=> arr, std::weak_ordering::equivalent);

  ASSERT_TRUE((arr1 <=> arr) == 0);
  ASSERT_TRUE((arr <=> arr1) == 0);
  ASSERT_TRUE((arr1 <=> arr2) < 0);
  ASSERT_TRUE((arr2 <=> arr1) > 0);
}
