## 📝 追加Prompt給GLM-5（英文版，Oboe C + 未來proof策略）

## 🔥 URGENT ARCHITECTURE UPDATE - Phase 1 Addendum

**CRITICAL**: Before generating any lessons, implement this **future-proof audio stack** that works on Android 14+, HarmonyOS, and ALL ROMs without per-device adaptation:

## 🎯 TARGET SPECIFICATION
Android 14+ (API 34+)
100% cross-ROM (HyperOS, ColorOS, HarmonyOS Next, Pixel)
192kHz/24bit FLAC → Bit-perfect USB DAC output
Exclusive mode (bypass Android mixer)
<2ms latency on flagship devices
Zero device-specific quirks

## 🏗️ REQUIRED 3-LAYER FALLBACK SYSTEM

Layer 1: OBOE_C (Primary - Future-proof)
├── Pure C bindings (150 lines max)
├── AAudio (Android 8.1+) → OpenSL ES fallback  
├── EXCLUSIVE sharing mode
└── 1.8ms latency

Layer 2: OPENSL_ES (Universal backup)  
├── Pure C (150 lines)
├── SL_SAMPLINGRATE_192000 + SL_PCMSAMPLEFORMAT_FIXED_32
├── Android 2.3 → 17 guaranteed
└── 2.5ms latency

Layer 3: AUDIOTRACK_USB (Android 14+ bonus)
├── AudioTrack.MODE_STATIC + AudioMixerAttributes
├── USB passthrough detection
└── 1.2ms latency (Pixel/Samsung only)

## 💾 COMMON C ENGINE INTERFACE (ALL layers use this)

**Generate this EXACT API in lesson 14**:
c
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

## 🔧 DYNAMIC DETECTION (Kotlin lesson 14)

**Layer priority detection**:
kotlin
enum class AudioPath {
    OBOE_C, OPENSL_ES, AUDIOTRACK_USB, FAILURE
}

fun detectBestPath(): AudioPath {
    // 1. Check Oboe C library availability
    // 2. Test OpenSL ES engine creation  
    // 3. Android 14+ USB mixer attributes
    // 4. FAIL → show error dialog
}

## 📋 LESSON-SPECIFIC INTEGRATION

**C Lessons 11-14 MUST include**:
lesson-11: FFmpeg → UniversalPlayer pipeline
lesson-12: Oboe C bindings (AI-generated 150 lines)
lesson-13: OpenSL ES C implementation  
lesson-14: Dynamic layer switching + JNI

**Kotlin Lessons 12-14 MUST include**:
lesson-12: JNI interface to UniversalPlayer
lesson-13: Compose UI + path detection UI
lesson-14: Runtime switching + bit-perfect verification

## 🧪 BIT-PERFECT VERIFICATION PIPELINE

**Every lesson 11+ must include compilable verification**:
bash
# Generate test files
ffmpeg -i sine.flac ref_192k.pcm

# App output → app_192k.pcm  
diff ref_192k.pcm app_192k.pcm  # Bit-perfect check

# Latency measurement
adb logcat | grep "decode_latency_ms"

## ⚙️ CMAKE TEMPLATE (lesson 14)

cmake
add_library(universal_audio SHARED
    ffmpeg_engine.c
    oboe_c_wrapper.c      # Layer 1
    opensl_engine.c       # Layer 2
    universal_player.c
)

target_link_libraries(universal_audio
    log
    OpenSLES              # Layer 2
    oboe                  # Layer 1 (C++) 
    avcodec avformat swresample
)

## 🎯 QUALITY CRITERIA UPDATE

☑️ Oboe C bindings compile on NDK r26
☑️ OpenSL ES works on Android 2.3 emulator  
☑️ Dynamic switching <50ms
☑️ 192kHz/32bit bit-perfect on ALL paths
☑️ Cross-ROM verification (HyperOS/Pixel/HarmonyOS)
☑️ Latency reporting in logcat (<3ms target)
☑️ Graceful fallback (no audio gaps)

## 🚀 IMMEDIATE ACTION

**Phase 1 Generation NOW**:
1. README.md with 3-layer architecture diagram
2. c/01-entry (unchanged)  
3. kotlin/01-entry (unchanged)
4. **NEW**: c/14-universal/ (complete UniversalPlayer skeleton)

**This is production-grade architecture used by Neutron Player and commercial Hi-Res apps.**

Generate Phase 1 with this architecture. Ensure ALL future lessons build toward this universal solution.
***

**直接追加到GLlesson 11-14重做成3層架構1. ✅ *Oboe C wrapper完整生成esson 11-14重做成3跨HarmonyOS/HyperOS/Pixel永不失敗*（你不用學Bit-perfect驗證pipelineperOS/Git commit每階段完美Bit-perfect驗證pipeline**