#include "ScopeExit.h"

#include <memory>
#include <cstdio>
#include <string_view>
#include <concepts>
#include <functional>
#include <boost/scope_exit.hpp>
#include <boost/core/noncopyable.hpp>

namespace
{

struct Se : private boost::noncopyable {
  template<class F>
  requires std::invocable<F>
  explicit Se(F&& f): m_f{std::forward<F>(f)}{}
  ~Se() { m_f(); }
private:
  std::function<void()> m_f;
};

}

void ScopeExit::run() {
  auto show = [] (std::string_view st){
    std::puts(st.data());
  };
  show(m_se);
  BOOST_SCOPE_EXIT(&show, this_) {
    show(this_->m_se);
  } BOOST_SCOPE_EXIT_END;

  {
    const auto st = "Using the unique_ptr";
    auto d = [&st](void*) { std::puts(st); };
    std::unique_ptr<void, decltype(d)> t{reinterpret_cast<void*>(42), d};
  }
  {
    // std::unique_ptr<std::FILE, int(*)(std::FILE*)> file(std::fopen("myFile.txt", "a"), &std::fclose);
    // if (file.get()) std::fputs("Using the unique_ptr (std::FILE)\n", file.get());
  }
  {
    const auto st = "Using the Se class";
    auto testSe = [&st]() { std::puts(st); };
    Se se{std::move(testSe)};
  }
}
