// File: compilation_phases.c
// Purpose: Demonstrate compilation phases with preprocessor directives
// Compile: gcc -E compilation_phases.c -o compilation_phases.i  (preprocess)
//          gcc -S compilation_phases.i -o compilation_phases.s  (compile)
//          gcc -c compilation_phases.s -o compilation_phases.o  (assemble)
//          gcc compilation_phases.o -o compilation_phases       (link)
// Or all at once: gcc compilation_phases.c -o compilation_phases

#include <stdio.h>   // Standard I/O functions
#define MESSAGE "Hello from macro!"  // Object-like macro
#define SQUARE(x) ((x) * (x))        // Function-like macro

// Conditional compilation
#ifdef DEBUG
#define LOG(msg) printf("[DEBUG] %s\n", msg)
#else
#define LOG(msg) /* no-op */
#endif

int main(void) {
    printf("%s\n", MESSAGE);
    
    int num = 5;
    printf("Square of %d is %d\n", num, SQUARE(num));
    
    LOG("This is a debug message");
    
    return 0;
}
