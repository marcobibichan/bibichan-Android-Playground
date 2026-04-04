/*
 * Purpose: Read and write M3U playlist files
 * Dependencies: stdio.h, stdlib.h, string.h
 * Compile: gcc -o ex04 ex04-playlist-file.c -Wall -Wextra
 * Run: ./ex04
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Maximum line length for playlist entries */
#define MAX_LINE_LENGTH 1024
#define MAX_PATH_LENGTH 512
#define MAX_TITLE_LENGTH 256

/* Playlist entry structure */
typedef struct PlaylistEntry {
    char path[MAX_PATH_LENGTH];
    char title[MAX_TITLE_LENGTH];
    int duration_ms;
    struct PlaylistEntry* next;
} PlaylistEntry;

/* Playlist structure */
typedef struct {
    PlaylistEntry* head;
    PlaylistEntry* tail;
    int count;
} Playlist;

/* Initialize empty playlist */
void playlist_init(Playlist* pl) {
    pl->head = NULL;
    pl->tail = NULL;
    pl->count = 0;
}

/* Add entry to playlist */
int playlist_add(Playlist* pl, const char* path, const char* title, int duration) {
    PlaylistEntry* entry = malloc(sizeof(PlaylistEntry));
    if (entry == NULL) {
        return -1;
    }
    
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    entry->path[MAX_PATH_LENGTH - 1] = '\0';
    
    strncpy(entry->title, title, MAX_TITLE_LENGTH - 1);
    entry->title[MAX_TITLE_LENGTH - 1] = '\0';
    
    entry->duration_ms = duration;
    entry->next = NULL;
    
    if (pl->tail == NULL) {
        pl->head = entry;
        pl->tail = entry;
    } else {
        pl->tail->next = entry;
        pl->tail = entry;
    }
    
    pl->count++;
    return 0;
}

/* Free all entries */
void playlist_free(Playlist* pl) {
    PlaylistEntry* current = pl->head;
    while (current != NULL) {
        PlaylistEntry* next = current->next;
        free(current);
        current = next;
    }
    pl->head = NULL;
    pl->tail = NULL;
    pl->count = 0;
}

/* Parse EXTINF line: #EXTINF:duration,title */
int parse_extinf(const char* line, int* duration, char* title) {
    if (strncmp(line, "#EXTINF:", 8) != 0) {
        return -1;
    }
    
    const char* duration_start = line + 8;
    char* comma = strchr(duration_start, ',');
    if (comma == NULL) {
        return -1;
    }
    
    /* Parse duration */
    *duration = atoi(duration_start);
    
    /* Copy title (skip comma) */
    const char* title_start = comma + 1;
    while (*title_start == ' ') title_start++;  /* Skip leading spaces */
    
    strncpy(title, title_start, MAX_TITLE_LENGTH - 1);
    title[MAX_TITLE_LENGTH - 1] = '\0';
    
    /* Remove trailing newline if present */
    size_t len = strlen(title);
    if (len > 0 && title[len - 1] == '\n') {
        title[len - 1] = '\0';
    }
    
    return 0;
}

/* Read M3U playlist file */
int read_m3u_playlist(const char* filename, Playlist* pl) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open playlist file");
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int last_duration = -1;
    char last_title[MAX_TITLE_LENGTH] = "";
    
    while (fgets(line, sizeof(line), file) != NULL) {
        /* Remove trailing newline */
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        
        /* Skip empty lines */
        if (len == 0) {
            continue;
        }
        
        /* Check for M3U header */
        if (strcmp(line, "#EXTM3U") == 0) {
            continue;
        }
        
        /* Check for EXTINF (metadata) */
        if (strncmp(line, "#EXTINF:", 8) == 0) {
            parse_extinf(line, &last_duration, last_title);
            continue;
        }
        
        /* Skip other comments */
        if (line[0] == '#') {
            continue;
        }
        
        /* This is a file path */
        playlist_add(pl, line, last_title, last_duration);
        
        /* Reset for next entry */
        last_duration = -1;
        last_title[0] = '\0';
    }
    
    if (ferror(file)) {
        perror("Error reading playlist");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}

/* Write M3U playlist file */
int write_m3u_playlist(const char* filename, const Playlist* pl) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create playlist file");
        return -1;
    }
    
    /* Write M3U header */
    fprintf(file, "#EXTM3U\n");
    
    /* Write each entry */
    PlaylistEntry* current = pl->head;
    while (current != NULL) {
        /* Write EXTINF if we have metadata */
        if (current->duration_ms > 0 || current->title[0] != '\0') {
            fprintf(file, "#EXTINF:%d,%s\n", 
                    current->duration_ms / 1000, 
                    current->title);
        }
        
        /* Write file path */
        fprintf(file, "%s\n", current->path);
        
        current = current->next;
    }
    
    fclose(file);
    return 0;
}

/* Print playlist contents */
void playlist_print(const Playlist* pl) {
    printf("Playlist (%d entries):\n", pl->count);
    
    PlaylistEntry* current = pl->head;
    int index = 1;
    
    while (current != NULL) {
        printf("  %d. %s\n", index++, current->title[0] ? current->title : current->path);
        if (current->duration_ms > 0) {
            printf("     Duration: %d:%02d\n", 
                   current->duration_ms / 60000,
                   (current->duration_ms / 1000) % 60);
        }
        printf("     Path: %s\n", current->path);
        current = current->next;
    }
}

int main(void) {
    /* Create a sample playlist */
    Playlist playlist;
    playlist_init(&playlist);
    
    /* Add some Hi-Res audio tracks */
    playlist_add(&playlist, "/music/symphony_192k.flac", 
                 "Symphony No. 9 - Hi-Res", 4200000);
    playlist_add(&playlist, "/music/jazz_96k.flac", 
                 "Jazz Night - 96kHz", 1800000);
    playlist_add(&playlist, "/music/rock_48k.flac", 
                 "Rock Anthem", 2100000);
    
    printf("=== Original Playlist ===\n");
    playlist_print(&playlist);
    
    /* Write to M3U file */
    printf("\n=== Writing Playlist ===\n");
    if (write_m3u_playlist("playlist.m3u", &playlist) == 0) {
        printf("Playlist written to playlist.m3u\n");
    }
    
    /* Free original playlist */
    playlist_free(&playlist);
    
    /* Read back from M3U file */
    printf("\n=== Reading Playlist ===\n");
    Playlist read_playlist;
    playlist_init(&read_playlist);
    
    if (read_m3u_playlist("playlist.m3u", &read_playlist) == 0) {
        printf("Playlist read from playlist.m3u\n\n");
        playlist_print(&read_playlist);
    }
    
    /* Cleanup */
    playlist_free(&read_playlist);
    
    return 0;
}
