#include <gtest/gtest.h>

#include "predef.h"

TEST(PredefTest, Os)
{
  constexpr bool isWindows = 
#ifdef NSB_OS_WINDOWS_AVAILABLE
  true;
#else
  false;
#endif

  constexpr bool isLinux = 
#ifdef NSB_OS_LINUX_AVAILABLE
  true;
#else
  false;
#endif

  constexpr bool isMacOs= 
#ifdef NSB_OS_MACOS_AVAILABLE
  true;
#else
  false;
#endif

  constexpr bool isUnknown = 
#ifdef NSB_OS_UNKNOWN_AVAILABLE
  true;
#else
  false;
#endif

  using namespace nsb;
  constexpr os currentOs = isWindows ? os::windows_os :
                           isLinux ? os::linux_os : 
                           isMacOs ? os::mac_os :
                           os::unknown_os;

  constexpr int num = os_available::windows_os +
                      os_available::linux_os +
                      os_available::mac_os +
                      os_available::unknown_os;

  constexpr bool isOsAvailable = NSB_OS_WINDOWS ? os_available::windows_os :
                                 NSB_OS_LINUX ? os_available::linux_os :
                                 NSB_OS_MACOS ? os_available::mac_os :
                                 os_available::unknown_os;

  ASSERT_EQ(isWindows, static_cast<bool>(NSB_OS_WINDOWS));
  ASSERT_EQ(isLinux, static_cast<bool>(NSB_OS_LINUX));
  ASSERT_EQ(isMacOs, static_cast<bool>(NSB_OS_MACOS));
  ASSERT_EQ(isUnknown, static_cast<bool>(NSB_OS_UNKNOWN));
  ASSERT_EQ(currentOs, os::current_os);
  ASSERT_EQ(num, 1);
  ASSERT_TRUE(isOsAvailable);
}
