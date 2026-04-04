/**
 * ex01-function-basic.c
 * Purpose: Demonstrate basic function declaration, definition, and calling
 * Dependencies: stdio.h
 * Compile: gcc -o ex01 ex01-function-basic.c
 * Run: ./ex01
 */

#include <stdio.h>

/* Function declaration (prototype) */
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

/* Function that returns nothing (void) */
void print_result(const char *operation, int result) {
    printf("%s: %d\n", operation, result);
}

/* Function with no parameters */
int get_default_value(void) {
    return 42;
}

/* Main function - entry point */
int main(void) {
    int x = 10, y = 3;
    
    /* Call functions and store results */
    int sum = add(x, y);
    int diff = subtract(x, y);
    int product = multiply(x, y);
    int quotient = divide(x, y);
    
    /* Print results using void function */
    print_result("Sum", sum);
    print_result("Difference", diff);
    print_result("Product", product);
    print_result("Quotient", quotient);
    
    /* Call function with no parameters */
    printf("Default value: %d\n", get_default_value());
    
    return 0;
}

/* Function definitions */
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    return a / b;
}
