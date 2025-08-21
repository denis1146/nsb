#pragma once

#undef NSB_OS_WINDOWS
#undef NSB_OS_WINDOWS_AVAILABLE
#ifdef _WIN32
#  define NSB_OS_WINDOWS 1
#  define NSB_OS_WINDOWS_AVAILABLE
#else
#  define NSB_OS_WINDOWS 0
#endif

#undef NSB_OS_LINUX
#undef NSB_OS_LINUX_AVAILABLE
#ifdef __linux__
#  define NSB_OS_LINUX 1
#  define NSB_OS_LINUX_AVAILABLE
#else
#  define NSB_OS_LINUX 0
#endif

#undef NSB_OS_MACOS
#undef NSB_OS_MACOS_AVAILABLE
#ifdef __APPLE__
#  define NSB_OS_MACOS 1
#  define NSB_OS_MACOS_AVAILABLE
#else
#  define NSB_OS_MACOS 0
#endif

#undef NSB_OS_UNKNOWN
#undef NSB_OS_UNKNOWN_AVAILABLE
#if NSB_OS_WINDOWS || NSB_OS_LINUX || NSB_OS_MACOS
#   define NSB_OS_UNKNOWN 0
#else
#  define NSB_OS_UNKNOWN 1
#  define NSB_OS_UNKNOWN_AVAILABLE
#endif

namespace nsb {

enum class os {
  windows_os,
  linux_os,
  mac_os,
  unknown_os,
  current_os = 
#if NSB_OS_WINDOWS
  windows_os,
#elif NSB_OS_LINUX
  linux_os,
#elif NSB_OS_MACOS
  mac_os,
#elif NSB_OS_UNKNOWN
  unknown_os,
#endif 
};

struct os_available {
  static constexpr bool windows_os{ os::current_os == os::windows_os };
  static constexpr bool linux_os{ os::current_os == os::linux_os };
  static constexpr bool mac_os{ os::current_os == os::mac_os };
  static constexpr bool unknown_os{ os::current_os == os::unknown_os };
};

} // ~nsb
