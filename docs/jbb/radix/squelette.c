#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

typedef uint32_t ui;

const int BLOCK_SIZE;
const int RADIX;
const int MASK;

void print_array(ui* t, int len){
    if (len > 20) { len = 20; }
    for (int i = 0; i < len; i++){
        printf("%llu\n", (long long unsigned)t[i]);
    }
    printf("\n");
}

bool is_sorted(ui *arr, int len){
    for (int i = 0; i < len - 1; i++){
        if (arr[i] > arr[i + 1]){
            return false;
        }
    }
    return true;
}

ui* rand_array(int len){
    ui* t = malloc(len * sizeof(ui));
    for (int i = 0; i < len; i++){
        t[i] = (ui)rand() * (ui)rand();
    }
    return t;
}



int main(void){

    return 0;
}
