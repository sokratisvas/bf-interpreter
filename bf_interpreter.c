#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define BUFFER_SIZE 128
#define INPUT_SIZE 256
#define MEM_SIZE 30000

bool is_operation(char c) {
    return (43 <= c && c <= 46) || (c == 60) || (c == 62) || (c == 91) || (c == 93);
}

void print_input(unsigned char* input, int input_len){
    for (int i = 0; i < input_len; i++) {
        if (i == (input_len - 1)) {
            printf("[%u]\n", input[i]);  
        }else {
            printf("[%u]", input[i]);  
        }
    }
}

int matching_brucket(unsigned char* input, int input_len, int start_idx, bool is_open) {
    int cnt = 1;
    int end_idx;
    if (is_open) {
        end_idx = start_idx + 1;
        
        while (cnt > 0) {
            if (input[end_idx] == '[') {
                cnt++;
            }else if (input[end_idx] == ']'){
                cnt--;
            }
            end_idx++;
        }
    }else {
        end_idx = start_idx - 1;

        while (cnt > 0) {
            if (input[end_idx] == '[') {
                cnt--;
            }else if (input[end_idx] == ']'){
                cnt++;
            }
            end_idx--;
        }
    }
    return end_idx;
}

void run_input(unsigned char* input, int input_len, unsigned char* memory) {
    int matching_idx = 0;
    int input_idx = 0;
    int memory_idx = 0;
    
    while (input_idx < input_len) {
        switch (input[input_idx]) {
            case '>':
                memory_idx++;
                assert(memory_idx < MEM_SIZE && "error: out of range");
                break;
            case '<':
                memory_idx--;
                assert(memory_idx >= 0 && "error: out of range");
                break;
            case '+':
                memory[memory_idx] = (memory[memory_idx] + 1) % (UCHAR_MAX + 1);
                break;
            case '-':
                memory[memory_idx] = (memory[memory_idx] + UCHAR_MAX) % (UCHAR_MAX + 1);
                break;
            case ',':
                memory[memory_idx] = getchar();
                break;
            case '.':
                putchar(memory[memory_idx]);
                break;
            case '[':
                matching_idx = matching_brucket(input, input_len, input_idx, true);
                if (!memory[memory_idx]) {
                    input_idx = matching_idx - 1;
                }
                break;
            case ']':
                matching_idx = matching_brucket(input, input_len, input_idx, false);
                if (memory[memory_idx]) {
                    input_idx = matching_idx + 1;
                }
                break;
        }
        input_idx++;
    }
}

int main() {
    unsigned char* memory = malloc(MEM_SIZE * sizeof(char));
    for (int i = 0; i < MEM_SIZE; i++) {
        memory[i] = 0;
    }

    unsigned char* input = malloc(INPUT_SIZE * sizeof(char));
    int input_len = 0;
    
    char path[BUFFER_SIZE];
    printf("enter .bf file path > ");
    scanf("%s", path);

    int idx = strlen(path) - 1;
    assert(path[idx] == 'f' && "error: file must end in .bf");
    while (path[idx--] != '/') {}
    idx++;
    int bf_len = strlen(path) - idx;
    
    char bf_file[bf_len];
    for (int i = 0; i < bf_len - 1; i++) {
        bf_file[i] = path[i + idx + 1];
    }
    bf_file[bf_len - 1] = '\0';

    printf("running \"%s\" ...\n", bf_file);
    
    // Clear Buffer
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }

    FILE* file = fopen(path, "r");
    assert(file != NULL && "error: file doesn't exist");
    char c;
    while ((c = fgetc(file)) != EOF && input_len <= INPUT_SIZE - 1) {
        if (is_operation(c)) {
            input[input_len++] = c;
        }
    }
    fclose(file);

    /* Read from input
    while ((input[input_len++] = getchar()) != '\n' && input_len <= INPUT_SIZE) {}
    input_len--;
    */

    run_input(input, input_len, memory);
    free(memory);
    free(input);
    return 0;
}
