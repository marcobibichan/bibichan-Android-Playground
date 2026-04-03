/*
 * Conditionals.kt - Conditional expressions demonstration
 * Compile: kotlinc Conditionals.kt -include-runtime -d Conditionals.jar
 * Run: java -jar Conditionals.jar
 */

// Audio quality level enumeration
enum class AudioQuality {
    HI_RES_ULTRA,   // 192kHz/24bit and above
    HI_RES,         // 96kHz/24bit
    CD_QUALITY,     // 44.1kHz/16bit
    LOW_QUALITY     // Below CD quality
}

// Audio codec enumeration
enum class AudioCodec {
    MP3, AAC, FLAC, WAV, OPUS, VORBIS, UNKNOWN
}

/**
 * Detects audio quality level based on sample rate and bit depth.
 * Demonstrates if-else if-else chain as an expression.
 */
fun detectAudioQuality(sampleRate: Int, bitDepth: Int): AudioQuality {
    // if expression with multiple branches
    // Each branch returns a value directly
    return if (sampleRate >= 192000 && bitDepth >= 24) {
        AudioQuality.HI_RES_ULTRA
    } else if (sampleRate >= 96000 && bitDepth >= 24) {
        AudioQuality.HI_RES
    } else if (sampleRate >= 44100 && bitDepth >= 16) {
        AudioQuality.CD_QUALITY
    } else {
        AudioQuality.LOW_QUALITY
    }
}

/**
 * Gets a human-readable description of audio quality.
 * Demonstrates if expression with block body.
 */
fun getQualityDescription(quality: AudioQuality): String {
    // if expression with block body
    // The last expression in each block is the return value
    return if (quality == AudioQuality.HI_RES_ULTRA) {
        println("Detected Hi-Res Ultra")
        "Ultra High Resolution (192kHz/24bit or higher)"
    } else if (quality == AudioQuality.HI_RES) {
        println("Detected Hi-Res")
        "High Resolution (96kHz/24bit)"
    } else if (quality == AudioQuality.CD_QUALITY) {
        println("Detected CD Quality")
        "CD Quality (44.1kHz/16bit)"
    } else {
        println("Detected Low Quality")
        "Low Quality Audio"
    }
}

/**
 * Gets codec name using when expression.
 * Demonstrates basic when usage with enum.
 */
fun getCodecName(codec: AudioCodec): String = when (codec) {
    AudioCodec.MP3 -> "MPEG Audio Layer 3"
    AudioCodec.AAC -> "Advanced Audio Coding"
    AudioCodec.FLAC -> "Free Lossless Audio Codec"
    AudioCodec.WAV -> "Waveform Audio File Format"
    AudioCodec.OPUS -> "Opus Interactive Audio Codec"
    AudioCodec.VORBIS -> "Vorbis Audio Codec"
    AudioCodec.UNKNOWN -> "Unknown Codec"
}

/**
 * Classifies codec as lossy or lossless.
 * Demonstrates multiple values in a single branch.
 */
fun getCodecType(codec: AudioCodec): String = when (codec) {
    AudioCodec.MP3, AudioCodec.AAC, AudioCodec.OPUS, AudioCodec.VORBIS -> "Lossy"
    AudioCodec.FLAC, AudioCodec.WAV -> "Lossless"
    AudioCodec.UNKNOWN -> "Unknown"
}

/**
 * Checks if sample rate is valid for Hi-Res audio.
 * Demonstrates range checking with when.
 */
fun checkSampleRate(sampleRate: Int): String = when (sampleRate) {
    in 192000..Int.MAX_VALUE -> "Hi-Res Ultra range"
    in 96000..191999 -> "Hi-Res range"
    in 44100..95999 -> "Standard range"
    in 1..44099 -> "Low quality range"
    0 -> "Invalid (zero)"
    else -> "Invalid (negative)"
}

/**
 * Processes audio data based on type.
 * Demonstrates type checking with when.
 */
fun processAudioData(data: Any): String = when (data) {
    is ByteArray -> "Processing byte array: ${data.size} bytes"
    is IntArray -> "Processing int array: ${data.size} samples"
    is FloatArray -> "Processing float array: ${data.size} samples"
    is String -> "Processing file path: $data"
    else -> "Unsupported data type: ${data::class.simpleName}"
}

/**
 * Determines audio format based on multiple conditions.
 * Demonstrates complex when condition without subject.
 */
fun determineAudioFormat(
    sampleRate: Int,
    bitDepth: Int,
    channels: Int,
    isLossless: Boolean
): String = when {
    sampleRate >= 192000 && bitDepth >= 24 && isLossless -> "Hi-Res Lossless"
    sampleRate >= 192000 && bitDepth >= 24 -> "Hi-Res (possibly lossy)"
    sampleRate >= 96000 && bitDepth >= 24 && isLossless -> "Hi-Res Lossless"
    sampleRate >= 96000 && bitDepth >= 24 -> "Hi-Res (possibly lossy)"
    channels > 2 -> "Multi-channel audio"
    sampleRate >= 44100 && bitDepth >= 16 -> "CD Quality"
    else -> "Low Quality"
}

/**
 * Main function demonstrating conditional expressions.
 */
fun main() {
    println("=== Kotlin Conditional Expressions Demo ===\n")
    
    // Test audio quality detection
    println("Audio Quality Detection:")
    val testCases = listOf(
        Pair(192000, 24),
        Pair(96000, 24),
        Pair(44100, 16),
        Pair(22050, 8)
    )
    
    for ((sampleRate, bitDepth) in testCases) {
        val quality = detectAudioQuality(sampleRate, bitDepth)
        println("  ${sampleRate}Hz/${bitDepth}bit: $quality")
    }
    
    // Test codec name and type
    println("\nCodec Information:")
    for (codec in AudioCodec.values()) {
        println("  $codec: ${getCodecName(codec)} (${getCodecType(codec)})")
    }
    
    // Test sample rate checking
    println("\nSample Rate Classification:")
    val sampleRates = listOf(192000, 96000, 44100, 22050, 0, -1)
    for (rate in sampleRates) {
        println("  $rate Hz: ${checkSampleRate(rate)}")
    }
    
    // Test type checking
    println("\nType-based Processing:")
    val dataTypes = listOf(
        ByteArray(1024),
        FloatArray(512),
        "audio.flac",
        42
    )
    for (data in dataTypes) {
        println("  ${processAudioData(data)}")
    }
    
    // Test complex conditions
    println("\nComplex Format Determination:")
    println("  192kHz/24bit/2ch/lossless: ${determineAudioFormat(192000, 24, 2, true)}")
    println("  96kHz/24bit/2ch/lossy: ${determineAudioFormat(96000, 24, 2, false)}")
    println("  44.1kHz/16bit/5.1ch: ${determineAudioFormat(44100, 16, 6, true)}")
}
