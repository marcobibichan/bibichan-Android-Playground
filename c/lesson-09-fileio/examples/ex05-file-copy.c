/*
 * Purpose: Copy audio file with progress display and verification
 * Dependencies: stdio.h, stdlib.h, string.h, errno.h
 * Compile: gcc -o ex05 ex05-file-copy.c -Wall -Wextra
 * Run: ./ex05 source.wav destination.wav
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Buffer size for file copy (1MB) */
#define COPY_BUFFER_SIZE (1024 * 1024)

/* Progress callback type */
typedef void (*ProgressCallback)(size_t current, size_t total, void* user_data);

/* Copy file with progress reporting */
int copy_file_with_progress(const char* src_path, const char* dst_path,
                            ProgressCallback callback, void* user_data) {
    /* Open source file */
    FILE* src = fopen(src_path, "rb");
    if (src == NULL) {
        fprintf(stderr, "Error opening source: %s\n", strerror(errno));
        return -1;
    }
    
    /* Get source file size */
    if (fseek(src, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking source: %s\n", strerror(errno));
        fclose(src);
        return -1;
    }
    
    long file_size = ftell(src);
    if (file_size < 0) {
        fprintf(stderr, "Error getting file size: %s\n", strerror(errno));
        fclose(src);
        return -1;
    }
    
    rewind(src);
    
    /* Open destination file */
    FILE* dst = fopen(dst_path, "wb");
    if (dst == NULL) {
        fprintf(stderr, "Error opening destination: %s\n", strerror(errno));
        fclose(src);
        return -1;
    }
    
    /* Allocate buffer */
    char* buffer = malloc(COPY_BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating buffer\n");
        fclose(src);
        fclose(dst);
        return -1;
    }
    
    /* Copy data */
    size_t total_copied = 0;
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, COPY_BUFFER_SIZE, src)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dst);
        
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to destination: %s\n", strerror(errno));
            free(buffer);
            fclose(src);
            fclose(dst);
            return -1;
        }
        
        total_copied += bytes_written;
        
        /* Call progress callback */
        if (callback != NULL) {
            callback(total_copied, file_size, user_data);
        }
    }
    
    /* Check for read errors */
    if (ferror(src)) {
        fprintf(stderr, "Error reading source: %s\n", strerror(errno));
        free(buffer);
        fclose(src);
        fclose(dst);
        return -1;
    }
    
    /* Flush and close */
    fflush(dst);
    free(buffer);
    fclose(src);
    fclose(dst);
    
    return 0;
}

/* Progress display callback */
void print_progress(size_t current, size_t total, void* user_data) {
    int percent = (int)((double)current / total * 100);
    static int last_percent = -1;
    
    /* Only update if percent changed */
    if (percent != last_percent) {
        printf("\rCopying: %3d%% [%zu/%zu bytes]", percent, current, total);
        fflush(stdout);
        last_percent = percent;
    }
}

/* Verify two files are identical */
int verify_files(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");
    
    if (f1 == NULL || f2 == NULL) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }
    
    /* Get file sizes */
    fseek(f1, 0, SEEK_END);
    long size1 = ftell(f1);
    fseek(f2, 0, SEEK_END);
    long size2 = ftell(f2);
    
    if (size1 != size2) {
        fclose(f1);
        fclose(f2);
        return -1;  /* Sizes differ */
    }
    
    rewind(f1);
    rewind(f2);
    
    /* Compare content */
    char buf1[4096], buf2[4096];
    size_t bytes1, bytes2;
    
    do {
        bytes1 = fread(buf1, 1, sizeof(buf1), f1);
        bytes2 = fread(buf2, 1, sizeof(buf2), f2);
        
        if (bytes1 != bytes2 || memcmp(buf1, buf2, bytes1) != 0) {
            fclose(f1);
            fclose(f2);
            return -1;  /* Content differs */
        }
    } while (bytes1 > 0);
    
    fclose(f1);
    fclose(f2);
    return 0;  /* Files are identical */
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        fprintf(stderr, "Example: %s audio.wav backup/audio.wav\n", argv[0]);
        return 1;
    }
    
    const char* src_path = argv[1];
    const char* dst_path = argv[2];
    
    printf("Source: %s\n", src_path);
    printf("Destination: %s\n", dst_path);
    printf("\n");
    
    /* Copy file with progress */
    int result = copy_file_with_progress(src_path, dst_path, print_progress, NULL);
    
    if (result == 0) {
        printf("\n\nCopy completed successfully!\n");
        
        /* Verify copy */
        printf("Verifying copy... ");
        if (verify_files(src_path, dst_path) == 0) {
            printf("✓ Files are identical\n");
        } else {
            printf("✗ Verification failed!\n");
            return 1;
        }
    } else {
        printf("\n\nCopy failed!\n");
        return 1;
    }
    
    return 0;
}
