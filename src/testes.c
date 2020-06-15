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
    test_assert(vetor[5] == 18, "SEGMENTATION FAULT TEST");
    terminal();
    free(vetor);
    return 0;
}

int test2() {
    yellow();
    test_assert(1/0 == 5, "DIVISION BY ZERO FAULT TEST");
    terminal();
    return 0;
}

int test3() {
    int t = 0;
    alarm(2);
    while(1) {t += 1;}
    yellow();
    printf("INIFINTE LOOP FAULT TEST");
    terminal();
    return 0;
}

int test4() {
    int a = 0, b = 0;
    for (int i = 0; i < 5000; i++) {a+=1;}
    for (int i = 0; i < 5000; i++) {b+=1;}
    cyan();
    test_assert(a == b, "LONG TASK PASS TEST");
    terminal();
    return 0;
}

int test5() {
    cyan();
    test_assert(1 == 1, "ASSERT PASS TEST");
    yellow();
    test_assert(0 == 1, "ASSERT FAULT TEST");
    terminal();
    return 0;
}

int test6() {
    for (int i = 0; i < 12; i++) {
        cyan();
        printf("DOZEN PRINT PASS TEST\n");
        terminal();
    }
    return 0;
}

int test7() {
    cyan();
    test_assert (0 == 0, "FAST ASSERT PASS TEST");
    terminal();
    sleep(2);
    cyan();
    test_assert (1 != 0, "SLOW ASSERT PASS TEST");
    terminal();
    return 0;
}

test_list = { TEST(test1), TEST(test2), TEST(test3), TEST(test4), TEST(test5), TEST(test6), TEST(test7)};

#include "mintest/runner.h"
