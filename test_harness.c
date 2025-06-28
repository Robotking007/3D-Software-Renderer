#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>

#define TEST_CASE(name) void name(void)
#define ASSERT(cond) \
    do { \
        if (!(cond)) { \
            printf("FAIL: %s (Line %d)\n", #cond, __LINE__); \
            test_failures++; \
            return; \
        } \
    } while (0)

#define RUN_TEST(test) \
    do { \
        printf("Running %-30s", #test); \
        test_count++; \
        test(); \
        printf("[PASS]\n"); \
    } while (0)

#define TEST_SUMMARY() \
    printf("\n%d tests, %d failures\n", test_count, test_failures)

// Global test counters
static int test_count = 0;
static int test_failures = 0;

#endif // TEST_HARNESS_H