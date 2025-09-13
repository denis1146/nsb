#pragma once

#include <iostream>
#include <exception>
#include <thread>
#include <chrono>
#include <memory>
#include <array>
#include <cstdio>

#include "ThreadGuard.h"
#include "JThread.h"

namespace unit_2 {

static void test()
{
  using namespace std::chrono_literals;

  std::cout << "unit_2\n";
  // ThreadGuard
  {
    std::cout << "1:\n";
    try {
      std::thread t { []{ std::cout << "  ThreadGuard\n"; }};
      ThreadGuard guard{ t };
      throw std::logic_error("logic_error");
    }
    catch (...) {
      std::cout << "  catch (...)\n";
    }
    std::this_thread::sleep_for(10ms);
  }

  // Buffer
  {
    std::cout << "2:\n";
    auto func = [](const std::string& st) {
      std::cout << st;
    };

    char buf[1024];
    std::snprintf(buf, std::size(buf), "  %s", "Buffer\n");
    // copy buf = char*
    // std::thread t(func, buf);            // UB <==
    std::thread t(func, std::string(buf));
    std::snprintf(buf, std::size(buf), "\n");
    t.detach();
    std::this_thread::sleep_for(10ms);
  }

  // String [Compilation error]
  {
    std::cout << "3:\n";
    auto func = [](std::string& st) {
      st += "ing\n";
    };

    std::string st = "  Str";
    // std::thread t(func, st);             // Compilation error
    std::thread t(func, std::ref(st));
    t.join();
    std::cout << st;
    std::this_thread::sleep_for(10ms);
  }

  // unique_ptr (Big Object)
  {
    std::cout << "3:\n";
    auto func = [](std::unique_ptr<std::string> obj) {
        std::cout << *obj << std::endl;
    };
    std::unique_ptr obj = std::make_unique<std::string>("  unique_ptr (Big Object)");
    std::thread t(func, std::move(obj));
    t.join();
    std::this_thread::sleep_for(10ms);
  }

  // JThread
  {
    std::cout << "4:\n";
    auto f0 = [] { std::cout << "  JThread\n"; };
    auto f1 = [] (auto p, auto p1) { std::cout << "  JThread " << p << " " << p1 << std::endl; };
    auto f3 = [] { std::cout << "  std::thread\n"; };
    auto f4 = [] { };

    std::array<JThread, 3> jthreads{};
    jthreads[0] = JThread(f0);
    jthreads[1] = JThread(f1, std::string(), 2);
    jthreads[2] = std::thread{ f3 };
    JThread jt{ std::thread{ f4 } };
    JThread jt1{ std::move(jt) };
    std::this_thread::sleep_for(10ms);
  }

  std::cout << std::endl << std::endl;
}

}