#include <list>
#include <thread>
#include <future>
#include <condition_variable>
#include <barrier>

namespace nsb::numeric {

namespace details {

inline static std::size_t getThreadCount(std::size_t length)
{
  const std::size_t hc = std::thread::hardware_concurrency();
  if (hc == 0)
    return 2;

  constexpr std::size_t perThreads = 16;
  const std::size_t maxThreads = (length + perThreads - 1) / perThreads;
  return std::min(hc, maxThreads);
};

template <class ForwardIt>
void for_range_blocks(ForwardIt first, ForwardIt last, std::function<void(ForwardIt, ForwardIt)> block)
{
  const std::size_t length = std::distance(first, last);
  const std::size_t num_threads = details::getThreadCount(length);
  const std::size_t block_sz = (length  + num_threads - 1) / num_threads;

  auto block_start = first;
  for (std::size_t i = 0; i < num_threads; ++i) {
    auto block_end = block_start;
    std::advance(block_end, block_sz);
    if (static_cast<std::size_t>(std::distance(first, block_end)) > length)
      block_end = last;

    block(block_start, block_end);
    block_start = block_end;
  }
}

} // ~details

template <class ExecutionPolicy, class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last)
{
  return std::reduce(std::forward<ExecutionPolicy>(policy), first, last);
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::std_sequenced_accumulate_policy, ForwardIt first, ForwardIt last)
{
  return std::accumulate(first, last, iterator_traits_t<ForwardIt>{});
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::std_sequenced_reduce_policy, ForwardIt first, ForwardIt last)
{
  return std::reduce(first, last);
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::sequenced_policy, ForwardIt first, ForwardIt last)
{
  auto ret = iterator_traits_t<ForwardIt>{};
  for (; first != last; ++first)
    ret = std::move(ret) + *first;
  return ret;
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_thread_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};

  using ReturnType = iterator_traits_t<ForwardIt>;
  const std::size_t length = std::distance(first, last);
  const std::size_t num_threads = details::getThreadCount(length);
  const std::size_t block_sz = length/num_threads;

  std::vector<ReturnType> rets(num_threads);
  std::vector<std::thread> threads;
  threads.reserve(num_threads - 1);

  ForwardIt block_start = first;
  for (std::size_t i = 0; i < num_threads - 1; ++i) {
    ForwardIt block_end = block_start;
    std::advance(block_end, block_sz);
    auto acc = [&rets, i](auto start, auto end) {
      rets[i] = reduce(SEQ, start, end);
    };

    threads.emplace_back(std::move(acc), block_start, block_end);
    block_start = block_end;
  }
  rets[num_threads - 1] = (reduce(SEQ, block_start, last));

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  return reduce(SEQ, cbegin(rets), cend(rets));
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_jthread_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};

  using ReturnType = iterator_traits_t<ForwardIt>;
  const std::size_t length = std::distance(first, last);
  const std::size_t num_threads = details::getThreadCount(length);
  const std::size_t block_sz = length/num_threads;

  std::vector<ReturnType> rets(num_threads);
  {
    std::vector<std::jthread> threads;
    threads.reserve(num_threads - 1);

    ForwardIt block_start = first;
    for (std::size_t i = 0; i < num_threads - 1; ++i) {
      ForwardIt block_end = block_start;
      std::advance(block_end, block_sz);
      auto acc = [&rets, i](auto start, auto end) {
        rets[i] = reduce(SEQ, start, end);
      };

      threads.emplace_back(std::move(acc), block_start, block_end);
      block_start = block_end;
    }
    rets[num_threads - 1] = (reduce(SEQ, block_start, last));
  }

  return reduce(SEQ, cbegin(rets), cend(rets));
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_promise_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};
  
  using ReturnType = iterator_traits_t<ForwardIt>;
  std::list<std::future<ReturnType>> acc_futures;
  auto block = [&acc_futures] (auto block_start, auto block_end) mutable {
    auto acc = [](std::promise<ReturnType> p, auto start, auto end) {
      p.set_value(reduce(SEQ, start, end));
    };
    std::promise<ReturnType> p;
    acc_futures.emplace_back(p.get_future());

    std::thread t{ std::move(acc), std::move(p), block_start, block_end };
    t.detach();
  };

  details::for_range_blocks<ForwardIt>(first, last, block);
  ReturnType ret{};
  for (auto& f : acc_futures) {
    ret = ret + f.get();
  }
  return ret;
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_packaged_task_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};
  
  using ReturnType = iterator_traits_t<ForwardIt>;
  std::list<std::future<ReturnType>> acc_futures;
  auto block = [&acc_futures] (auto block_start, auto block_end) mutable {
    auto acc = [](auto start, auto end) -> ReturnType {
      return reduce(SEQ, start, end);
    };
    std::packaged_task<ReturnType(ForwardIt, ForwardIt)> pt{ std::move(acc) };
    acc_futures.emplace_back(pt.get_future());

    std::thread t{ std::move(pt), block_start, block_end };
    t.detach();
  };

  details::for_range_blocks<ForwardIt>(first, last, block);
  ReturnType ret{};
  for (auto& f : acc_futures) {
    ret = ret + f.get();
  }
  return ret;
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_async_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};
  
  using ReturnType = iterator_traits_t<ForwardIt>;
  std::list<std::future<ReturnType>> acc_futures;
  auto block = [&acc_futures] (auto block_start, auto block_end) mutable {
    auto acc = [](auto start, auto end) -> ReturnType {
      return reduce(SEQ, start, end);
    };

    acc_futures.emplace_back(std::async(std::launch::async, std::move(acc), block_start, block_end));
  };

  details::for_range_blocks<ForwardIt>(first, last, block);
  ReturnType ret{};
  for (auto& acc : acc_futures) {
    ret = ret + acc.get();
  }
  return ret;
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_cv_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};
  
  using ReturnType = iterator_traits_t<ForwardIt>;
  std::list<ReturnType> rets;
  std::condition_variable cv;
  std::mutex mut;
  bool isReadyTreads = false;
  std::size_t counter_threads = 0;

  auto acc = [&cv, &mut, &counter_threads, &isReadyTreads] (ReturnType& ret, auto start, auto end) mutable {
    std::unique_lock l{ mut };
    cv.wait(l, [&isReadyTreads] { return isReadyTreads; });
    ret = reduce(SEQ, start, end);
    --counter_threads;
    cv.notify_one();
  };

  std::list<std::jthread> threads;
  auto block = [&threads, &rets, &acc, &counter_threads] (auto block_start, auto block_end) mutable {
    ++counter_threads;
    threads.emplace_back(acc, std::ref(rets.emplace_back()), block_start, block_end);
  };
  details::for_range_blocks<ForwardIt>(first, last, block);

  std::unique_lock lock{ mut };
  isReadyTreads = true;
  lock.unlock();
  cv.notify_all();
  lock.lock();
  cv.wait(lock, [&counter_threads] { return counter_threads == 0; });

  return reduce(SEQ, cbegin(rets), cend(rets));
}

template <class ForwardIt>
iterator_traits_t<ForwardIt>
reduce(execution::parallel_barrier_policy, ForwardIt first, ForwardIt last)
{
  if (first == last)
    return {};
  
  using ReturnType = iterator_traits_t<ForwardIt>;
  std::list<ReturnType> rets;
  std::promise<ReturnType> p;
  std::future<ReturnType> ret = p.get_future();
  bool isReady = false;
  const std::size_t num_threads = details::getThreadCount(std::distance(first, last));
  std::barrier sync(num_threads, [&rets, &isReady, p = std::move(p)] mutable {
    if (isReady) 
      p.set_value(reduce(SEQ, rets.cbegin(), rets.cend()));
    isReady = true;
  });

  auto acc = [&sync](ReturnType& ret, auto start, auto end) mutable {
    sync.arrive_and_wait();
    ret = reduce(SEQ, start, end);
    sync.arrive_and_wait();
  };

  auto block = [&rets, &acc] (auto block_start, auto block_end) mutable {
    std::thread t(acc, std::ref(rets.emplace_back()), block_start, block_end);
    t.detach();
  };

  details::for_range_blocks<ForwardIt>(first, last, block);
  return ret.get();
}

} // nsb::numeric
