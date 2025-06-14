#include <gtest/gtest.h>

#include <array>
#include "numeric/reduce.h"

namespace 
{

template <class E>
class ReduceTest: public testing::Test
{
public:
  using Execution = E;
};

namespace execution = nsb::numeric::execution;
using ExecutionTypes = ::testing::Types<
  execution::std_sequenced_policy
, execution::std_parallel_policy
, execution::std_parallel_unsequenced_policy
, execution::std_unsequenced_policy

, execution::std_sequenced_accumulate_policy
, execution::std_sequenced_reduce_policy

, execution::sequenced_policy
, execution::parallel_thread_policy
, execution::parallel_jthread_policy
, execution::parallel_promise_policy
, execution::parallel_packaged_task_policy
, execution::parallel_async_policy
, execution::parallel_cv_policy
, execution::parallel_barrier_policy
// , execution::parallel_latch_policy
>;
TYPED_TEST_SUITE(ReduceTest, ExecutionTypes);

}

TYPED_TEST(ReduceTest, reduce)
{
  // Arrange
  using namespace std::string_literals;
  const std::vector<int> v(160, 2);

  const std::array<int, 0> ai{};
  const std::array<double, 0> ad{};
  const std::array<std::string, 0> as{};
  
  const std::array ai1{1};
  const std::array ad1{1};
  const std::array as1{"1"s};

  const std::array ai2{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const std::array ad2{0., 1.1, 2.2, 3., 4., 5., 6., 7., 8., 9.};
  const std::array<std::string, 10> as2{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    
  // Act
  const auto e = typename TestFixture::Execution{};
  const auto acc_vi = nsb::numeric::reduce(e, std::cbegin(v), std::cend(v));

  const auto i = nsb::numeric::reduce(e, std::cbegin(ai), std::cend(ai));
  const auto d = nsb::numeric::reduce(e, std::cbegin(ad), std::cend(ad));
  const auto s = nsb::numeric::reduce(e, std::cbegin(as), std::cend(as));

  const auto i1 = nsb::numeric::reduce(e, std::cbegin(ai1), std::cend(ai1));
  const auto d1 = nsb::numeric::reduce(e, std::cbegin(ad1), std::cend(ad1));
  const auto s1 = nsb::numeric::reduce(e, std::cbegin(as1), std::cend(as1));

  const auto i2 = nsb::numeric::reduce(e, std::cbegin(ai2), std::cend(ai2));
  const auto d2 = nsb::numeric::reduce(e, std::cbegin(ad2), std::cend(ad2));
  const auto s2 = nsb::numeric::reduce(e, std::cbegin(as2), std::cend(as2));

  // Assert
  ASSERT_EQ(acc_vi, 320);

  ASSERT_EQ(i, 0);
  ASSERT_DOUBLE_EQ(d, 0.);
  ASSERT_EQ(s, ""s);

  ASSERT_EQ(i1, 1);
  ASSERT_DOUBLE_EQ(d1, 1.);
  ASSERT_EQ(s1, "1"s);

  ASSERT_EQ(i2, 45);
  ASSERT_DOUBLE_EQ(d2, 45.3);
  if constexpr (std::is_same_v< typename TestFixture::Execution, execution::std_unsequenced_policy>) {
    GTEST_SKIP() << "SKIP std_unsequenced_policy [" << s2 << "]";
  }
  else {
    ASSERT_EQ(s2, std::string{"0123456789"});
  }
}

TYPED_TEST(ReduceTest, reduce_for_container)
{
  // Arrange
  using namespace std::string_literals;
  const std::vector<int> vi(160, 2);
  constexpr std::array<float, 0> af{};
  constexpr std::array ad{0., 1.1, 2.2, 3., 4., 5., 6., 7., 8., 9.};
  const unsigned long long all[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const std::array<std::string, 10> as{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

  // Act
  const auto e = typename TestFixture::Execution{};
  const auto i = nsb::numeric::reduce(e, vi);
  const auto f = nsb::numeric::reduce(e, af);
  const auto d = nsb::numeric::reduce(e, ad);
  const auto ll = nsb::numeric::reduce(e, all);
  const auto ui = nsb::numeric::reduce(e, std::vector<uint16_t>{500, 50, 5});
  const auto s = nsb::numeric::reduce(e, as);

  // Assert
  ASSERT_EQ(i, 320);
  ASSERT_EQ(f, 0);
  ASSERT_EQ(d, 45.3);
  ASSERT_EQ(ui, 555);
  ASSERT_EQ(ll, 45ull);

  if constexpr (std::is_same_v< typename TestFixture::Execution, execution::std_unsequenced_policy>) {
    GTEST_SKIP() << "SKIP std_unsequenced_policy (reduce_for_container) [" << s << "]";
  }
  else {
    ASSERT_EQ(s, std::string{"0123456789"});
  }
}
