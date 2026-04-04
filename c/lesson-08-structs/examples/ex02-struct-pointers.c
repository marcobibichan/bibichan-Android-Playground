/*
 * Purpose: Demonstrate structure pointers, dynamic allocation, and arrow operator
 * Dependencies: stdio.h, stdlib.h, string.h
 * Compile: gcc -o ex02 ex02-struct-pointers.c -Wall -Wextra
 * Run: ./ex02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define a structure for audio track information */
typedef struct AudioTrack {
    char* title;              /* Dynamic string for title */
    char* artist;             /* Dynamic string for artist */
    char* filepath;           /* Dynamic string for file path */
    int duration_ms;          /* Duration in milliseconds */
    int sample_rate;          /* Sample rate in Hz */
    struct AudioTrack* next;  /* Pointer to next track (linked list) */
} AudioTrack;

/* Function to create a new track (factory function) */
AudioTrack* create_track(const char* title, const char* artist,
                         const char* filepath, int duration, int sample_rate) {
    /* Allocate memory for the structure */
    AudioTrack* track = (AudioTrack*)malloc(sizeof(AudioTrack));
    if (track == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for track\n");
        return NULL;
    }
    
    /* Allocate and copy strings */
    track->title = strdup(title);
    track->artist = strdup(artist);
    track->filepath = strdup(filepath);
    
    /* Check for allocation failures */
    if (track->title == NULL || track->artist == NULL || track->filepath == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for strings\n");
        free(track->title);
        free(track->artist);
        free(track->filepath);
        free(track);
        return NULL;
    }
    
    /* Set numeric fields */
    track->duration_ms = duration;
    track->sample_rate = sample_rate;
    track->next = NULL;
    
    return track;
}

/* Function to free a track and its strings */
void free_track(AudioTrack* track) {
    if (track != NULL) {
        free(track->title);
        free(track->artist);
        free(track->filepath);
        free(track);
    }
}

/* Function to print track information */
void print_track(const AudioTrack* track) {
    if (track == NULL) {
        printf("Track is NULL\n");
        return;
    }
    
    printf("Track Information:\n");
    printf("  Title: %s\n", track->title);
    printf("  Artist: %s\n", track->artist);
    printf("  File: %s\n", track->filepath);
    printf("  Duration: %d ms (%.2f min)\n", 
           track->duration_ms, track->duration_ms / 60000.0);
    printf("  Sample Rate: %d Hz\n", track->sample_rate);
}

int main(void) {
    /* Static structure pointer */
    struct AudioHeader {
        int sample_rate;
        int channels;
        int bit_depth;
    };
    
    /* Allocate structure on heap */
    struct AudioHeader* header = (struct AudioHeader*)malloc(sizeof(struct AudioHeader));
    if (header == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    
    /* Initialize using arrow operator (pointer->member) */
    header->sample_rate = 192000;
    header->channels = 2;
    header->bit_depth = 24;
    
    printf("Dynamic AudioHeader:\n");
    printf("  Sample Rate: %d Hz\n", header->sample_rate);
    printf("  Channels: %d\n", header->channels);
    printf("  Bit Depth: %d bits\n", header->bit_depth);
    
    /* Arrow operator is equivalent to dereference + dot */
    /* header->sample_rate is same as (*header).sample_rate */
    printf("\nUsing (*ptr).member syntax: %d Hz\n", (*header).sample_rate);
    
    free(header);
    
    /* Create linked list of tracks */
    printf("\n=== Playlist (Linked List) ===\n");
    
    AudioTrack* head = create_track("Hi-Res Symphony", "Orchestra", 
                                    "/music/symphony.flac", 240000, 192000);
    AudioTrack* track2 = create_track("Jazz Night", "Jazz Band",
                                      "/music/jazz.flac", 180000, 96000);
    AudioTrack* track3 = create_track("Rock Anthem", "Rock Band",
                                      "/music/rock.flac", 210000, 44100);
    
    /* Link tracks together */
    head->next = track2;
    track2->next = track3;
    
    /* Traverse linked list */
    AudioTrack* current = head;
    int track_num = 1;
    while (current != NULL) {
        printf("\n--- Track %d ---\n", track_num++);
        print_track(current);
        current = current->next;
    }
    
    /* Free all tracks in list */
    current = head;
    while (current != NULL) {
        AudioTrack* next = current->next;
        free_track(current);
        current = next;
    }
    
    return 0;
}
