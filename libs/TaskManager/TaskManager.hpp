#pragma once

#include <functional>
#include <memory>

namespace nsb {

/// @brief Interface for tasks.
struct ITask {
  using Function = std::function<void()>;

  ITask() = default;
  virtual ~ITask() = default;

  /// @brief Stops the task.
  virtual void stop() = 0;

  /// @brief Checks if the task is stopped.
  /// @return True if stopped, false otherwise.
  virtual bool stopped() = 0;
};

/// @brief Interface for thread pool.
struct IThreadPool {
  virtual ~IThreadPool() = default;

  /// @brief Starts the thread pool.
  virtual void start() = 0;

  /// @brief Stops the thread pool.
  /// @param force If true, forces immediate stop.
  virtual void stop(bool force) = 0;

  /// @brief Checks if the thread pool is stopped.
  /// @return True if stopped, false otherwise.
  virtual bool stopped() = 0;

  /// @brief Restarts the thread pool.
  /// @param force If true, forces stop before restart.
  virtual void restart(bool force = false)
  {
    stop(force);
    start();
  }
};

/// @brief Interface for task manager.
struct ITaskManager {
  using ITask_ptr = std::shared_ptr<ITask>;

  virtual ~ITaskManager() = default;

  /// @brief Starts the task manager.
  virtual void start() = 0;

  /// @brief Stops the task manager.
  /// @param force If true, forces immediate stop.
  virtual void stop(bool force) = 0;

  /// @brief Checks if the task manager is stopped.
  /// @return True if stopped, false otherwise.
  virtual bool stopped() = 0;

  /// @brief Adds a task to the manager.
  /// @param func The function to execute as a task.
  /// @return Shared pointer to the added task.
  virtual ITask_ptr addTask(ITask::Function func) = 0;

  /// @brief Removes a task from the manager.
  /// @param task The task to remove.
  virtual void removeTask(ITask_ptr task) = 0;

  /// @brief Stops the task manager (non-force).
  void stop() { stop(false); }

  /// @brief Forces stop of the task manager.
  void forceStop() { stop(true); }
};

}  // namespace nsb
