/*
 * AudioControlFlow.kt - Audio control flow for Android
 * This file demonstrates control flow patterns used in Android audio apps
 * 
 * Note: This is a demonstration file. In a real Android app, you would:
 * - Use lifecycle-aware components
 * - Implement proper error handling
 * - Use coroutines for async operations
 */

/**
 * Player state sealed class for state machine.
 * Sealed classes are perfect for when expressions.
 */
sealed class PlayerState {
    object Idle : PlayerState()
    object Loading : PlayerState()
    data class Playing(val position: Long, val duration: Long) : PlayerState()
    data class Paused(val position: Long) : PlayerState()
    data class Error(val message: String, val code: Int) : PlayerState()
    object Completed : PlayerState()
}

/**
 * Audio format information.
 */
data class AudioFormat(
    val sampleRate: Int,
    val bitDepth: Int,
    val channels: Int,
    val codec: String
) {
    /**
     * Determines if this is Hi-Res audio.
     * Uses when expression with range checking.
     */
    fun isHiRes(): Boolean = when {
        sampleRate >= 96000 && bitDepth >= 24 -> true
        else -> false
    }
    
    /**
     * Gets quality description using when expression.
     */
    fun getQualityDescription(): String = when {
        sampleRate >= 192000 && bitDepth >= 24 -> "Hi-Res Ultra (192kHz/24bit)"
        sampleRate >= 96000 && bitDepth >= 24 -> "Hi-Res (96kHz/24bit)"
        sampleRate >= 48000 && bitDepth >= 24 -> "High Quality (48kHz/24bit)"
        sampleRate >= 44100 && bitDepth >= 16 -> "CD Quality (44.1kHz/16bit)"
        else -> "Low Quality"
    }
    
    /**
     * Gets channel configuration string.
     */
    fun getChannelConfig(): String = when (channels) {
        1 -> "Mono"
        2 -> "Stereo"
        in 3..5 -> "Surround"
        6 -> "5.1 Surround"
        8 -> "7.1 Surround"
        else -> "Unknown ($channels channels)"
    }
}

/**
 * Audio buffer processor using control flow.
 */
class AudioBufferProcessor {
    /**
     * Processes audio buffer with gain and limiting.
     * Demonstrates for loop with early exit.
     */
    fun processBuffer(
        buffer: FloatArray,
        gain: Float,
        limitThreshold: Float = 1.0f
    ): ProcessResult {
        var clippedSamples = 0
        var peakLevel = 0.0f
        
        for (i in buffer.indices) {
            // Apply gain
            var sample = buffer[i] * gain
            
            // Check peak
            val absSample = kotlin.math.abs(sample)
            if (absSample > peakLevel) {
                peakLevel = absSample
            }
            
            // Apply limiting
            when {
                sample > limitThreshold -> {
                    sample = limitThreshold
                    clippedSamples++
                }
                sample < -limitThreshold -> {
                    sample = -limitThreshold
                    clippedSamples++
                }
            }
            
            buffer[i] = sample
        }
        
        return ProcessResult(
            processedSamples = buffer.size,
            clippedSamples = clippedSamples,
            peakLevel = peakLevel
        )
    }
    
    /**
     * Finds silence regions in audio.
     * Demonstrates state machine pattern.
     */
    fun findSilenceRegions(
        buffer: FloatArray,
        threshold: Float,
        minSilenceSamples: Int
    ): List<SilenceRegion> {
        val regions = mutableListOf<SilenceRegion>()
        var silenceStart = -1
        var silenceCount = 0
        
        for (i in buffer.indices) {
            val isSilent = kotlin.math.abs(buffer[i]) < threshold
            
            when {
                isSilent && silenceStart == -1 -> {
                    // Start of silence
                    silenceStart = i
                    silenceCount = 1
                }
                isSilent -> {
                    // Continue silence
                    silenceCount++
                }
                !isSilent && silenceStart != -1 -> {
                    // End of silence
                    if (silenceCount >= minSilenceSamples) {
                        regions.add(SilenceRegion(silenceStart, i - 1))
                    }
                    silenceStart = -1
                    silenceCount = 0
                }
            }
        }
        
        // Handle trailing silence
        if (silenceCount >= minSilenceSamples) {
            regions.add(SilenceRegion(silenceStart, buffer.size - 1))
        }
        
        return regions
    }
}

data class ProcessResult(
    val processedSamples: Int,
    val clippedSamples: Int,
    val peakLevel: Float
)

data class SilenceRegion(
    val startIndex: Int,
    val endIndex: Int
)

/**
 * Player state machine handler.
 * Demonstrates when expression with sealed class.
 */
class PlayerStateMachine {
    private var state: PlayerState = PlayerState.Idle
    private var listeners = mutableListOf<(PlayerState) -> Unit>()
    
    fun addListener(listener: (PlayerState) -> Unit) {
        listeners.add(listener)
    }
    
    /**
     * Handles state transitions using when expression.
     */
    fun handleEvent(event: PlayerEvent) {
        val newState = when (event) {
            is PlayerEvent.Load -> {
                when (state) {
                    is PlayerState.Idle, 
                    is PlayerState.Completed,
                    is PlayerState.Error -> PlayerState.Loading
                    else -> state  // Invalid transition, stay in current state
                }
            }
            is PlayerEvent.Play -> {
                when (state) {
                    is PlayerState.Loading -> PlayerState.Playing(0, event.duration)
                    is PlayerState.Paused -> PlayerState.Playing(event.position, event.duration)
                    else -> state
                }
            }
            is PlayerEvent.Pause -> {
                when (state) {
                    is PlayerState.Playing -> PlayerState.Paused(state.position)
                    else -> state
                }
            }
            is PlayerEvent.Seek -> {
                when (state) {
                    is PlayerState.Playing -> state.copy(position = event.position)
                    is PlayerState.Paused -> state.copy(position = event.position)
                    else -> state
                }
            }
            is PlayerEvent.Complete -> {
                when (state) {
                    is PlayerState.Playing -> PlayerState.Completed
                    else -> state
                }
            }
            is PlayerEvent.Error -> {
                PlayerState.Error(event.message, event.code)
            }
            is PlayerEvent.Stop -> {
                PlayerState.Idle
            }
        }
        
        if (newState != state) {
            state = newState
            notifyListeners()
        }
    }
    
    /**
     * Gets UI state based on current player state.
     */
    fun getUiState(): UiState = when (state) {
        is PlayerState.Idle -> UiState(
            showPlayButton = true,
            showPauseButton = false,
            showProgress = false,
            statusText = "Ready to play"
        )
        is PlayerState.Loading -> UiState(
            showPlayButton = false,
            showPauseButton = false,
            showProgress = true,
            statusText = "Loading..."
        )
        is PlayerState.Playing -> {
            val playing = state as PlayerState.Playing
            UiState(
                showPlayButton = false,
                showPauseButton = true,
                showProgress = true,
                statusText = "Playing ${formatTime(playing.position)} / ${formatTime(playing.duration)}"
            )
        }
        is PlayerState.Paused -> {
            val paused = state as PlayerState.Paused
            UiState(
                showPlayButton = true,
                showPauseButton = false,
                showProgress = true,
                statusText = "Paused at ${formatTime(paused.position)}"
            )
        }
        is PlayerState.Error -> {
            val error = state as PlayerState.Error
            UiState(
                showPlayButton = true,
                showPauseButton = false,
                showProgress = false,
                statusText = "Error: ${error.message}"
            )
        }
        is PlayerState.Completed -> UiState(
            showPlayButton = true,
            showPauseButton = false,
            showProgress = true,
            statusText = "Completed"
        )
    }
    
    private fun notifyListeners() {
        for (listener in listeners) {
            listener(state)
        }
    }
    
    private fun formatTime(ms: Long): String {
        val seconds = (ms / 1000) % 60
        val minutes = (ms / (1000 * 60)) % 60
        return String.format("%02d:%02d", minutes, seconds)
    }
}

sealed class PlayerEvent {
    data class Load(val url: String) : PlayerEvent()
    data class Play(val position: Long = 0, val duration: Long = 0) : PlayerEvent()
    data class Pause(val position: Long) : PlayerEvent()
    data class Seek(val position: Long) : PlayerEvent()
    object Complete : PlayerEvent()
    data class Error(val message: String, val code: Int) : PlayerEvent()
    object Stop : PlayerEvent()
}

data class UiState(
    val showPlayButton: Boolean,
    val showPauseButton: Boolean,
    val showProgress: Boolean,
    val statusText: String
)

/**
 * Main function demonstrating audio control flow.
 */
fun main() {
    println("=== Audio Control Flow Demo ===\n")
    
    // Test audio format
    println("Audio Format Testing:")
    val formats = listOf(
        AudioFormat(192000, 24, 2, "FLAC"),
        AudioFormat(96000, 24, 2, "FLAC"),
        AudioFormat(44100, 16, 2, "WAV"),
        AudioFormat(48000, 16, 6, "AC3")
    )
    
    for (format in formats) {
        println("  ${format.codec}: ${format.getQualityDescription()}, ${format.getChannelConfig()}")
    }
    
    // Test buffer processing
    println("\nBuffer Processing:")
    val processor = AudioBufferProcessor()
    val buffer = FloatArray(1000) { (Math.random() * 2 - 1).toFloat() }
    val result = processor.processBuffer(buffer, 1.5f, 1.0f)
    println("  Processed: ${result.processedSamples} samples")
    println("  Clipped: ${result.clippedSamples} samples")
    println("  Peak: ${result.peakLevel}")
    
    // Test silence detection
    println("\nSilence Detection:")
    val testBuffer = FloatArray(100) { i ->
        when {
            i < 20 -> 0.0f  // Silence
            i < 80 -> 0.5f  // Audio
            else -> 0.0f    // Silence
        }
    }
    val silenceRegions = processor.findSilenceRegions(testBuffer, 0.01f, 10)
    for (region in silenceRegions) {
        println("  Silence: ${region.startIndex} - ${region.endIndex}")
    }
    
    // Test state machine
    println("\nState Machine:")
    val stateMachine = PlayerStateMachine()
    stateMachine.addListener { state ->
        println("  State changed to: ${state::class.simpleName}")
    }
    
    stateMachine.handleEvent(PlayerEvent.Load("test.flac"))
    stateMachine.handleEvent(PlayerEvent.Play(0, 300000))
    stateMachine.handleEvent(PlayerEvent.Pause(50000))
    stateMachine.handleEvent(PlayerEvent.Play(50000, 300000))
    stateMachine.handleEvent(PlayerEvent.Complete())
    
    println("\nFinal UI State: ${stateMachine.getUiState().statusText}")
}
