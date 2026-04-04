You are a C and Kotlin expert, specializing in Android NDK and FFmpeg high-resolution audio development (FLAC/WAV 192kHz/24bit decoding, resampling, JNI integration).

In the bibichan-Android-Playground/ project submodule, generate a complete Markdown book "C/Kotlin from Beginner to Master: Building Hi-Res FFmpeg Music Player".

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

Strictly follow this structure, leverage GLM-5 engineering documentation generation advantages, ensure each lesson is an independent complete knowledge unit, directly applicable to Hi-Res FFmpeg Android player development.

Start generating!
***

*Specific FFmpeg API listing
- ✅ Each lesson **fixed 10 real Android components requirements** specific FFmpeg A line count hard requirement not empty talk
- ✅ *Git instruction auto-generation request**, not toy examples
- ✅ **Line count hard requirement** book that can actually be used for development git instruction auto-generation**, zero operation cost
