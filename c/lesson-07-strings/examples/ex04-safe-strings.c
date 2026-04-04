/*
 * ex04-safe-strings.c
 * Purpose: Demonstrate safe string handling practices
 * Dependencies: stdio.h, stdstdlib.h, string.h
 * Compile: gcc -o ex04 ex04-safe-strings.c
 * Run: ./ex04
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ============================================================
 * Safe String Functions
 * ============================================================ */

/* Safe string copy with explicit size checking */
int safe_strcpy(char *dest, size_t dest_size, const char *src) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }
    
    size_t src_len = strlen(src);
    if (src_len >= dest_size) {
        /* Source too long, copy what we can */
        strncpy(dest, src, dest_size - 1);
        dest[dest_size - 1] = '\0';
        return -2;  /* Truncated */
    }
    
    strcpy(dest, src);
    return 0;
}

/* Safe string concatenation */
int safe_strcat(char *dest, size_t dest_size, const char *src) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }
    
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    size_t needed = dest_len + src_len + 1;
    
    if (needed > dest_size) {
        /* Not enough space, append what we can */
        size_t available = dest_size - dest_len - 1;
        strncat(dest, src, available);
        return -2;  /* Truncated */
    }
    
    strcat(dest, src);
    return 0;
}

/* Safe formatted string */
int safe_snprintf(char *dest, size_t dest_size, const char *fmt, ...) {
    if (dest == NULL || dest_size == 0 || fmt == NULL) {
        return -1;
    }
    
    va_list args;
    va_start(args, fmt);
    int result = vsnprintf(dest, dest_size, fmt, args);
    va_end(args);
    
    if (result < 0) {
        return -1;
    }
    
    if ((size_t)result >= dest_size) {
        return -2;  /* Truncated */
    }
    
    return 0;
}

/* Safe string duplicate with error handling */
char *safe_strdup(const char *src, const char *context) {
    if (src == NULL) {
        fprintf(stderr, "Error: %s - NULL pointer\n", 
                context ? context : "safe_strdup");
        return NULL;
    }
    
    char *result = strdup(src);
    if (result == NULL) {
        fprintf(stderr, "Error: %s - allocation failed\n", 
                context ? context : "safe_strdup");
    }
    return result;
}

/* ============================================================
 * String Builder for Safe Concatenation
 * ============================================================ */

typedef struct {
    char *buffer;
    size_t length;
    size_t capacity;
} SafeStringBuilder;

SafeStringBuilder *sb_create_safe(size_t initial_capacity) {
    SafeStringBuilder *sb = malloc(sizeof(SafeStringBuilder));
    if (sb == NULL) return NULL;
    
    sb->buffer = malloc(initial_capacity);
    if (sb->buffer == NULL) {
        free(sb);
        return NULL;
    }
    
    sb->buffer[0] = '\0';
    sb->length = 0;
    sb->capacity = initial_capacity;
    return sb;
}

int sb_append_safe(SafeStringBuilder *sb, const char *str) {
    if (sb == NULL || str == NULL) return -1;
    
    size_t str_len = strlen(str);
    size_t needed = sb->length + str_len + 1;
    
    if (needed > sb->capacity) {
        size_t new_capacity = sb->capacity * 2;
        if (new_capacity < needed) new_capacity = needed;
        
        char *new_buffer = realloc(sb->buffer, new_capacity);
        if (new_buffer == NULL) return -1;
        
        sb->buffer = new_buffer;
        sb->capacity = new_capacity;
    }
    
    memcpy(sb->buffer + sb->length, str, str_len + 1);
    sb->length += str_len;
    return 0;
}

int sb_appendf_safe(SafeStringBuilder *sb, const char *fmt, ...) {
    if (sb == NULL || fmt == NULL) return -1;
    
    /* First, determine required size */
    va_list args;
    va_start(args, fmt);
    int needed = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    
    if (needed < 0) return -1;
    
    /* Ensure capacity */
    size_t total_needed = sb->length + needed + 1;
    if (total_needed > sb->capacity) {
        size_t new_capacity = sb->capacity * 2;
        if (new_capacity < total_needed) new_capacity = total_needed;
        
        char *new_buffer = realloc(sb->buffer, new_capacity);
        if (new_buffer == NULL) return -1;
        
        sb->buffer = new_buffer;
        sb->capacity = new_capacity;
    }
    
    /* Format into buffer */
    va_start(args, fmt);
    vsnprintf(sb->buffer + sb->length, needed + 1, fmt, args);
    va_end(args);
    
    sb->length += needed;
    return 0;
}

char *sb_to_string_safe(SafeStringBuilder *sb) {
    if (sb == NULL) return NULL;
    return sb->buffer;
}

void sb_free_safe(SafeStringBuilder *sb) {
    if (sb != NULL) {
        free(sb->buffer);
        free(sb);
    }
}

/* ============================================================
 * Main Function
 * ============================================================ */

int main(void) {
    printf("=== Safe String Operations Demo ===\n\n");
    
    /* Safe copy */
    printf("--- Safe Copy ---\n");
    char dest[10];
    
    int result = safe_strcpy(dest, sizeof(dest), "Hello");
    printf("Copy 'Hello': %s (result=%d)\n", dest, result);
    
    result = safe_strcpy(dest, sizeof(dest), "This is too long");
    printf("Copy 'This is too long': %s (result=%d, truncated)\n", dest, result);
    
    /* Safe concatenation */
    printf("\n--- Safe Concatenation ---\n");
    char concat[20] = "Hello";
    
    result = safe_strcat(concat, sizeof(concat), ", World!");
    printf("After concat: %s (result=%d)\n", concat, result);
    
    result = safe_strcat(concat, sizeof(concat), " This will be truncated");
    printf("After overflow: %s (result=%d)\n", concat, result);
    
    /* Safe formatted string */
    printf("\n--- Safe Formatted String ---\n");
    char formatted[30];
    
    result = safe_snprintf(formatted, sizeof(formatted), 
                           "Track %d: %s", 1, "Song Title");
    printf("Formatted: %s (result=%d)\n", formatted, result);
    
    result = safe_snprintf(formatted, sizeof(formatted),
                           "Very long format string: %d %s %f", 
                           12345, "text", 3.14159);
    printf("Truncated: %s (result=%d)\n", formatted, result);
    
    /* String builder */
    printf("\n--- String Builder ---\n");
    SafeStringBuilder *sb = sb_create_safe(10);
    
    sb_append_safe(sb, "Hello");
    sb_append_safe(sb, ", ");
    sb_append_safe(sb, "Dynamic");
    sb_append_safe(sb, " ");
    sb_append_safe(sb, "World!");
    
    printf("Built string: %s\n", sb->buffer);
    printf("Length: %zu, Capacity: %zu\n", sb->length, sb->capacity);
    
    /* Formatted append */
    sb_appendf_safe(sb, " (Track %d)", 1);
    printf("After formatted append: %s\n", sb->buffer);
    printf("Length: %zu, Capacity: %zu\n", sb->length, sb->capacity);
    
    sb_free_safe(sb);
    
    /* Error handling demo */
    printf("\n--- Error Handling ---\n");
    
    char *copy = safe_strdup("Test string", "main");
    if (copy != NULL) {
        printf("Duplicated: %s\n", copy);
        free(copy);
    }
    
    /* NULL handling */
    char *null_copy = safe_strdup(NULL, "test_null");
    printf("NULL strdup returned: %p\n", (void *)null_copy);
    
    /* Comparison of safe vs unsafe */
    printf("\n--- Safe vs Unsafe Comparison ---\n");
    char small[5];
    
    printf("Unsafe: strcpy(small, \"This is way too long\")\n");
    printf("  -> Would overflow buffer!\n");
    
    printf("Safe: safe_strcpy(small, sizeof(small), \"This is way too long\")\n");
    result = safe_strcpy(small, sizeof(small), "This is way too long");
    printf("  -> Result: '%s' (truncated safely, result=%d)\n", small, result);
    
    /* Best practices summary */
    printf("\n--- Best Practices Summary ---\n");
    printf("1. Always use bounded functions (strncpy, strncat, snprintf)\n");
    printf("2. Check return values for errors\n");
    printf("3. Ensure null termination after strncpy\n");
    printf("4. Use string builders for complex concatenation\n");
    printf("5. Free dynamically allocated strings\n");
    printf("6. Validate input parameters\n");
    
    printf("\nAll safe operations completed\n");
    return 0;
}
