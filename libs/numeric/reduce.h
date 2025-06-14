#pragma once

#include <numeric>
#include <execution>
#include <functional>

#include "utility.h"

namespace nsb::numeric {

namespace execution {

  using std_sequenced_policy            = std::execution::sequenced_policy;
  using std_parallel_policy             = std::execution::parallel_policy;
  using std_parallel_unsequenced_policy = std::execution::parallel_unsequenced_policy;
  using std_unsequenced_policy          = std::execution::unsequenced_policy;

  class std_sequenced_accumulate_policy {};
  class std_sequenced_reduce_policy     {};

  class sequenced_policy                {};
  class parallel_thread_policy          {};
  class parallel_jthread_policy         {};
  class parallel_promise_policy         {};
  class parallel_packaged_task_policy   {};
  class parallel_async_policy           {};
  class parallel_cv_policy              {};
  class parallel_barrier_policy         {};
  class parallel_latch_policy           {};

} // ~execution


inline constexpr execution::std_sequenced_policy            STD_SEQ;
inline constexpr execution::std_parallel_policy             STD_PAR_UNSEQ;
inline constexpr execution::std_parallel_unsequenced_policy STD_PAR;
inline constexpr execution::std_unsequenced_policy          STD_UNSEQ;

inline constexpr execution::std_sequenced_accumulate_policy STD_SEQ_ACC;
inline constexpr execution::std_sequenced_reduce_policy     STD_SEQ_REDUCE;

inline constexpr execution::sequenced_policy                SEQ;    
inline constexpr execution::parallel_thread_policy          PAR_THREAD;
inline constexpr execution::parallel_jthread_policy         PAR_JTHREAD;
inline constexpr execution::parallel_promise_policy         PAR_PROMISE;
inline constexpr execution::parallel_packaged_task_policy   PAR_PT;
inline constexpr execution::parallel_async_policy           PAR_ASYNC;
inline constexpr execution::parallel_cv_policy              PAR_CV;
inline constexpr execution::parallel_barrier_policy         PAR_BARRIER;
inline constexpr execution::parallel_latch_policy           PAR_LATCH;


template <class ExecutionPolicy, class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::std_sequenced_accumulate_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::std_sequenced_reduce_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::sequenced_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_thread_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_jthread_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_promise_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_packaged_task_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_async_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_cv_policy, ForwardIt first, ForwardIt last);

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_barrier_policy, ForwardIt first, ForwardIt last);

template <class Execution, class Container>
auto
reduce(Execution&& e, const Container& c);

} // nsb::numeric

#include "reduce.hpp"
