#pragma once

#include "TaskManager.hpp"

namespace nsb {

/// @brief Creates a task manager instance.
/// @param threads Number of threads to use (0 for default).
/// @return Shared pointer to the task manager.
std::shared_ptr<ITaskManager> makeTaskManager(size_t threads = 0);

}  // namespace nsb
