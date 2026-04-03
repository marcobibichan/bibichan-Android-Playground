// File: preprocessor_demo.c
// Purpose: Advanced preprocessor features demonstration
// Compile: gcc -E preprocessor_demo.c -o preprocessor_demo.i
//          gcc preprocessor_demo.c -o preprocessor_demo

#include <stdio.h>
#include <string.h>

// Stringizing operator (#)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Token pasting operator (##)
#define CONCAT(a, b) a ## b
#define MAKE_FUNC(name) void func_ ## name() { printf(#name "\n"); }

// Variadic macro
#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", \
            __FILE__, __LINE__, ##__VA_ARGS__)

// Conditional compilation with value checking
#define VERSION 2
#if VERSION >= 2
#define FEATURE_NEW_API 1
#else
#define FEATURE_NEW_API 0
#endif

// Generate functions using macros
MAKE_FUNC(alpha)
MAKE_FUNC(beta)

int main(void) {
    // Stringizing demonstration
    printf("Stringify test: %s\n", STRINGIFY(hello));
    printf("Value to string: %s\n", TOSTRING(VERSION));
    
    // Token pasting demonstration
    int CONCAT(my, var) = 42;  // Creates: int myvar = 42;
    printf("myvar = %d\n", myvar);
    
    // Generated functions
    func_alpha();
    func_beta();
    
    // Debug print
    DEBUG_PRINT("Starting program");
    DEBUG_PRINT("Value: %d", 100);
    
    // Conditional feature
    #if FEATURE_NEW_API
    printf("Using new API\n");
    #else
    printf("Using old API\n");
    #endif
    
    return 0;
}
