
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
 
    int size = sizeof(all_tests)/sizeof(test_data);
    int pass_count = 0;

    if (argc == 1) {
        
        printf("Running %d tests:\n", size);
        printf("=====================\n\n");
    
        for (int i = 0; i < size; i++) {
            if (all_tests[i].function() >= 0) {
                printf("%s: [PASS]\n", all_tests[i].name);
                pass_count++;
            };
        }

        printf("\n\n=====================\n");
        printf("%d/%d tests passed\n", pass_count, size);
        return 0;
    }

    else {
        printf("Running %d tests:\n", argc-1);
        printf("=====================\n\n");

        for (int i = 0; i < argc; i++) {
            
            if (all_tests[i].function() >= 0) {
                if (strcmp(all_tests[i].name, argv[i+1]) == 0) {
                    printf("%s: [PASS]\n", all_tests[i].name);
                    pass_count++;
                
                }
            }
        }
        
        printf("\n\n=====================\n");
        printf("%d/%d tests passed\n", pass_count, argc-1);
        return 0;
    }
    
    
    
}
