#pragma once

#define BEETRAP_TESTING_FRAMEWORK

#ifndef BEETRAP_TEST_FRAMEWORK_COUNTERS
#define BEETRAP_TEST_FRAMEWORK_COUNTERS

static int total_Tests = 0;
static int failed_Tests = 0;

#endif

#define TEST_ASSERT(expression)                                    \
{                                                                  \
    ++total_Tests;                                                 \
    if(!(expression))                                              \
    {                                                              \
        ++failed_Tests;                                            \
        PRINT_CONSOLE("Func : %s %s\n", FUNC_NAME, FILE_AND_LINE); \
    }                                                              \
}                                                                  \

#define IS_TRUE(A)                          \
{                                           \
    bool expression = (A);                  \
    if(!(expression))                       \
    {                                       \
        PRINT_CONSOLE("IS_TRUE Failed - "); \
    }                                       \
    TEST_ASSERT(expression);                \
}                                           \

#define IS_FALSE(A)                          \
{                                            \
    bool expression = (!A);                   \
    if(!(expression))                        \
    {                                        \
        PRINT_CONSOLE("IS_FALSE Failed - "); \
    }                                        \
    TEST_ASSERT(expression);                 \
}                                            \

#define ARE_EQUAL(A, B)                       \
{                                             \
    bool expression = ((A) == (B));           \
    if(!(expression))                         \
    {                                         \
        PRINT_CONSOLE("ARE_EQUAL Failed - "); \
    }                                         \
    TEST_ASSERT(expression);                  \
}                                             \

#define ARE_NOT_EQUAL(A, B)                   \
{                                             \
    bool expression = ((A) != (B));           \
    if(!(expression))                         \
    {                                         \
        PRINT_CONSOLE("ARE_EQUAL Failed - "); \
    }                                         \
    TEST_ASSERT(expression);                  \
}                                             \

