#pragma once

#if !(defined NDEBUG) || !(defined _NDEBUG)
#define CONFIG_MODE_DEBUG
#endif

#if defined _WIN32 || defined WIN32 || defined __WIN32__
#define CONFIG_PLATFORM_WINDOWS
#endif

#if defined CONFIG_MODE_DEBUG
#include <common/ieee754_comparison.h>
#include <common/macro_overload.h>
#include <iomanip>
#include <sstream>

#if defined _MSC_VER
#define debug_break() __debugbreak();
#elif defined __APPLE__
#define debug_break() __builtin_trap();
#elif defined __GNUC__ || defined __MINGW32 || defined __clang__
#include <signal.h>
#define debug_break() raise(SIGSEGV);
#else
#define debug_break() *((int*)0) = 0;
#endif

#ifdef CONFIG_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#define debug_printf(...)                    \
  do {                                       \
    char buf[1024];                          \
    snprintf(buf, sizeof(buf), __VA_ARGS__); \
    fprintf_s(stdout, buf);                  \
    fflush(stdout);                          \
    OutputDebugString(buf);                  \
  } while (0);
#else
#define debug_printf(...)       \
  fprintf(stdout, __VA_ARGS__); \
  fflush(stdout);
#endif

#define print_error(filePath, line, functionName, msgtype, errMsg)          \
  do {                                                                      \
    debug_printf("%s(%i): in function \"%s\":\n%s: %s\n\n", filePath, line, \
                 functionName, msgtype, errMsg);                            \
  } while (0);

template <typename T, typename U, typename Stream>
inline void debug_assert_print_impl(const T& left, const U& right,
                                    const char* left_str, const char* right_str,
                                    const char* op_str, Stream& ss) {
  ss << "expected " << left_str << " " << op_str << " " << right_str
     << ", but got:\n"
     << left_str << ":\n"
     << left << "\n"
     << right_str << ":\n"
     << right << "\n";
}

template <typename T, typename U>
inline std::string debug_assert_print(const T& left, const U& right,
                                      const char* left_str,
                                      const char* right_str,
                                      const char* op_str) {
  std::stringstream ss;
  debug_assert_print_impl(left, right, left_str, right_str, op_str, ss);
  return ss.str();
}

inline std::string debug_assert_print(float left, float right,
                                      const char* left_str,
                                      const char* right_str,
                                      const char* op_str) {
  std::stringstream ss;
  ss << std::fixed << std::setw(10) << std::setprecision(10);
  debug_assert_print_impl(left, right, left_str, right_str, op_str, ss);
  ss << "(The difference is " << ulps_distance(left, right) << " ULPs)\n";
  return ss.str();
}

inline std::string debug_assert_print(double left, double right,
                                      const char* left_str,
                                      const char* right_str,
                                      const char* op_str) {
  std::stringstream ss;
  ss << std::fixed << std::setw(20) << std::setprecision(20);
  debug_assert_print_impl(left, right, left_str, right_str, op_str, ss);
  ss << "(The difference is " << ulps_distance(left, right) << " ULPs)\n";
  return ss.str();
}

#define debug_assert2(cond, msg)                                          \
  if (!(cond)) {                                                          \
    print_error(__FILE__, __LINE__, __func__, "assertion failed", (msg)); \
    debug_break();                                                        \
  }

#define debug_assert1(cond)                                               \
  if (!(cond)) {                                                          \
    print_error(__FILE__, __LINE__, __func__, "assertion failed", #cond); \
    debug_break();                                                        \
  }

#define debug_assert(...) MACRO_OVERLOAD(debug_assert, __VA_ARGS__)

#define expect_with_op(left, right, op)                             \
  if (!(op(left, right))) {                                         \
    const auto msg =                                                \
        debug_assert_print(left, right, #left, #right, op.c_str()); \
    print_error(__FILE__, __LINE__, __func__, "assertion failed",   \
                (msg.c_str()));                                     \
    debug_break();                                                  \
  }

struct op_equal {
  template <typename T, typename U>
  inline bool operator()(const T& left, const U& right) const {
    return left == right;
  }

  template <typename T>
  inline bool operator()(const T& left, const float& right) const {
    return almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const float& left, const T& right) const {
    return almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const T& left, const double& right) const {
    return almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const double& left, const T& right) const {
    return almost_equal(left, right, ulps);
  }

  inline bool operator()(double left, float right) const {
    return almost_equal(left, static_cast<double>(right), ulps);
  }

  inline bool operator()(float left, double right) const {
    return almost_equal(static_cast<double>(left), right, ulps);
  }

  inline bool operator()(float left, float right) const {
    return almost_equal(left, right, ulps);
  }

  inline bool operator()(double left, double right) const {
    return almost_equal(left, right, ulps);
  }

  inline const char* c_str() const {
    return "==";
  }

 private:
  const int ulps = 4;
};

struct op_not_equal {
  template <typename T, typename U>
  inline bool operator()(const T& left, const U& right) const {
    return left != right;
  }

  template <typename T>
  inline bool operator()(const T& left, const float& right) const {
    return !almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const float& left, const T& right) const {
    return !almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const T& left, const double& right) const {
    return !almost_equal(left, right, ulps);
  }

  template <typename T>
  inline bool operator()(const double& left, const T& right) const {
    return !almost_equal(left, right, ulps);
  }

  inline bool operator()(double left, float right) const {
    return !almost_equal(left, static_cast<double>(right), ulps);
  }

  inline bool operator()(float left, double right) const {
    return !almost_equal(static_cast<double>(left), right, ulps);
  }

  inline bool operator()(float left, float right) const {
    return !almost_equal(left, right, ulps);
  }

  inline bool operator()(double left, double right) const {
    return !almost_equal(left, right, ulps);
  }

  inline const char* c_str() const {
    return "!=";
  }

 private:
  const int ulps = 4;
};

struct op_less {
  template <typename T, typename U>
  bool operator()(const T& left, const U& right) {
    return left < right;
  }

  const char* c_str() {
    return "<";
  }
};

struct op_greater {
  template <typename T, typename U>
  bool operator()(const T& left, const U& right) {
    return left > right;
  }

  const char* c_str() {
    return ">";
  }
};

struct op_less_equal {
  template <typename T, typename U>
  bool operator()(const T& left, const U& right) {
    return left <= right;
  }

  const char* c_str() {
    return "<=";
  }
};

struct op_greater_equal {
  template <typename T, typename U>
  bool operator()(const T& left, const U& right) {
    return left >= right;
  }

  const char* c_str() {
    return ">=";
  }
};

#define expect_equal(left, right) expect_with_op(left, right, op_equal())

#define expect_not_equal(left, right) \
  expect_with_op(left, right, op_not_equal())

#define expect_less(left, right) expect_with_op(left, right, op_less())

#define expect_less_equal(left, right) \
  expect_with_op(left, right, op_less_equal())

#define expect_greater(left, right) expect_with_op(left, right, op_greater())

#define expect_greater_equal(left, right) \
  expect_with_op(left, right, op_greater_equal())

#define expect_non_null(ptr)                                       \
  if ((ptr == nullptr) || (!ptr) || (ptr == NULL) || (ptr == 0)) { \
    std::stringstream ss;                                          \
    ss << "expected " << #ptr " to be non-null";                   \
    print_error(__FILE__, __LINE__, __func__, "assertion failed",  \
                (ss.str().c_str()));                               \
    debug_break();                                                 \
  }

#else
#define debug_printf(...)
#define debug_assert(...)
#define expect_equal(...)
#define expect_not_equal(...)
#define expect_less(...)
#define expect_less_equal(...)
#define expect_greater(...)
#define expect_greater_equal(...)
#endif
