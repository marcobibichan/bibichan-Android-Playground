/**
 * ex01-pointer-basic.c
 * Purpose: Demonstrate basic pointer declaration, initialization, and dereferencing
 * Dependencies: stdio.h
 * Compile: gcc -o ex01 ex01-pointer-basic.c
 * Run: ./ex01
 */

#include <stdio.h>
#include <stdint.h>

int main(void) {
    /* Basic pointer declaration and initialization */
    int x = 42;
    int *p = &x;          /* p points to x */
    
    printf("=== Basic Pointer Operations ===\n");
    printf("Value of x: %d\n", x);
    printf("Address of x: %p\n", (void*)&x);
    printf("Value of p (address): %p\n", (void*)p);
    printf("Value pointed by p: %d\n", *p);
    
    /* Dereferencing to modify */
    *p = 100;
    printf("\nAfter *p = 100:\n");
    printf("Value of x: %d\n", x);  /* x is now 100 */
    
    /* Pointer to different types */
    double d = 3.14159;
    double *dp = &d;
    printf("\n=== Pointer to Double ===\n");
    printf("Value of d: %f\n", d);
    printf("Value pointed by dp: %f\n", *dp);
    
    /* Pointer size */
    printf("\n=== Pointer Sizes ===\n");
    printf("Size of int*: %zu bytes\n", sizeof(int*));
    printf("Size of double*: %zu bytes\n", sizeof(double*));
    printf("Size of void*: %zu bytes\n", sizeof(void*));
    printf("Size of int**: %zu bytes\n", sizeof(int**));
    
    /* NULL pointer */
    printf("\n=== NULL Pointer ===\n");
    int *null_p = NULL;
    printf("NULL pointer value: %p\n", (void*)null_p);
    
    if (null_p == NULL) {
        printf("Pointer is NULL, safe to check before use\n");
    }
    
    /* Pointer to pointer */
    printf("\n=== Pointer to Pointer ===\n");
    int y = 200;
    int *py = &y;
    int **ppy = &py;
    
    printf("Value of y: %d\n", y);
    printf("Value pointed by py: %d\n", *py);
    printf("Value pointed by ppy (address): %p\n", (void*)*ppy);
    printf("Value pointed by *ppy: %d\n", **ppy);
    
    return 0;
}
