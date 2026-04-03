// File: AudioTypes.kt
// Purpose: Audio-specific type usage in Kotlin
// Run:     kotlinc AudioTypes.kt -include-runtime -d AudioTypes.jar

// Type aliases for audio
typealias SampleRate = Int
typealias BitDepth = Int
typealias ChannelCount = Int
typealias AudioSamples = ShortArray

/**
 * Audio format configuration
 */
data class AudioFormat(
    val sampleRate: SampleRate,
    val bitDepth: BitDepth,
    val channels: ChannelCount
) {
    val bytesPerSample: Int
        get() = bitDepth / 8
    
    val dataRate: Int
        get() = sampleRate * channels * bytesPerSample
    
    fun bufferSize(durationMs: Int): Int {
        return dataRate * durationMs / 1000
    }
    
    override fun toString(): String {
        return "$sampleRate Hz, $bitDepth-bit, $channels ch"
    }
}

/**
 * Check if format is Hi-Res
 */
fun AudioFormat.isHiRes(): Boolean {
    return sampleRate >= 96000 || bitDepth >= 24
}

fun main() {
    // CD Quality
    val cd = AudioFormat(
        sampleRate = 44100,
        bitDepth = 16,
        channels = 2
    )
    
    println("CD Quality: $cd")
    println("  Data rate: ${cd.dataRate} bytes/sec")
    println("  1-minute size: ${cd.bufferSize(60000)} bytes")
    println("  Is Hi-Res: ${cd.isHiRes()}")
    
    // Hi-Res Quality
    val hires = AudioFormat(
        sampleRate = 192000,
        bitDepth = 24,
        channels = 2
    )
    
    println("\nHi-Res Quality: $hires")
    println("  Data rate: ${hires.dataRate} bytes/sec")
    println("  1-minute size: ${hires.bufferSize(60000)} bytes")
    println("  Is Hi-Res: ${hires.isHiRes()}")
    
    // Type-safe calculations
    val sampleRate: SampleRate = 192000
    val bitDepth: BitDepth = 24
    val channels: ChannelCount = 2
    
    // Using type aliases for clarity
    val dataRate: Int = sampleRate * bitDepth * channels / 8
    println("\nData rate calculation:")
    println("  $sampleRate Hz * $bitDepth bits * $channels ch / 8 = $dataRate bytes/sec")
    
    // Audio samples
    val samples: AudioSamples = ShortArray(4096)
    for (i in samples.indices) {
        samples[i] = (i % 65536 - 32768).toShort()
    }
    println("\nAudio samples:")
    println("  Size: ${samples.size}")
    println("  First 5: ${samples.take(5).toList()}")
}
