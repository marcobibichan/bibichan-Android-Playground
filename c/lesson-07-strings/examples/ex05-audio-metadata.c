/*
 * ex05-audio-metadata.c
 * Purpose: Demonstrate string handling for audio metadata
 * Dependencies: stdio.h, stdlib.h, string.h, ctype.h
 * Compile: gcc -o ex05 ex05-audio-metadata.c
 * Run: ./ex05
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================
 * Audio Metadata Structures
 * ============================================================ */

typedef struct {
    char title[256];
    char artist[256];
    char album[256];
    char genre[64];
    char comment[512];
    int year;
    int track;
    int duration_ms;
    int sample_rate;
    int channels;
    int bit_depth;
} AudioMetadata;

typedef struct {
    char key[64];
    char value[256];
} MetadataField;

/* ============================================================
 * Metadata Parsing Functions
 * ============================================================ */

/* Trim whitespace */
static char *trim_whitespace(char *str) {
    char *end;
    
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == '\0') return str;
    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    
    return str;
}

/* Parse Vorbis comment format: KEY=value */
int parse_vorbis_comment(const char *line, MetadataField *field) {
    const char *eq = strchr(line, '=');
    if (eq == NULL) return -1;
    
    /* Extract key */
    size_t key_len = eq - line;
    if (key_len >= sizeof(field->key)) key_len = sizeof(field->key) - 1;
    strncpy(field->key, line, key_len);
    field->key[key_len] = '\0';
    
    /* Convert key to uppercase (Vorbis convention) */
    for (char *p = field->key; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    
    /* Extract value */
    strncpy(field->value, eq + 1, sizeof(field->value) - 1);
    field->value[sizeof(field->value) - 1] = '\0';
    trim_whitespace(field->value);
    
    return 0;
}

/* Initialize metadata structure */
void metadata_init(AudioMetadata *meta) {
    memset(meta, 0, sizeof(AudioMetadata));
    strcpy(meta->title, "Unknown Title");
    strcpy(meta->artist, "Unknown Artist");
    strcpy(meta->album, "Unknown Album");
}

/* Apply metadata field to structure */
void metadata_apply_field(AudioMetadata *meta, const MetadataField *field) {
    if (strcmp(field->key, "TITLE") == 0) {
        strncpy(meta->title, field->value, sizeof(meta->title) - 1);
    } else if (strcmp(field->key, "ARTIST") == 0) {
        strncpy(meta->artist, field->value, sizeof(meta->artist) - 1);
    } else if (strcmp(field->key, "ALBUM") == 0) {
        strncpy(meta->album, field->value, sizeof(meta->album) - 1);
    } else if (strcmp(field->key, "GENRE") == 0) {
        strncpy(meta->genre, field->value, sizeof(meta->genre) - 1);
    } else if (strcmp(field->key, "DATE") == 0 || 
               strcmp(field->key, "YEAR") == 0) {
        meta->year = atoi(field->value);
    } else if (strcmp(field->key, "TRACKNUMBER") == 0 ||
               strcmp(field->key, "TRACK") == 0) {
        meta->track = atoi(field->value);
    } else if (strcmp(field->key, "COMMENT") == 0 ||
               strcmp(field->key, "DESCRIPTION") == 0) {
        strncpy(meta->comment, field->value, sizeof(meta->comment) - 1);
    }
}

/* Parse multiple metadata lines */
int parse_metadata_lines(const char *lines[], int count, AudioMetadata *meta) {
    metadata_init(meta);
    
    for (int i = 0; i < count; i++) {
        MetadataField field;
        if (parse_vorbis_comment(lines[i], &field) == 0) {
            metadata_apply_field(meta, &field);
        }
    }
    
    return 0;
}

/* ============================================================
 * Path and Filename Handling
 * ============================================================ */

/* Extract filename from path */
const char *get_filename(const char *path) {
    const char *filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = strrchr(path, '\\');  /* Windows */
    }
    return filename ? filename + 1 : path;
}

/* Get file extension */
const char *get_extension(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext ? ext + 1 : "";
}

/* Check if file is supported audio format */
int is_supported_format(const char *filename) {
    const char *ext = get_extension(filename);
    
    /* Convert to lowercase for comparison */
    char ext_lower[16];
    size_t i;
    for (i = 0; ext[i] && i < sizeof(ext_lower) - 1; i++) {
        ext_lower[i] = tolower((unsigned char)ext[i]);
    }
    ext_lower[i] = '\0';
    
    /* Check supported formats */
    const char *supported[] = {
        "flac", "wav", "mp3", "ogg", "m4a", "aac", "opus", NULL
    };
    
    for (i = 0; supported[i] != NULL; i++) {
        if (strcmp(ext_lower, supported[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

/* Build output filename */
int build_output_filename(char *dest, size_t dest_size,
                          const char *artist, const char *title,
                          const char *ext) {
    return snprintf(dest, dest_size, "%s - %s.%s", artist, title, ext);
}

/* Build safe filename (replace invalid characters) */
void sanitize_filename(char *filename) {
    const char *invalid_chars = "<>:\"/\\|?*";
    
    for (char *p = filename; *p; p++) {
        if (strchr(invalid_chars, *p) != NULL) {
            *p = '_';  /* Replace with underscore */
        }
    }
}

/* ============================================================
 * Display Functions
 * ============================================================ */

/* Print metadata */
void print_metadata(const AudioMetadata *meta) {
    printf("=== Audio Metadata ===\n");
    printf("Title:      %s\n", meta->title);
    printf("Artist:     %s\n", meta->artist);
    printf("Album:      %s\n", meta->album);
    printf("Genre:      %s\n", meta->genre);
    printf("Year:       %d\n", meta->year);
    printf("Track:      %d\n", meta->track);
    printf("Duration:   %d ms\n", meta->duration_ms);
    printf("Sample Rate: %d Hz\n", meta->sample_rate);
    printf("Channels:   %d\n", meta->channels);
    printf("Bit Depth:  %d\n", meta->bit_depth);
    if (meta->comment[0] != '\0') {
        printf("Comment:    %s\n", meta->comment);
    }
}

/* Format duration as MM:SS */
void format_duration(int duration_ms, char *buffer, size_t buffer_size) {
    int total_seconds = duration_ms / 1000;
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;
    snprintf(buffer, buffer_size, "%d:%02d", minutes, seconds);
}

/* ============================================================
 * Main Function
 * ============================================================ */

int main(void) {
    printf("========================================\n");
    printf("Audio Metadata String Handling Demo\n");
    printf("========================================\n\n");
    
    /* Simulated FLAC Vorbis comments */
    const char *vorbis_comments[] = {
        "TITLE=Money",
        "ARTIST=Pink Floyd",
        "ALBUM=The Dark Side of the Moon",
        "DATE=1973",
        "TRACKNUMBER=6",
        "GENRE=Progressive Rock",
        "COMMENT=24-bit/192kHz Hi-Res FLAC"
    };
    
    /* Parse metadata */
    AudioMetadata meta;
    parse_metadata_lines(vorbis_comments, 
                         sizeof(vorbis_comments) / sizeof(vorbis_comments[0]),
                         &meta);
    
    /* Add technical info */
    meta.sample_rate = 192000;
    meta.channels = 2;
    meta.bit_depth = 24;
    meta.duration_ms = 382000;  /* 6:22 */
    
    print_metadata(&meta);
    
    /* Path handling demo */
    printf("\n=== Path Handling ===\n");
    const char *test_paths[] = {
        "/music/Pink Floyd/Dark Side/Money.flac",
        "C:\\Music\\Hi-Res\\track01.wav",
        "song.mp3",
        "/home/user/Music/artist-album-track.flac"
    };
    
    for (int i = 0; i < 4; i++) {
        const char *path = test_paths[i];
        const char *filename = get_filename(path);
        const char *ext = get_extension(filename);
        
        printf("\nPath: %s\n", path);
        printf("  Filename: %s\n", filename);
        printf("  Extension: %s\n", ext);
        printf("  Supported: %s\n", 
               is_supported_format(filename) ? "Yes" : "No");
    }
    
    /* Build output filename */
    printf("\n=== Output Filename Builder ===\n");
    char output[256];
    build_output_filename(output, sizeof(output), 
                          meta.artist, meta.title, "flac");
    printf("Output filename: %s\n", output);
    
    /* Sanitize filename */
    char unsafe_name[] = "Pink Floyd: The Dark Side of the Moon";
    printf("\nUnsafe filename: %s\n", unsafe_name);
    sanitize_filename(unsafe_name);
    printf("Sanitized: %s\n", unsafe_name);
    
    /* Format string for display */
    printf("\n=== Formatted Display ===\n");
    char display[512];
    snprintf(display, sizeof(display),
             "Now Playing: %s by %s\n"
             "From: %s (%d)\n"
             "Format: %d Hz / %d-bit / %d ch",
             meta.title, meta.artist, meta.album, meta.year,
             meta.sample_rate, meta.bit_depth, meta.channels);
    printf("%s\n", display);
    
    /* Duration formatting */
    printf("\n=== Duration Formatting ===\n");
    char duration_str[16];
    format_duration(meta.duration_ms, duration_str, sizeof(duration_str));
    printf("Duration: %s (%d ms)\n", duration_str, meta.duration_ms);
    
    /* Metadata comparison */
    printf("\n=== Metadata Comparison ===\n");
    AudioMetadata meta2;
    const char *vorbis_comments2[] = {
        "TITLE=Time",
        "ARTIST=Pink Floyd",
        "ALBUM=The Dark Side of the Moon",
        "DATE=1973",
        "TRACKNUMBER=4"
    };
    parse_metadata_lines(vorbis_comments2, 5, &meta2);
    
    printf("Comparing '%s' with '%s':\n", meta.title, meta2.title);
    printf("  Same artist: %s\n", 
           strcmp(meta.artist, meta2.artist) == 0 ? "Yes" : "No");
    printf("  Same album: %s\n", 
           strcmp(meta.album, meta2.album) == 0 ? "Yes" : "No");
    printf("  Same year: %s\n", 
           meta.year == meta2.year ? "Yes" : "No");
    
    return 0;
}
