/*
 * ex03-string-parsing.c
 * Purpose: Demonstrate string parsing techniques
 * Dependencies: stdio.h, stdlib.h, string.h, ctype.h
 * Compile: gcc -o ex03 ex03-string-parsing.c
 * Run: ./ex03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Parse key=value pair */
int parse_kv_pair(const char *input, char *key, char *value, size_t max_len) {
    /* Find the '=' delimiter */
    const char *eq = strchr(input, '=');
    if (eq == NULL) {
        return -1;  /* No '=' found */
    }
    
    /* Extract key */
    size_t key_len = eq - input;
    if (key_len >= max_len) {
        key_len = max_len - 1;
    }
    strncpy(key, input, key_len);
    key[key_len] = '\0';
    
    /* Extract value */
    const char *val_start = eq + 1;
    strncpy(value, val_start, max_len - 1);
    value[max_len - 1] = '\0';
    
    return 0;
}

/* Trim whitespace from both ends */
char *trim(char *str) {
    /* Trim leading space */
    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    if (*str == '\0') {
        return str;
    }
    
    /* Trim trailing space */
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
    
    return str;
}

/* Split string into tokens (non-destructive) */
int split_string(const char *input, const char *delim, 
                 char tokens[][100], int max_tokens) {
    /* Make a copy since strtok modifies the string */
    char *copy = strdup(input);
    if (copy == NULL) {
        return -1;
    }
    
    int count = 0;
    char *token = strtok(copy, delim);
    
    while (token != NULL && count < max_tokens) {
        strncpy(tokens[count], token, 99);
        tokens[count][99] = '\0';
        count++;
        token = strtok(NULL, delim);
    }
    
    free(copy);
    return count;
}

/* Parse audio metadata line */
typedef struct {
    char artist[100];
    char album[100];
    char title[100];
    int year;
    int track;
} AudioMetadata;

int parse_metadata_line(const char *line, AudioMetadata *meta) {
    /* Expected format: artist|album|title|year|track */
    char tokens[5][100];
    int count = split_string(line, "|", tokens, 5);
    
    if (count < 5) {
        return -1;
    }
    
    strncpy(meta->artist, trim(tokens[0]), sizeof(meta->artist) - 1);
    strncpy(meta->album, trim(tokens[1]), sizeof(meta->album) - 1);
    strncpy(meta->title, trim(tokens[2]), sizeof(meta->title) - 1);
    meta->year = atoi(tokens[3]);
    meta->track = atoi(tokens[4]);
    
    return 0;
}

int main(void) {
    printf("=== String Parsing Demo ===\n\n");
    
    /* Key-Value parsing */
    printf("--- Key-Value Parsing ---\n");
    const char *config_lines[] = {
        "sample_rate=192000",
        "channels=2",
        "bit_depth=24",
        "format=FLAC"
    };
    
    for (int i = 0; i < 4; i++) {
        char key[50], value[50];
        if (parse_kv_pair(config_lines[i], key, value, sizeof(key)) == 0) {
            printf("Key: '%s', Value: '%s'\n", key, value);
        }
    }
    
    /* String trimming */
    printf("\n--- String Trimming ---\n");
    char strings[][50] = {
        "   hello world   ",
        "\t\ttabbed\t\t",
        "no-spaces",
        "   "
    };
    
    for (int i = 0; i < 4; i++) {
        char copy[50];
        strcpy(copy, strings[i]);
        char *trimmed = trim(copy);
        printf("Original: '%s' -> Trimmed: '%s'\n", strings[i], trimmed);
    }
    
    /* Token splitting */
    printf("\n--- Token Splitting ---\n");
    const char *csv = "apple,banana,cherry,date,elderberry";
    char tokens[10][100];
    int count = split_string(csv, ",", tokens, 10);
    
    printf("CSV: %s\n", csv);
    printf("Tokens (%d): ", count);
    for (int i = 0; i < count; i++) {
        printf("[%s] ", tokens[i]);
    }
    printf("\n");
    
    /* Audio metadata parsing */
    printf("\n--- Audio Metadata Parsing ---\n");
    const char *metadata_line = "Pink Floyd|The Dark Side of the Moon|Time|1973|4";
    
    AudioMetadata meta;
    if (parse_metadata_line(metadata_line, &meta) == 0) {
        printf("Artist: %s\n", meta.artist);
        printf("Album:  %s\n", meta.album);
        printf("Title:  %s\n", meta.title);
        printf("Year:   %d\n", meta.year);
        printf("Track:  %d\n", meta.track);
    }
    
    /* Path parsing */
    printf("\n--- Path Parsing ---\n");
    const char *filepath = "/music/artist/album/track.flac";
    
    /* Find filename */
    const char *filename = strrchr(filepath, '/');
    if (filename != NULL) {
        filename++;  /* Skip the '/' */
        printf("Filename: %s\n", filename);
    }
    
    /* Find extension */
    const char *ext = strrchr(filename, '.');
    if (ext != NULL) {
        printf("Extension: %s\n", ext + 1);
    }
    
    /* Directory path */
    char dir_path[256];
    size_t dir_len = filename - filepath;
    strncpy(dir_path, filepath, dir_len);
    dir_path[dir_len] = '\0';
    printf("Directory: %s\n", dir_path);
    
    /* URL parsing */
    printf("\n--- URL Parsing ---\n");
    const char *url = "https://example.com:8080/music/track.flac?quality=hi-res";
    
    /* Parse protocol */
    const char *proto_end = strstr(url, "://");
    if (proto_end != NULL) {
        size_t proto_len = proto_end - url;
        char protocol[16];
        strncpy(protocol, url, proto_len);
        protocol[proto_len] = '\0';
        printf("Protocol: %s\n", protocol);
    }
    
    /* Parse host */
    const char *host_start = proto_end ? proto_end + 3 : url;
    const char *path_start = strchr(host_start, '/');
    const char *port_start = strchr(host_start, ':');
    
    char host[128];
    if (port_start != NULL && (path_start == NULL || port_start < path_start)) {
        strncpy(host, host_start, port_start - host_start);
        host[port_start - host_start] = '\0';
        printf("Host: %s\n", host);
        printf("Port: %s\n", port_start + 1);
    } else if (path_start != NULL) {
        strncpy(host, host_start, path_start - host_start);
        host[path_start - host_start] = '\0';
        printf("Host: %s\n", host);
    }
    
    /* Parse query string */
    const char *query = strchr(url, '?');
    if (query != NULL) {
        printf("Query: %s\n", query + 1);
        
        /* Parse query parameters */
        char query_copy[256];
        strcpy(query_copy, query + 1);
        char *param = strtok(query_copy, "&");
        while (param != NULL) {
            char key[64], value[64];
            if (parse_kv_pair(param, key, value, sizeof(key)) == 0) {
                printf("  Param: %s = %s\n", key, value);
            }
            param = strtok(NULL, "&");
        }
    }
    
    return 0;
}
