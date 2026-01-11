#include <gtest/gtest.h>

#include <sstream>
#include <iostream>
#include <thread>
#include "TaskManager/TaskManagerAsio.h"

TEST(TaskManagerAsio, TaskManagerAsio)
{
  size_t i = 0;
  nsb::ITask::Function f = [&i] {
    auto j = ++i;
    std::stringstream s;
    s << "[  +TEST+  ]" << " TEST +++++++++++++ " << j << "  id = ";
    s << std::this_thread::get_id() << "\n";
    std::cout << s.str();
  };

  auto f0 = []{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5000ms);
  };


  auto taskManager = nsb::makeTaskManager(5);
  taskManager->addTask(f0);

  for (int j = 0; j < 100; ++j)
    taskManager->addTask(f);

  auto f1 = []{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5000ms);
    EXPECT_TRUE(false) << " ERROR ---------------------------------------------\n" << std::endl; 
  };
  auto task1 = taskManager->addTask(f1);
  task1->stop();
  auto task2 = taskManager->addTask(f1);
  taskManager->removeTask(std::move(task2));

  taskManager->stop();

  taskManager->start();

}
