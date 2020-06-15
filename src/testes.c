//ericfoa tests

#include "mintest/macros.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void terminal() {
    printf("\033[0m");
}


void yellow() {
    printf("\033[0;33m");
}

void cyan() {
    printf("\033[0;34m");
}

int test1() {
    int *vetor = malloc(1);
    yellow();
    test_assert(vetor[5] == 18, "FAULT TEST SEGMENTATION");
    terminal();
    free(vetor);
    return 0;
}

int test2() {
    yellow();
    test_assert(1/0 == 5, "FAULT TEST DIVISION BY ZERO");
    terminal();
    return 0;
}

int test3() {
    int t = 0;
    while(1) {t += 1;}
    yellow();
    printf("FAULT TEST INFINITE LOOP");
    terminal();
    return 0;
}

int test4() {
    int a = 0, b = 0;
    for (int i = 0; i < 5000; i++) {a+=1;}
    for (int i = 0; i < 5000; i++) {b+=1;}
    cyan();
    test_assert(a == b, "PASS TEST LONG TASK");
    terminal();
    return 0;
}

int test5() {
    cyan();
    test_assert(1 == 1, "PASS TEST ASSERT");
    yellow();
    test_assert(0 == 1, "FAULT TEST ASSERT");
    terminal();
    return 0;
}

int test6() {
    for (int i = 0; i < 12; i++) {
        cyan();
        printf("PASS TEST DOZEN PRINT\n");
        terminal();
    }
    return 0;
}

int test7() {
    cyan();
    test_assert (0 == 0, "PASS TEST FAST ASSERT");
    terminal();

    sleep(1);
    cyan();
    test_assert (1 != 0, "PASS TEST SLOW ASSERT");
    terminal();
    return 0;
}

test_list = { TEST(test1), TEST(test2), TEST(test3), TEST(test4), TEST(test5), TEST(test6), TEST(test7)};

#include "mintest/runner.h"
