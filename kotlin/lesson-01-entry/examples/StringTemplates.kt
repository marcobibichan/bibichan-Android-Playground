// File: StringTemplates.kt
// Purpose: Demonstrate string templates and raw strings
// Run:     kotlinc StringTemplates.kt -include-runtime -d Templates.jar

fun main() {
    // Basic string template
    val name = "Kotlin"
    println("Hello, $name!")
    
    // Expression in template
    val a = 10
    val b = 20
    println("Sum: ${a + b}")
    println("Max: ${if (a > b) a else b}")
    
    // Nested template
    val items = listOf("apple", "banana", "cherry")
    println("Items: ${items.joinToString(", ")}")
    
    // Raw string (multi-line)
    val text = """
        |Hello,
        |Kotlin
        |World!
    """.trimMargin()
    println(text)
    
    // Raw string with template
    val lang = "Kotlin"
    val code = """
        fun main() {
            println("Hello, $lang!")
        }
    """.trimIndent()
    println(code)
    
    // Dollar sign escape
    val price = "Price: \$100"
    println(price)
    
    // String formatting
    val pi = 3.14159265359
    println("Pi: ${"%.2f".format(pi)}")
    println("Pi: %.2f".format(pi))
}

/**
 * Audio configuration example using string templates
 */
fun audioConfigExample() {
    val sampleRate = 192000
    val bitDepth = 24
    val channels = 2
    
    // Formatted output
    println("""
        |Audio Configuration:
        |  Sample Rate: $sampleRate Hz
        |  Bit Depth: $bitDepth bits
        |  Channels: $channels
        |  Data Rate: ${sampleRate * bitDepth * channels / 8} bytes/sec
    """.trimMargin())
}
