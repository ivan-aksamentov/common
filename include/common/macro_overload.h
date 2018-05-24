#pragma once

#define COUNT_ASSERT_ARGS_IMPL2(_1, _2, count, ...) count
#define COUNT_ASSERT_ARGS_IMPL(args) COUNT_ASSERT_ARGS_IMPL2 args
#define COUNT_ASSERT_ARGS(...) COUNT_ASSERT_ARGS_IMPL((__VA_ARGS__, 2, 1, 0))
#define ASSERT_CHOOSE_HELPER2(name, count) name##count
#define ASSERT_CHOOSE_HELPER1(name, count) ASSERT_CHOOSE_HELPER2(name, count)
#define ASSERT_CHOOSE_HELPER(name, count) ASSERT_CHOOSE_HELPER1(name, count)
#define ASSERT_GLUE(x, y) x y
#define MACRO_OVERLOAD(name, ...)                                           \
    ASSERT_GLUE(                                                            \
        ASSERT_CHOOSE_HELPER(debug_assert, COUNT_ASSERT_ARGS(__VA_ARGS__)), \
        (__VA_ARGS__))

