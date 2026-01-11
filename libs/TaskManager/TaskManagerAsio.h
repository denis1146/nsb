#pragma once

#include "TaskManager.h"

namespace nsb
{

std::unique_ptr<ITaskManager> makeTaskManager(size_t threads = 0);

} // ~nsb
