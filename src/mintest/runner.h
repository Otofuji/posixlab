
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void white() {
    printf("\033[0m");
}

void green() {
    printf("\033[0;32m");
}

void red() {
    printf("\033[0;31m");
}

void ambar() {
    printf("\033[0;33m");
}

void blue() {
    printf("\033[0;34m");
}



int main(int argc, char *argv[])
{

    int size = sizeof(all_tests) / sizeof(test_data), pass_count = 0, child_pass_count = 0, wt = 0;
    pid_t child, wpid;

    if (argc == 1)
    {
        white();
        printf("\n\n=====================\n\n");
        printf("Running %d tests:\n", size);
        printf("\n=====================\n\n");
       

        for (int i = 0; i < size; i++)
        {
            child = fork();
            if (child == 0)
            {
                if (all_tests[i].function() >= 0)
                {
                    green();
                    printf("%s: [PASS]\n", all_tests[i].name);
                    white();
                    child_pass_count = 1;
                }
                break;
            }
        }

        if (child == 0)
        {

            return child_pass_count;
        }

        while ((wpid = wait(&wt)) > 0)
        {
            int pid = wpid - getpid();
            red();
            if (WIFSIGNALED(wt)) printf("test%d: [ERRO] %s\n", pid, strsignal(WTERMSIG(wt)));
            white();
            pass_count += WEXITSTATUS(wt);
        }

        white();
        printf("\n\n=====================\n\n");
        printf("%d/%d tests passed\n", pass_count, size);
        printf("\n=====================\n\n");;
        
    }

    else
    {
        white();
        printf("\n\n=====================\n\n");
        printf("Running %d tests:\n", size);
        printf("\n=====================\n\n");
        

        for (int i = 0; i < size; i++)
        {
            if (strcmp(all_tests[i].name, argv[1]) == 0)
            {
                if (all_tests[i].function() >= 0)
                {
                    green();
                    printf("%s: [PASS]\n", all_tests[i].name);
                    white();
                    pass_count++;
                    
                }
            }
        }

        white();
        printf("\n\n=====================\n\n");
        printf("%d/%d tests passed\n", pass_count, argc - 1);
        printf("\n=====================\n\n");;
        
        return 0;
    }
}
