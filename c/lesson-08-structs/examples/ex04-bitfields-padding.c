/*
 * Purpose: Demonstrate bit fields and structure padding analysis
 * Dependencies: stdio.h, stdint.h
 * Compile: gcc -o ex04 ex04-bitfields-padding.c -Wall -Wextra
 * Run: ./ex04
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Structure without bit fields - normal padding */
struct NormalFlags {
    uint8_t is_compressed;   /* 1 byte */
    uint8_t is_float;        /* 1 byte */
    uint8_t channels;        /* 1 byte */
    uint8_t bit_depth;       /* 1 byte */
};  /* Total: 4 bytes */

/* Structure with bit fields - compact storage */
struct BitFieldFlags {
    uint8_t is_compressed : 1;   /* 1 bit */
    uint8_t is_float : 1;         /* 1 bit */
    uint8_t channels : 4;         /* 4 bits (supports 0-15 channels) */
    uint8_t bit_depth : 6;        /* 6 bits (supports 0-63 bit depth) */
};  /* Total: 2 bytes (may vary by compiler) */

/* Audio header with intentional padding */
struct PaddedHeader {
    char type[4];           /* 4 bytes */
    uint32_t size;          /* 4 bytes */
    uint16_t format;        /* 2 bytes */
    /* 2 bytes padding here (for uint32_t alignment) */
    uint32_t sample_rate;   /* 4 bytes */
    uint32_t byte_rate;     /* 4 bytes */
};  /* Total: 20 bytes (with padding) */

/* Audio header with packed attribute (GCC extension) */
struct __attribute__((packed)) PackedHeader {
    char type[4];           /* 4 bytes */
    uint32_t size;          /* 4 bytes */
    uint16_t format;        /* 2 bytes */
    uint32_t sample_rate;   /* 4 bytes (no padding before this) */
    uint32_t byte_rate;     /* 4 bytes */
};  /* Total: 18 bytes (no padding) */

/* FLAC frame header bit field simulation */
struct FlacFrameHeader {
    uint32_t sync_code : 14;        /* 14 bits: sync code */
    uint32_t reserved : 1;           /* 1 bit: reserved */
    uint32_t blocking_strategy : 1;  /* 1 bit: blocking strategy */
    uint32_t block_size : 4;         /* 4 bits: block size */
    uint32_t sample_rate : 4;        /* 4 bits: sample rate */
    uint32_t channel_assignment : 4; /* 4 bits: channel assignment */
    uint32_t sample_size : 3;        /* 3 bits: sample size */
    uint32_t reserved2 : 1;          /* 1 bit: reserved */
};  /* Total: 32 bits = 4 bytes */

/* Function to print memory layout */
void print_memory_layout(const void* ptr, size_t size, const char* name) {
    const uint8_t* bytes = (const uint8_t*)ptr;
    printf("%s (%zu bytes): ", name, size);
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", bytes[i]);
    }
    printf("\n");
}

int main(void) {
    printf("=== Structure Size Comparison ===\n");
    printf("sizeof(NormalFlags): %zu bytes\n", sizeof(struct NormalFlags));
    printf("sizeof(BitFieldFlags): %zu bytes\n", sizeof(struct BitFieldFlags));
    printf("sizeof(PaddedHeader): %zu bytes\n", sizeof(struct PaddedHeader));
    printf("sizeof(PackedHeader): %zu bytes\n", sizeof(struct PackedHeader));
    printf("sizeof(FlacFrameHeader): %zu bytes\n", sizeof(struct FlacFrameHeader));
    
    /* Normal flags usage */
    printf("\n=== Normal Flags Usage ===\n");
    struct NormalFlags normal = {1, 0, 2, 24};
    printf("Normal: compressed=%d, float=%d, channels=%d, bit_depth=%d\n",
           normal.is_compressed, normal.is_float, normal.channels, normal.bit_depth);
    print_memory_layout(&normal, sizeof(normal), "NormalFlags");
    
    /* Bit field usage */
    printf("\n=== Bit Field Usage ===\n");
    struct BitFieldFlags bf = {0};
    bf.is_compressed = 1;
    bf.is_float = 0;
    bf.channels = 2;
    bf.bit_depth = 24;
    printf("BitField: compressed=%d, float=%d, channels=%d, bit_depth=%d\n",
           bf.is_compressed, bf.is_float, bf.channels, bf.bit_depth);
    print_memory_layout(&bf, sizeof(bf), "BitFieldFlags");
    
    /* Padded header */
    printf("\n=== Padded Header ===\n");
    struct PaddedHeader padded = {0};
    memcpy(padded.type, "RIFF", 4);
    padded.size = 1024;
    padded.format = 1;
    padded.sample_rate = 192000;
    padded.byte_rate = 192000 * 2 * 3;
    print_memory_layout(&padded, sizeof(padded), "PaddedHeader");
    
    /* Packed header */
    printf("\n=== Packed Header ===\n");
    struct PackedHeader packed = {0};
    memcpy(packed.type, "RIFF", 4);
    packed.size = 1024;
    packed.format = 1;
    packed.sample_rate = 192000;
    packed.byte_rate = 192000 * 2 * 3;
    print_memory_layout(&packed, sizeof(packed), "PackedHeader");
    
    /* FLAC frame header */
    printf("\n=== FLAC Frame Header ===\n");
    struct FlacFrameHeader flac = {0};
    flac.sync_code = 0x3FFE;        /* 14-bit sync code */
    flac.blocking_strategy = 1;      /* Variable block size */
    flac.block_size = 0x6;           /* 4096 samples */
    flac.sample_rate = 0x8;          /* 192kHz */
    flac.channel_assignment = 0x2;   /* Stereo */
    flac.sample_size = 0x4;          /* 24-bit */
    print_memory_layout(&flac, sizeof(flac), "FlacFrameHeader");
    
    /* Offset analysis */
    printf("\n=== Member Offsets ===\n");
    printf("PaddedHeader offsets:\n");
    printf("  type: %zu\n", offsetof(struct PaddedHeader, type));
    printf("  size: %zu\n", offsetof(struct PaddedHeader, size));
    printf("  format: %zu\n", offsetof(struct PaddedHeader, format));
    printf("  sample_rate: %zu\n", offsetof(struct PaddedHeader, sample_rate));
    printf("  byte_rate: %zu\n", offsetof(struct PaddedHeader, byte_rate));
    
    printf("\nPackedHeader offsets:\n");
    printf("  type: %zu\n", offsetof(struct PackedHeader, type));
    printf("  size: %zu\n", offsetof(struct PackedHeader, size));
    printf("  format: %zu\n", offsetof(struct PackedHeader, format));
    printf("  sample_rate: %zu\n", offsetof(struct PackedHeader, sample_rate));
    printf("  byte_rate: %zu\n", offsetof(struct PackedHeader, byte_rate));
    
    return 0;
}
