#include "TaskManagerAsio.h"

#include <thread>
#include <optional>
#include <utility>
#include <stdexcept>
#include <list>
#include <type_traits>
#include <boost/asio.hpp>

namespace nsb
{

template <class T>
struct SharedEnablerHelper: T
{
  SharedEnablerHelper(auto&&... vars): T(std::forward<decltype(vars)>(vars)...){}
  // template<class... Args>
  // SharedEnablerHelper(Args&&... args): T(std::forward<Args>(args)...){}
};

template<class T, class... Args>
std::shared_ptr<T> makeSharedEnabler(Args&&... args)
{
  auto p = std::make_shared<SharedEnablerHelper<T>>(std::forward<Args>(args)...);
  return std::static_pointer_cast<T>(p);
}

template <class RetType, class F, class... Args>
std::pair<std::future<RetType>, ITask::Function> 
makeFutureFunction(F&& func, Args&&... args)
{
  std::packaged_task<RetType(Args...)> task{std::forward<F>(func)};
  std::future<RetType> future = task.get_future();
  ITask::Function function = [t = std::move(task),
    ...vars = std::forward<Args>(args)] () {
    t(std::forward<decltype(vars)>(vars)...);
  };

  return {future, function};
}

class Task: public ITask, public std::enable_shared_from_this<Task>
{
public:
  friend class SharedEnablerHelper<Task>;
  static std::shared_ptr<Task> makeTask(ITask::Function&& f)
  {
    return makeSharedEnabler<Task>(std::move(f));
  }

  void stop() override
  {
    m_stopped.store(true);
  }

  bool stopped() override
  {
    return m_stopped.load();
  }

  void call(){
    m_task();
  }
  
private:
  explicit Task(ITask::Function&& task)
  : m_task{std::move(task)}
  {
  }
  
  ITask::Function m_task;
  std::atomic_bool m_stopped{false};
  // future
  // wait 
  // get
};

class ThreadPoolAsio: public IThreadPool
{
public:
  struct ExecutorError: std::runtime_error
  {
    ExecutorError()
    : runtime_error{"Error! Bad executor access, thread pool stopped"}
    {};
  };

  explicit ThreadPoolAsio(size_t threads = 0) noexcept
  : m_threadsCount{threads == 0 ?
    std::thread::hardware_concurrency() == 0 ? 4 :
    std::thread::hardware_concurrency() : threads}
  {
  }

  void start() override
  {
    std::lock_guard l{m_mutex};
    if (m_threadPool) return;

    m_threadPool.emplace(m_threadsCount);
    m_workGuard.emplace(m_threadPool->executor());
  }

  void stop(bool force) override
  {
    std::lock_guard l{m_mutex};
    if (!m_threadPool) return;
    
    m_workGuard.reset();
    if (force) {
      m_threadPool->stop();
    }
    else {
      m_threadPool->join();
    }
    m_threadPool.reset();
  }

  bool stopped()
  {
    std::lock_guard l{m_mutex};
    return !m_threadPool;
  }

  auto executor()
  {
    std::lock_guard l{m_mutex};
    if (!m_threadPool)
      throw ExecutorError{};

    return m_threadPool->executor();
  }

private:
  using thread_pool = boost::asio::thread_pool;
  using executor_work_guard = 
    boost::asio::executor_work_guard<decltype(std::declval<thread_pool>().executor())>;

  const size_t m_threadsCount;
  std::mutex m_mutex;
  std::optional<thread_pool> m_threadPool;
  std::optional<executor_work_guard> m_workGuard;  
};

class TaskManagerAsio: public ITaskManager
{
public:
  TaskManagerAsio(size_t threads)
  : m_ThreadPool{threads}
  {
    m_ThreadPool.start();
  }

  void start() override
  {
    m_ThreadPool.start();
  }

  void stop(bool force) override
  {
    m_ThreadPool.stop(force);
  }

  bool stopped() override
  {
    return m_ThreadPool.stopped();
  }

  std::shared_ptr<ITask> addTask(ITask::Function func) override
  {
    auto task = Task::makeTask(std::move(func));
    std::lock_guard l{m_mutex};
    m_tasks.push_back(task);
    boost::asio::post(m_ThreadPool.executor(), [wp = task->weak_from_this()](){
      auto sp = wp.lock();
      if (!sp || sp->stopped())
        return;

      sp->call();
    });
    return task;
  }

  void removeTask(std::shared_ptr<ITask> task) override
  {
    std::lock_guard l{m_mutex};
    task->stop();
    m_tasks.remove(std::static_pointer_cast<Task>(task));
  }

private:
  std::mutex m_mutex;
  std::list<std::shared_ptr<Task>> m_tasks;
  ThreadPoolAsio m_ThreadPool;
};

std::unique_ptr<ITaskManager> makeTaskManager(size_t threads)
{
  return std::make_unique<TaskManagerAsio>(threads);
}

} // ~nsb
