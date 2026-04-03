/*
 * Loops.kt - Loop statements demonstration
 * Compile: kotlinc Loops.kt -include-runtime -d Loops.jar
 * Run: java -jar Loops.jar
 */

/**
 * Generates a sine wave sample at the given parameters.
 */
fun generateSineWave(
    sampleRate: Int,
    frequency: Double,
    durationSeconds: Double,
    amplitude: Double = 1.0
): DoubleArray {
    val numSamples = (sampleRate * durationSeconds).toInt()
    val samples = DoubleArray(numSamples)
    
    // For loop with range: iterate through all samples
    for (i in 0 until numSamples) {
        val t = i.toDouble() / sampleRate
        samples[i] = amplitude * kotlin.math.sin(2.0 * kotlin.math.PI * frequency * t)
    }
    
    return samples
}

/**
 * Calculates the peak amplitude of audio samples.
 * Demonstrates for loop with direct iteration.
 */
fun calculatePeakAmplitude(samples: DoubleArray): Double {
    var peak = 0.0
    
    // Direct iteration over array elements
    for (sample in samples) {
        val absValue = kotlin.math.abs(sample)
        if (absValue > peak) {
            peak = absValue
        }
    }
    
    return peak
}

/**
 * Calculates RMS (Root Mean Square) level.
 * Demonstrates for loop with index.
 */
fun calculateRms(samples: DoubleArray): Double {
    var sumSquares = 0.0
    
    // Iteration with index using withIndex()
    for ((index, sample) in samples.withIndex()) {
        sumSquares += sample * sample
    }
    
    return kotlin.math.sqrt(sumSquares / samples.size)
}

/**
 * Finds the first sample above threshold.
 * Demonstrates early exit with break.
 */
fun findFirstAboveThreshold(samples: DoubleArray, threshold: Double): Int {
    for (i in samples.indices) {
        if (kotlin.math.abs(samples[i]) > threshold) {
            return i  // Early return
        }
    }
    return -1  // Not found
}

/**
 * Counts samples within a range.
 * Demonstrates continue usage.
 */
fun countSamplesInRange(
    samples: DoubleArray,
    minVal: Double,
    maxVal: Double
): Int {
    var count = 0
    
    for (sample in samples) {
        // Skip samples outside range
        if (sample < minVal || sample > maxVal) {
            continue
        }
        count++
    }
    
    return count
}

/**
 * Processes multi-channel audio.
 * Demonstrates nested loops with labels.
 */
fun processMultiChannel(
    channels: List<DoubleArray>,
    gain: Double
): List<DoubleArray> {
    val result = mutableListOf<DoubleArray>()
    
    // Outer loop: channels
    for ((channelIndex, channel) in channels.withIndex()) {
        val processedChannel = DoubleArray(channel.size)
        
        // Inner loop: samples
        for (sampleIndex in channel.indices) {
            processedChannel[sampleIndex] = channel[sampleIndex] * gain
        }
        
        result.add(processedChannel)
        println("Processed channel $channelIndex")
    }
    
    return result
}

/**
 * Demonstrates labeled break.
 * Finds a specific sample pattern across channels.
 */
fun findPatternInChannels(channels: List<DoubleArray>, pattern: Double): Boolean {
    found@ for ((channelIndex, channel) in channels.withIndex()) {
        for (sampleIndex in channel.indices) {
            if (kotlin.math.abs(channel[sampleIndex] - pattern) < 0.001) {
                println("Pattern found at channel $channelIndex, sample $sampleIndex")
                break@found  // Exit both loops
            }
        }
    }
    
    return true
}

/**
 * Simulates FFmpeg decode loop.
 * Demonstrates while loop pattern.
 */
fun simulateDecodeLoop(maxPackets: Int): DecodeResult {
    var packetsProcessed = 0
    var framesDecoded = 0
    var errors = 0
    
    // While loop for unknown iteration count
    while (packetsProcessed < maxPackets) {
        // Simulate packet processing
        val success = Math.random() > 0.1  // 90% success rate
        
        if (!success) {
            errors++
            if (errors > 3) {
                println("Too many errors, stopping")
                break
            }
            continue
        }
        
        // Simulate frame decoding
        val framesInPacket = (1..3).random()
        repeat(framesInPacket) {
            framesDecoded++
        }
        
        packetsProcessed++
    }
    
    return DecodeResult(packetsProcessed, framesDecoded, errors)
}

data class DecodeResult(
    val packetsProcessed: Int,
    val framesDecoded: Int,
    val errors: Int
)

/**
 * Demonstrates do-while for retry logic.
 */
fun connectWithRetry(maxRetries: Int): ConnectionResult {
    var retryCount = 0
    var connected = false
    
    // Do-while: at least one attempt
    do {
        retryCount++
        println("Connection attempt $retryCount")
        
        // Simulate connection (succeeds after 2 attempts)
        connected = retryCount >= 2
        
        if (!connected && retryCount < maxRetries) {
            Thread.sleep(100)  // Wait before retry
        }
    } while (!connected && retryCount < maxRetries)
    
    return ConnectionResult(connected, retryCount)
}

data class ConnectionResult(
    val success: Boolean,
    val attempts: Int
)

/**
 * Main function demonstrating loop statements.
 */
fun main() {
    println("=== Kotlin Loop Statements Demo ===\n")
    
    // Generate test audio
    println("Generating 1kHz sine wave at 192kHz...")
    val samples = generateSineWave(192000, 1000.0, 1.0, 0.8)
    println("Generated ${samples.size} samples\n")
    
    // Calculate metrics
    println("Audio Metrics:")
    println("  Peak amplitude: ${calculatePeakAmplitude(samples)}")
    println("  RMS level: ${calculateRms(samples)}")
    
    // Find threshold
    val threshold = 0.7
    val firstAbove = findFirstAboveThreshold(samples, threshold)
    println("  First sample above $threshold: index $firstAbove")
    
    // Count in range
    val inRange = countSamplesInRange(samples, -0.5, 0.5)
    println("  Samples in [-0.5, 0.5]: $inRange (${100.0 * inRange / samples.size}%)\n")
    
    // Multi-channel processing
    println("Multi-channel Processing:")
    val channels = listOf(
        DoubleArray(100) { Math.random() },
        DoubleArray(100) { Math.random() }
    )
    val processed = processMultiChannel(channels, 0.5)
    println("  Processed ${processed.size} channels\n")
    
    // Decode simulation
    println("Decode Simulation:")
    val decodeResult = simulateDecodeLoop(10)
    println("  Packets: ${decodeResult.packetsProcessed}")
    println("  Frames: ${decodeResult.framesDecoded}")
    println("  Errors: ${decodeResult.errors}\n")
    
    // Connection retry
    println("Connection Retry:")
    val connectionResult = connectWithRetry(5)
    println("  Connected: ${connectionResult.success}")
    println("  Attempts: ${connectionResult.attempts}")
}
