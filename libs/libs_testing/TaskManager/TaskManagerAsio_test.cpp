#include "TaskManager/TaskManagerAsio.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <ranges>
#include <thread>

TEST(MakeTaskManager, makeTaskManager)
{
  auto tm1 = nsb::makeTaskManager();
  EXPECT_TRUE(tm1);
  EXPECT_FALSE(tm1->stopped());

  auto tm2 = nsb::makeTaskManager(2);
  EXPECT_TRUE(tm2);
  EXPECT_FALSE(tm2->stopped());

  tm1->start();
  EXPECT_FALSE(tm1->stopped());
  tm1->start();
  EXPECT_FALSE(tm1->stopped());
  tm1->stop();
  EXPECT_TRUE(tm1->stopped());
  tm1->stop();
  EXPECT_TRUE(tm1->stopped());
  auto task = tm1->addTask([] {});
  EXPECT_FALSE(task);
}

TEST(TaskManagerAsio, TaskManagerAsio)
{
  using namespace std::chrono_literals;
  constexpr auto number = 10'000;
  std::atomic_int counter = 0;
  nsb::ITask::Function func = [&counter] { ++counter; };
  nsb::ITask::Function funcExpectation = [] { std::this_thread::sleep_for(1ms); };
  auto taskManager = nsb::makeTaskManager(42);

  for ([[maybe_unused]] auto i : std::views::iota(0, number))
    taskManager->addTask(func);
  taskManager->stop();
  EXPECT_TRUE(taskManager->stopped());
  EXPECT_EQ(counter.load(), number);

  counter.store(0);
  taskManager->start();
  EXPECT_FALSE(taskManager->stopped());
  for ([[maybe_unused]] auto i : std::views::iota(0, number))
    taskManager->addTask(funcExpectation);
  taskManager->forceStop();
  EXPECT_TRUE(taskManager->stopped());
  EXPECT_NE(counter.load(), number);
}

TEST(Task, Task)
{
  using namespace std::chrono_literals;
  constexpr auto number = 10;
  std::atomic_int counter = 0;
  nsb::ITask::Function func = [&counter] {
    std::this_thread::sleep_for(1ms);
    ++counter;
  };
  auto taskManager = nsb::makeTaskManager(1);

  nsb::ITaskManager::ITask_ptr task;
  for ([[maybe_unused]] auto i : std::views::iota(0, number))
    task = taskManager->addTask(func);
  EXPECT_FALSE(task->stopped());
  task->stop();
  EXPECT_TRUE(task->stopped());
  taskManager->stop();
  EXPECT_EQ(counter.load(), number - 1);
}
