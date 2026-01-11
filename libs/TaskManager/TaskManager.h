#pragma once

#include <memory>
#include <functional>

namespace nsb
{

struct ITask
{
  using Function = std::function<void()>;

  ITask() = default;
  virtual ~ITask() = default;

  // virtual void cancel() = 0;
  // virtual void start() = 0;
  virtual void stop() = 0;
  virtual bool stopped() = 0;
  // virtual void wait() = 0;
  // virtual int get() = 0;
};

struct IThreadPool
{
  virtual ~IThreadPool() = default;
  virtual void start() = 0;
  virtual void stop(bool force) = 0;
  virtual bool stopped() = 0;

  virtual void restart(bool force = false)
  {
    stop(force);
    start();
  }
};

struct ITaskManager
{
  virtual ~ITaskManager() = default;
  virtual void start() = 0;
  virtual void stop(bool force) = 0;
  virtual bool stopped() = 0;
  // void addTask();
  virtual std::shared_ptr<ITask> addTask(ITask::Function func) = 0;
  virtual void removeTask(std::shared_ptr<ITask> task) = 0;

  void stop()
  {
    stop(false);
  }
  void forceStop(bool force = false)
  {
    stop(force);
  }
};

}
