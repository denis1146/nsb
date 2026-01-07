#include "Timers.h"

#include <boost/asio.hpp>
#include <thread>
#include <iostream>

namespace
{
  enum class TimerType
  {
    SYNC,
    ASYNC,
    PERIODIC,
    PARALLEL,
  };

  void periodic(const boost::system::error_code& err, boost::asio::steady_timer& t, int& count)
  {
    std::cout << count << " [" << err << "]" << std::endl;
    --count;
    if (!count) return;

    t.expires_at(t.expiry() + boost::asio::chrono::seconds(1));
    t.async_wait([&t, &count](const boost::system::error_code& e) {
      periodic(e, t, count);
    });
  }

  class PeriodicParallel
  {
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer t1_;
    boost::asio::steady_timer t2_;
    int count_{6};

  public:
    PeriodicParallel(boost::asio::io_context& io)
    : strand_{boost::asio::make_strand(io)}
    , t1_{io, boost::asio::chrono::seconds(1)}
    , t2_{io, boost::asio::chrono::seconds(1)}
    {
      t1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&PeriodicParallel::print<1>, this)));
      t2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&PeriodicParallel::print<2>, this)));
    }

    template<size_t n>
    void print()
    {
      static_assert(n == 1 || n == 2);
      if (!count_) return;
      
      std::cout << count_ << " Timer " << n << " [ thread_id = " <<
        std::this_thread::get_id() << "] " << std::endl;
      --count_;

      if constexpr (n == 1) {
        t1_.expires_at(t1_.expiry() + boost::asio::chrono::seconds(1));
        t1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&PeriodicParallel::print<1>, this)));
      }
      else {
        t2_.expires_at(t2_.expiry() + boost::asio::chrono::seconds(1));
        t2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&PeriodicParallel::print<2>, this)));
      }
    }
  };
}


void asioTimers()
{
  boost::asio::io_context io;
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));

  constexpr TimerType timerType{TimerType::PARALLEL};
  if constexpr (TimerType::SYNC == timerType) {
    t.wait();
  }
  else if constexpr (TimerType::ASYNC == timerType) {
    t.async_wait([](const boost::system::error_code& e){
      std::cout << "async_wait: " << e << std::endl;
    });
    io.run();
  }
  else if constexpr (TimerType::PERIODIC == timerType) {
    int count = 3;
    t.async_wait([&t, &count](const boost::system::error_code& e){
        periodic(e, t, count);
    });
    io.run();
  }
  else if constexpr (TimerType::PARALLEL == timerType) {
    PeriodicParallel p{io};
    std::jthread jthread{[&io](){ io.run(); }};
    io.run();
  }
}
