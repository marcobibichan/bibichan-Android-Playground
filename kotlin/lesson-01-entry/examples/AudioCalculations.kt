// File: AudioCalculations.kt
// Purpose: Audio-related calculations demonstrating Kotlin features
// Run:     kotlinc AudioCalculations.kt -include-runtime -d Audio.jar

/**
 * Audio format constants
 */
object AudioConstants {
    const val SAMPLE_RATE_CD = 44100
    const val SAMPLE_RATE_HIRES = 192000
    const val BIT_DEPTH_CD = 16
    const val BIT_DEPTH_HIRES = 24
    const val CHANNELS_STEREO = 2
}

/**
 * Audio format data class
 */
data class AudioFormat(
    val sampleRate: Int,
    val bitDepth: Int,
    val channels: Int
) {
    /**
     * Calculate data rate in bytes per second
     */
    fun dataRate(): Int = sampleRate * bitDepth * channels / 8
    
    /**
     * Calculate buffer size for given duration
     */
    fun bufferSize(durationMs: Int): Int = dataRate() * durationMs / 1000
    
    /**
     * Format as string
     */
    override fun toString(): String {
        return "$sampleRate Hz, $bitDepth-bit, $channels channels"
    }
}

/**
 * Extension function for AudioFormat
 */
fun AudioFormat.isHiRes(): Boolean {
    return sampleRate >= 96000 || bitDepth >= 24
}

/**
 * Main function demonstrating audio calculations
 */
fun main() {
    // CD Quality
    val cd = AudioFormat(
        sampleRate = AudioConstants.SAMPLE_RATE_CD,
        bitDepth = AudioConstants.BIT_DEPTH_CD,
        channels = AudioConstants.CHANNELS_STEREO
    )
    
    println("CD Quality: $cd")
    println("Data Rate: ${cd.dataRate()} bytes/sec")
    println("1-minute size: ${cd.bufferSize(60000)} bytes")
    println("Is Hi-Res: ${cd.isHiRes()}")
    
    println()
    
    // Hi-Res Quality
    val hires = AudioFormat(
        sampleRate = AudioConstants.SAMPLE_RATE_HIRES,
        bitDepth = AudioConstants.BIT_DEPTH_HIRES,
        channels = AudioConstants.CHANNELS_STEREO
    )
    
    println("Hi-Res Quality: $hires")
    println("Data Rate: ${hires.dataRate()} bytes/sec")
    println("1-minute size: ${hires.bufferSize(60000)} bytes")
    println("Is Hi-Res: ${hires.isHiRes()}")
    
    println()
    
    // Comparison
    println("Hi-Res is ${hires.dataRate() / cd.dataRate()}x larger than CD")
    
    // Using named arguments
    val custom = AudioFormat(
        sampleRate = 96000,
        bitDepth = 32,
        channels = 2
    )
    println("\nCustom: $custom")
}

/**
 * Calculate file size for audio
 */
fun calculateFileSize(
    format: AudioFormat,
    durationSeconds: Int
): Long {
    return format.dataRate().toLong() * durationSeconds
}

/**
 * Format file size as human-readable string
 */
fun formatFileSize(bytes: Long): String {
    return when {
        bytes < 1024 -> "$bytes B"
        bytes < 1024 * 1024 -> "${bytes / 1024} KB"
        bytes < 1024 * 1024 * 1024 -> "${bytes / (1024 * 1024)} MB"
        else -> "${bytes / (1024 * 1024 * 1024)} GB"
    }
}
