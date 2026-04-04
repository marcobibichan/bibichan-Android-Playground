/*
 * ex02-dynamic-strings.c
 * Purpose: Demonstrate dynamic string allocation and manipulation
 * Dependencies: stdio.h, stdlib.h, string.h
 * Compile: gcc -o ex02 ex02-dynamic-strings.c
 * Run: ./ex02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("=== Dynamic String Demo ===\n\n");
    
    /* strdup - duplicate a string */
    printf("--- strdup Demo ---\n");
    const char *original = "Hello, Dynamic World!";
    char *duplicate = strdup(original);
    
    if (duplicate == NULL) {
        fprintf(stderr, "strdup failed\n");
        return 1;
    }
    
    printf("Original: %s\n", original);
    printf("Duplicate: %s\n", duplicate);
    printf("Addresses: original=%p, duplicate=%p\n", 
           (void *)original, (void *)duplicate);
    
    /* Modify the duplicate (safe, it's a copy) */
    duplicate[7] = 'D';
    printf("Modified duplicate: %s\n", duplicate);
    free(duplicate);
    
    /* strndup - duplicate with length limit */
    printf("\n--- strndup Demo ---\n");
    const char *long_string = "This is a very long string that we'll truncate";
    char *truncated = strndup(long_string, 10);
    
    if (truncated != NULL) {
        printf("Original: %s\n", long_string);
        printf("Truncated (10 chars): %s\n", truncated);
        printf("Length: %zu\n", strlen(truncated));
        free(truncated);
    }
    
    /* Manual dynamic string allocation */
    printf("\n--- Manual Allocation ---\n");
    
    size_t buffer_size = 100;
    char *dynamic_str = (char *)malloc(buffer_size);
    
    if (dynamic_str == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    
    /* Initialize */
    strncpy(dynamic_str, "Initial content", buffer_size - 1);
    dynamic_str[buffer_size - 1] = '\0';
    printf("Initial: %s (length=%zu, capacity=%zu)\n", 
           dynamic_str, strlen(dynamic_str), buffer_size);
    
    /* Append more content */
    strncat(dynamic_str, " + appended text", 
            buffer_size - strlen(dynamic_str) - 1);
    printf("After append: %s\n", dynamic_str);
    
    /* Resize if needed */
    size_t needed = strlen(dynamic_str) + 100;
    if (needed > buffer_size) {
        char *temp = (char *)realloc(dynamic_str, needed);
        if (temp != NULL) {
            dynamic_str = temp;
            buffer_size = needed;
            printf("Resized to: %zu bytes\n", buffer_size);
        }
    }
    
    free(dynamic_str);
    
    /* Array of strings */
    printf("\n--- String Array ---\n");
    
    const char *words[] = {"apple", "banana", "cherry", "date", NULL};
    int count = 0;
    
    /* Count words */
    for (int i = 0; words[i] != NULL; i++) {
        count++;
    }
    
    printf("Word count: %d\n", count);
    
    /* Duplicate array */
    char **word_copies = (char **)malloc((count + 1) * sizeof(char *));
    if (word_copies != NULL) {
        for (int i = 0; i < count; i++) {
            word_copies[i] = strdup(words[i]);
        }
        word_copies[count] = NULL;
        
        /* Print and free */
        printf("Copies: ");
        for (int i = 0; word_copies[i] != NULL; i++) {
            printf("%s ", word_copies[i]);
            free(word_copies[i]);
        }
        printf("\n");
        free(word_copies);
    }
    
    /* String builder pattern */
    printf("\n--- String Builder Pattern ---\n");
    
    typedef struct {
        char *data;
        size_t length;
        size_t capacity;
    } StringBuilder;
    
    StringBuilder sb;
    sb.capacity = 16;
    sb.data = (char *)malloc(sb.capacity);
    sb.data[0] = '\0';
    sb.length = 0;
    
    const char *parts[] = {"Hello", ", ", "World", "!"};
    for (int i = 0; i < 4; i++) {
        size_t part_len = strlen(parts[i]);
        if (sb.length + part_len + 1 > sb.capacity) {
            sb.capacity *= 2;
            char *temp = (char *)realloc(sb.data, sb.capacity);
            if (temp != NULL) {
                sb.data = temp;
            }
        }
        strcat(sb.data, parts[i]);
        sb.length += part_len;
    }
    
    printf("Built string: %s\n", sb.data);
    printf("Length: %zu, Capacity: %zu\n", sb.length, sb.capacity);
    free(sb.data);
    
    /* Memory efficiency comparison */
    printf("\n--- Memory Efficiency ---\n");
    printf("Static array (100 chars): 100 bytes\n");
    printf("Dynamic string (actual length): %zu bytes + overhead\n", strlen("Hello"));
    printf("strdup overhead: includes allocation metadata\n");
    
    return 0;
}
