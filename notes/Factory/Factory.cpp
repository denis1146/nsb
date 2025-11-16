#include "Factory.h"

#include <cassert>

#include "predef.h"


namespace {

struct WindowsFactory: IFooFactory {
  std::string show() override {
    auto st = std::string(17, '=');
    return st + " WindowsFactory " + st;
  };
};

struct LinuxFactory: IFooFactory {
  std::string show() override {
    auto st = std::string(18, '=');
    return st + " LinuxFactory " + st;
  };
};

struct MacOsFactory: IFooFactory {
  std::string show() override {
    auto st = std::string(18, '=');
    return st + " MacOsFactory " + st;
  };
};

template<nsb::os os>
std::unique_ptr<IFooFactory> makeFooFactory()
{
  if constexpr (nsb::os::windows_os == os) {
    return std::make_unique<WindowsFactory>();
  }
  else if constexpr (nsb::os::linux_os == os) {
    return std::make_unique<LinuxFactory>();
  }
  else if constexpr (nsb::os::mac_os == os) {
    return std::make_unique<MacOsFactory>();
  }

  assert(false);
  return {};
}

}

std::unique_ptr<IFooFactory> getFooFactory()
{
  return makeFooFactory<nsb::os::current_os>();
}