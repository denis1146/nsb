#include "TaskManagerAsio.hpp"

#include <boost/asio.hpp>
#include <boost/scope_exit.hpp>
#include <list>
#include <optional>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>

namespace {

template <class T>
struct SharedEnablerHelper : T {
  explicit SharedEnablerHelper(auto&&... vars)
  : T(std::forward<decltype(vars)>(vars)...)
  {
  }
  // template<class... Args>
  // SharedEnablerHelper(Args&&... args): T(std::forward<Args>(args)...){}
};

template <class T, class... Args>
std::shared_ptr<T> makeSharedEnabler(Args&&... args)
{
  auto p = std::make_shared<SharedEnablerHelper<T>>(std::forward<Args>(args)...);
  return std::static_pointer_cast<T>(p);
}

}  // namespace

namespace nsb {

template <class RetType, class F, class... Args>
std::pair<std::future<RetType>, ITask::Function> makeFutureFunction(F&& func, Args&&... args)
{
  std::packaged_task<RetType(Args...)> task{ std::forward<F>(func) };
  std::future<RetType> future = task.get_future();
  ITask::Function function = [t = std::move(task), ... vars = std::forward<Args>(args)]() {
    t(std::forward<decltype(vars)>(vars)...);
  };

  return { future, function };
}

class Task : public ITask, public std::enable_shared_from_this<Task> {
public:
  friend class SharedEnablerHelper<Task>;
  static std::shared_ptr<Task> makeTask(ITask::Function&& f)
  {
    return makeSharedEnabler<Task>(std::move(f));
  }

  void stop() override { m_stopped.store(true); }

  bool stopped() override { return m_stopped.load(); }

  void call()
  {
    m_task();
    stop();
  }

private:
  explicit Task(ITask::Function&& task)
  : m_task{ std::move(task) }
  {
  }

  ITask::Function m_task;
  std::atomic_bool m_stopped{ false };
};

class ThreadPoolAsio : public IThreadPool {
public:
  struct ExecutorError : std::runtime_error {
    ExecutorError()
    : runtime_error{
      "Error! Bad executor access, thread pool stopped (nsb::ThreadPoolAsio::ExecutorError)"
    } {};
  };

  ThreadPoolAsio(const ThreadPoolAsio&) = delete;
  ThreadPoolAsio& operator=(const ThreadPoolAsio&) = delete;

  explicit ThreadPoolAsio(size_t threads = 0) noexcept
  : m_threadsCount{ threads == 0 ? std::thread::hardware_concurrency() == 0
                                       ? 4
                                       : std::thread::hardware_concurrency()
                                 : threads }
  {
  }

  void start() override
  {
    if (m_threadPool)
      return;

    m_threadPool.emplace(m_threadsCount);
    m_workGuard.emplace(m_threadPool->executor());
  }

  void stop(bool force) override
  {
    if (!m_threadPool)
      return;

    m_workGuard.reset();
    if (force) {
      m_threadPool->stop();
    } else {
      m_threadPool->join();
    }
    m_threadPool.reset();
  }

  bool stopped() { return !m_threadPool; }

  auto executor() noexcept(false)
  {
    if (!m_threadPool)
      throw ExecutorError{};

    return m_threadPool->executor();
  }

private:
  using thread_pool = boost::asio::thread_pool;
  using executor_work_guard =
      boost::asio::executor_work_guard<decltype(std::declval<thread_pool>().executor())>;

  const size_t m_threadsCount;
  std::optional<thread_pool> m_threadPool;
  std::optional<executor_work_guard> m_workGuard;
};

class TaskManagerAsio : public ITaskManager, public std::enable_shared_from_this<TaskManagerAsio> {
public:
  TaskManagerAsio(const TaskManagerAsio&) = delete;
  TaskManagerAsio& operator=(const TaskManagerAsio&) = delete;

  friend class SharedEnablerHelper<TaskManagerAsio>;
  static std::shared_ptr<TaskManagerAsio> makeTaskManagerAsio(size_t threads)
  {
    return makeSharedEnabler<TaskManagerAsio>(threads);
  }

  void start() override
  {
    {
      std::lock_guard l{ m_threadPoolMutex };
      m_threadPool.start();
    }
    {
      std::lock_guard l{ m_tasksMutex };
      m_tasks.clear();
    }
  }

  void stop(bool force) override
  {
    if (force) {
      std::lock_guard l{ m_tasksMutex };
      m_tasks.clear();
    }
    std::lock_guard l{ m_threadPoolMutex };
    m_threadPool.stop(force);
  }

  bool stopped() override
  {
    std::lock_guard l{ m_threadPoolMutex };
    return m_threadPool.stopped();
  }

  ITask_ptr addTask(ITask::Function func) override
  {
    auto tsk = Task::makeTask(std::move(func));
    {
      std::lock_guard l{ m_tasksMutex };
      m_tasks.push_back(tsk);
    }
    try {
      std::lock_guard l{ m_threadPoolMutex };
      boost::asio::post(m_threadPool.executor(),
                        [t = tsk->weak_from_this(), tm = weak_from_this()]() {
                          auto task = t.lock();

                          BOOST_SCOPE_EXIT(&tm, task)
                          {
                            if (auto taskManager = tm.lock())
                              taskManager->removeTask(task);
                          }
                          BOOST_SCOPE_EXIT_END;

                          if (!task || task->stopped())
                            return;

                          task->call();
                        });
    } catch (const ThreadPoolAsio::ExecutorError& e) {
      tsk.reset();
    }

    return tsk;
  }

  void removeTask(ITask_ptr task) override
  {
    if (!task)
      return;
    task->stop();
    std::lock_guard l{ m_tasksMutex };
    m_tasks.remove(std::static_pointer_cast<Task>(task));
  }

private:
  TaskManagerAsio(size_t threads)
  : m_threadPool{ threads }
  {
    start();
  }

  std::mutex m_tasksMutex;
  std::list<ITask_ptr> m_tasks;
  std::mutex m_threadPoolMutex;
  ThreadPoolAsio m_threadPool;
};

std::shared_ptr<ITaskManager> makeTaskManager(size_t threads)
{
  return TaskManagerAsio::makeTaskManagerAsio(threads);
}

}  // namespace nsb
