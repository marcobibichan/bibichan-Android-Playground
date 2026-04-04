/**
 * ex03-scope-lifetime.c
 * Purpose: Demonstrate variable scope and lifetime
 * Dependencies: stdio.h
 * Compile: gcc -o ex03 ex03-scope-lifetime.c
 * Run: ./ex03
 */

#include <stdio.h>

/* Global variable - external linkage */
int global_counter = 0;

/* File-scope variable - internal linkage */
static int file_scope_counter = 0;

/* Function with local variable */
void count_calls_local(void) {
    int call_count = 0;  /* Local - reinitialized each call */
    call_count++;
    global_counter++;
    printf("Local call_count: %d, global_counter: %d\n", 
           call_count, global_counter);
}

/* Function with static local variable */
void count_calls_static(void) {
    static int call_count = 0;  /* Static - persists between calls */
    call_count++;
    file_scope_counter++;
    printf("Static call_count: %d, file_scope_counter: %d\n", 
           call_count, file_scope_counter);
}

/* Demonstrate block scope */
void demonstrate_block_scope(void) {
    int x = 10;
    printf("Outer block: x = %d\n", x);
    
    {  /* New block scope */
        int x = 20;  /* Shadows outer x */
        printf("Inner block: x = %d\n", x);
    }
    
    printf("Outer block again: x = %d\n", x);
}

/* Return pointer to static variable (safe) */
int* get_static_pointer(void) {
    static int persistent_value = 100;
    persistent_value++;
    return &persistent_value;
}

/* DANGEROUS - Return pointer to local variable (commented out) */
int* dangerous_get_local_pointer(void) {
    int local = 42;
    /* return &local; */  /* NEVER do this! Undefined behavior */
    return NULL;
}

int main(void) {
    printf("=== Local vs Static Variables ===\n");
    
    /* Call multiple times to see difference */
    for (int i = 0; i < 3; i++) {
        count_calls_local();
    }
    printf("\n");
    
    for (int i = 0; i < 3; i++) {
        count_calls_static();
    }
    printf("\n");
    
    printf("=== Block Scope ===\n");
    demonstrate_block_scope();
    printf("\n");
    
    printf("=== Static Variable Pointer ===\n");
    int *ptr = get_static_pointer();
    printf("Value: %d\n", *ptr);
    *ptr = 200;
    int *ptr2 = get_static_pointer();
    printf("Value after modification: %d\n", *ptr2);
    
    return 0;
}
