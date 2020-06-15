#include <stdio.h>


void fire() {
    printf("\033[0;31m");
}

void end_of_error() {
    printf("\033[0m");
}

void lime() {
    printf("\033[0;32m");
}

typedef int (*test_func)();

typedef struct {
    char name[50];
    test_func function;
} test_data;

#define test_printf (printf("%s: ", __func__));printf

#define test_assert(expr, str) { if(!(expr)) { fire(); printf("%s: [FAIL] %s in %s:%d\n", __func__, str, __FILE__, __LINE__); end_of_error(); return -1; } else {lime(); printf("%s: [PASS] %s\n", __func__, str); end_of_error();} }

#define TEST(f) {.name=#f, .function=f}

#define test_list test_data all_tests[]

