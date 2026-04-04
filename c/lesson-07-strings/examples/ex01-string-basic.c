/*
 * ex01-string-basic.c
 * Purpose: Demonstrate basic string operations
 * Dependencies: stdio.h, string.h
 * Compile: gcc -o ex01 ex01-string-basic.c
 * Run: ./ex01
 */

#include <stdio.h>
#include <string.h>

int main(void) {
    printf("=== Basic String Operations ===\n\n");
    
    /* String literal declaration */
    const char *greeting = "Hello, World!";
    
    /* Get string length */
    size_t len = strlen(greeting);
    printf("String: \"%s\"\n", greeting);
    printf("Length: %zu characters\n", len);
    printf("Size in memory: %zu bytes (including null terminator)\n", len + 1);
    
    /* Character array (modifiable) */
    char modifiable[] = "Hello, World!";
    modifiable[0] = 'h';  /* Safe: this is a copy, not the literal */
    printf("\nModified: %s\n", modifiable);
    
    /* String comparison */
    const char *str1 = "apple";
    const char *str2 = "banana";
    
    int cmp_result = strcmp(str1, str2);
    if (cmp_result < 0) {
        printf("\"%s\" comes before \"%s\"\n", str1, str2);
    } else if (cmp_result > 0) {
        printf("\"%s\" comes after \"%s\"\n", str1, str2);
    } else {
        printf("Strings are equal\n");
    }
    
    /* String copy (safe version) */
    printf("\n--- String Copy ---\n");
    char dest[20];
    strncpy(dest, greeting, sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';  /* Ensure null termination */
    printf("Copied: %s\n", dest);
    
    /* String concatenation */
    printf("\n--- String Concatenation ---\n");
    char concat[50] = "Hello";
    strncat(concat, ", ", sizeof(concat) - strlen(concat) - 1);
    strncat(concat, "World!", sizeof(concat) - strlen(concat) - 1);
    printf("Concatenated: %s\n", concat);
    
    /* Character and substring search */
    printf("\n--- Search Operations ---\n");
    const char *text = "The quick brown fox jumps over the lazy dog";
    
    char *first_o = strchr(text, 'o');
    if (first_o != NULL) {
        printf("First 'o' at position: %ld\n", first_o - text);
    }
    
    char *last_o = strrchr(text, 'o');
    if (last_o != NULL) {
        printf("Last 'o' at position: %ld\n", last_o - text);
    }
    
    char *fox = strstr(text, "fox");
    if (fox != NULL) {
        printf("Found \"fox\" at position: %ld\n", fox - text);
    }
    
    /* String tokenization */
    printf("\n--- Tokenization ---\n");
    char sentence[] = "apple,banana,cherry,date";
    char *token = strtok(sentence, ",");
    printf("Tokens: ");
    while (token != NULL) {
        printf("[%s] ", token);
        token = strtok(NULL, ",");
    }
    printf("\n");
    
    /* String case comparison */
    printf("\n--- Case Sensitivity ---\n");
    const char *upper = "HELLO";
    const char *lower = "hello";
    printf("strcmp(\"%s\", \"%s\") = %d\n", upper, lower, strcmp(upper, lower));
    printf("strncmp(\"%s\", \"%s\", 5) = %d\n", upper, lower, strncmp(upper, lower, 5));
    
    /* String length comparison */
    printf("\n--- Length Comparison ---\n");
    const char *short_str = "Hi";
    const char *long_str = "Hello, World!";
    printf("Short string length: %zu\n", strlen(short_str));
    printf("Long string length: %zu\n", strlen(long_str));
    
    /* Memory layout demonstration */
    printf("\n--- Memory Layout ---\n");
    char buffer[] = "ABCDE";
    printf("String: %s\n", buffer);
    printf("Bytes: ");
    for (int i = 0; i <= strlen(buffer); i++) {
        printf("'%c'(%d) ", buffer[i] ? buffer[i] : '\\0', (unsigned char)buffer[i]);
    }
    printf("\n");
    
    return 0;
}
