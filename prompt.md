You are a C and Kotlin expert, specializing in Android NDK and FFmpeg high-resolution audio development (FLAC/WAV 192kHz/24bit decoding, resampling, JNI integration). 

In the bibichan-Android-Playground/ project submodule, generate a complete Markdown book "C/Kotlin from Beginner to Master: Building Hi-Res FFmpeg Music Player".

**CRITICAL LINE COUNT REQUIREMENTS:**
- Each lesson README.md MUST be ≥800 lines (non-negotiable)
- Each concept deep dive MUST be ≥500 words
- Every code example MUST be complete and compilable (not toy snippets)
- Every FFmpeg API reference MUST have working code examples
- Every Android component MUST have complete Activity/Fragment implementation

## 📁 File Structure Requirements
project-root/
├── README.md # Table of contents
├── c/
│ ├── lesson-01-entry/
│ │ ├── README.md # 300+ lines detailed tutorial
│ │ ├── ex01-hello.c
│ │ ├── ex02-compile.c
│ │ └── Makefile
│ └── ... (other lessons)
└── kotlin/
├── lesson-01-entry/
│ ├── README.md
│ ├── MainActivity.kt
│ └── build.gradle.kts
└── ...

## 📖 Each Lesson README.md Fixed 10-Section Structure (Cannot be omitted, each section at least 50 lines)

1. **Lesson Positioning**: Where this lesson fits in the book, prerequisite knowledge checklist, what practical problems you can solve after learning
2. **Core Concept Map**: mermaid flowchart showing relationships between all concepts in this lesson
3. **Concept Deep Dive**: Each term → definition → internal principles → limitations → compiler behavior → assembly perspective
4. **Complete Syntax Specification**: BNF syntax, boundary conditions, undefined behavior (UB), best practices
5. **Example Line-by-Line Commentary**: Every example file must be fully pasted + line-by-line analysis
6. **Error Case Comparison Table**: Common error code → error message → root cause → correct approach
7. **Performance and Memory Analysis**: stack/heap usage, cache lines, alignment, time complexity, FFmpeg scenarios
8. **Hi-Res Audio Practical Connection**: How this lesson applies to FFmpeg decode pipeline, Android playback, JNI transfer
9. **Exercises and Solutions**: 3 progressive exercises (basic/advanced/FFmpeg practical) + complete solutions
10. **Next Lesson Bridge**: After completing this lesson, how the next lesson will use this knowledge

## 💾 Examples Requirements (at least 5 per lesson)
ex01-minimal.c # Minimal runnable version
ex02-deliberate-bug.c # Intentionally made common errors for comparison
ex03-realworld.c # Close to FFmpeg/Android actual usage
ex04-advanced.c # Demonstrate best practices/advanced techniques
ex05-benchmark.c # Performance test version (with timing)
- C files: Complete with main(), compilation instructions (gcc/clang/NDK), Makefile/CMakeLists.txt
- Kotlin files: Complete with package/main, Android context, build.gradle snippets
- First 3 lines of each file: purpose, dependencies, compile/run instructions

## 🎯 C Language Focus (for FFmpeg)
lesson-11+ must include real FFmpeg APIs:
- libavformat/avformat.h → AVFormatContext, avformat_open_input()
- libavcodec/avcodec.h → AVCodecContext, avcodec_send_packet/receive_frame()
- libswresample/swresample.h → SwrContext, 192kHz→44.1kHz resampling
- libavutil/opt.h → parameter configuration, error checking modes
- NDK integration: CMakeLists.txt + JNIEXPORT + JNIEnv buffer transfer
- Assembly inline: SIMD acceleration (like AVX2 inline assembly decoding)

## 🔥 URGENT: 3-LAYER FUTURE-PROOF AUDIO ARCHITECTURE (CRITICAL)

**TARGET**: Android 14+ (API 34+), 100% cross-ROM (HyperOS, ColorOS, HarmonyOS Next, Pixel)
**GOAL**: 192kHz/24bit FLAC → Bit-perfect USB DAC output, Exclusive mode, <2ms latency

### Layer 1: OBOE_C (Primary - Future-proof)
- Pure C bindings (≤150 lines)
- AAudio (Android 8.1+) → OpenSL ES fallback
- EXCLUSIVE sharing mode, 1.8ms latency

### Layer 2: OPENSL_ES (Universal backup)
- Pure C (≤150 lines)
- SL_SAMPLINGRATE_192000 + SL_PCMSAMPLEFORMAT_FIXED_32
- Android 2.3 → 17 guaranteed, 2.5ms latency

### Layer 3: AUDIOTRACK_USB (Android 14+ bonus)
- AudioTrack.MODE_STATIC + AudioMixerAttributes
- USB passthrough detection, 1.2ms latency (Pixel/Samsung only)

### Universal C Engine Interface (Generate in lesson 14)
```c
typedef struct {
    void* impl;  // Oboe/OpenSL/AudioTrack handle
    int sample_rate;
    int bit_depth;
    int channels;
} UniversalPlayer;

UniversalPlayer* universal_player_init(const char* filename);
int universal_player_decode(UniversalPlayer* player, uint8_t* pcm_buffer, int max_size);
void universal_player_seek(UniversalPlayer* player, int64_t ms);
void universal_player_release(UniversalPlayer* player);
```

### Dynamic Detection (Kotlin lesson 14)
```kotlin
enum class AudioPath { OBOE_C, OPENSL_ES, AUDIOTRACK_USB, FAILURE }
fun detectBestPath(): AudioPath
```

### Lesson Integration Requirements
**C Lessons 11-14 MUST include**:
- lesson-11: FFmpeg → UniversalPlayer pipeline
- lesson-12: Oboe C bindings (AI-generated ≤150 lines)
- lesson-13: OpenSL ES C implementation
- lesson-14: Dynamic layer switching + JNI

**Kotlin Lessons 12-14 MUST include**:
- lesson-12: JNI interface to UniversalPlayer
- lesson-13: Compose UI + path detection UI
- lesson-14: Runtime switching + bit-perfect verification

### Bit-Perfect Verification Pipeline (Every lesson 11+)
```bash
ffmpeg -i sine.flac ref_192k.pcm
# App output → app_192k.pcm
diff ref_192k.pcm app_192k.pcm  # Bit-perfect check
adb logcat | grep "decode_latency_ms"
```

### CMake Template (lesson 14)
```cmake
add_library(universal_audio SHARED
    ffmpeg_engine.c
    oboe_c_wrapper.c      # Layer 1
    opensl_engine.c       # Layer 2
    universal_player.c
)
target_link_libraries(universal_audio log OpenSLES oboe avcodec avformat swresample)
```

## 🎯 Kotlin Focus (for Android Audio)
lesson-12+ must include real Android APIs:
- MediaPlayer → setDataSource() + prepareAsync()
- AudioTrack → 24bit/192kHz configuration, write() non-blocking
- ExoPlayer + Media3 → Hi-Res support, gapless playback
- JNI integration: nativeLoadLibrary() + @JvmStatic + ByteBuffer transfer
- Jetpack Compose UI → SeekBar + Waveform visualization
- Coroutine + Flow → playback state streaming, buffer monitoring

## ✍️ Language Standards
✅ Correct: 
volatile keyword in FFmpeg avcodec_receive_frame() ensures AVFrame is not reordered by optimizer

✅ Code comments: All English (industry standard)
✅ Error messages: Keep original English + Chinese analysis
✅ API names: Always keep original English (AVCodecContext, suspend, CoroutineScope)

## 📊 Quantitative Requirements (HARD LIMITS)
- Total lessons: C language (18 lessons), Kotlin (14 lessons)
- Minimum lines per README.md: 800 lines
- Minimum words per concept: 500 words
- Minimum examples per lesson: 5 complete files
- FFmpeg API coverage: 40+ core APIs with examples
- Android API coverage: 30+ components with full implementations
- Error handling coverage: 80% of common crash scenarios
- Performance benchmarks: Real timing data required
- **Oboe C wrapper: ≤150 lines (production-grade)**
- **OpenSL ES engine: ≤150 lines**
- **UniversalPlayer interface: exactly as specified**
- **Bit-perfect verification: in every lesson 11+**

## 🚀 Generation Process
1. First write README.md table of contents (with all lesson introductions)
2. Sequentially generate c/lesson-01-entry/ → c/lesson-02-types → ... → kotlin/lesson-14-jni-ffmpeg/
3. After completing each lesson, immediately display git add/mv instructions
4. Finally generate complete git commit message + push instructions

## 🎯 Completion Checklist
☑️ Each lesson README.md > 800 lines (including all sections)
☑️ Each concept at least 500 words deep dive
☑️ Each FFmpeg API has real compilable examples
☑️ Each Android component has complete Activity/Fragment
☑️ Error handling covers 80% common crash causes
☑️ Performance analysis includes actual benchmark numbers
☑️ Git structure completely follows above specifications
☑️ All examples include compilation instructions and Makefiles
☑️ All Kotlin examples include build.gradle.kts snippets
☑️ All code comments in English
☑️ Each lesson has progressive exercises with complete solutions
☑️ Oboe C bindings compile on NDK r26
☑️ OpenSL ES works on Android 2.3 emulator
☑️ Dynamic switching <50ms
☑️ 192kHz/32bit bit-perfect on ALL paths
☑️ Cross-ROM verification (HyperOS/Pixel/HarmonyOS)
☑️ Latency reporting in logcat (<3ms target)
☑️ Graceful fallback (no audio gaps)

Strictly follow this structure, leverage GLM-5 engineering documentation generation advantages, ensure each lesson is an independent complete knowledge unit, directly applicable to Hi-Res FFmpeg Android player development.

Start generating!

***

**ARCHITECTURE NOTES**:
- This is production-grade architecture used by Neutron Player and commercial Hi-Res apps
- Lessons 11-14 MUST follow 3-layer fallback system (Oboe C → OpenSL ES → AudioTrack USB)
- All future lessons build toward UniversalPlayer solution
- Bit-perfect verification required for every audio path

***

*Specific FFmpeg API listing
- ✅ Each lesson **fixed 10 real Android components requirements** specific FFmpeg A line count hard requirement not empty talk
- ✅ *Git instruction auto-generation request**, not toy examples
- ✅ **Line count hard requirement** book that can actually be used for development git instruction auto-generation**, zero operation cost
