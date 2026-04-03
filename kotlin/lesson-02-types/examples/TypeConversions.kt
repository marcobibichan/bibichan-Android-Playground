// File: TypeConversions.kt
// Purpose: Demonstrate type conversions in Kotlin
// Run:     kotlinc TypeConversions.kt -include-runtime -d TypeConversions.jar

fun main() {
    // Numeric conversions
    val intVal: Int = 42
    val longVal: Long = intVal.toLong()
    val doubleVal: Double = intVal.toDouble()
    val floatVal: Float = intVal.toFloat()
    
    println("Numeric conversions:")
    println("  Int: $intVal")
    println("  toLong: $longVal")
    println("  toDouble: $doubleVal")
    println("  toFloat: $floatVal")
    
    // String conversions
    val numStr = "192000"
    val num = numStr.toInt()
    val numSafe = numStr.toIntOrNull()
    
    println("\nString to number:")
    println("  String: $numStr")
    println("  toInt: $num")
    println("  toIntOrNull: $numSafe")
    
    // Safe conversion with default
    val invalidStr = "abc"
    val safeNum = invalidStr.toIntOrNull() ?: 0
    println("\nSafe conversion:")
    println("  Invalid string: $invalidStr")
    println("  Default value: $safeNum")
    
    // Smart cast
    val obj: Any = "Kotlin"
    if (obj is String) {
        println("\nSmart cast:")
        println("  obj is String: true")
        println("  Length: ${obj.length}")
    }
    
    // When with smart cast
    val data: Any = 42
    val result = when (data) {
        is String -> "String: $data"
        is Int -> "Int: ${data + 1}"
        is Double -> "Double: ${data * 2}"
        else -> "Unknown type"
    }
    println("\nWhen with smart cast:")
    println("  Result: $result")
    
    // Audio-specific conversions
    val sampleRate = 192000
    val sampleRateStr = sampleRate.toString()
    val parsedRate = sampleRateStr.toInt()
    
    println("\nAudio conversions:")
    println("  Sample rate: $sampleRate")
    println("  As string: $sampleRateStr")
    println("  Parsed back: $parsedRate")
    
    // Byte array conversions
    val text = "Hello"
    val bytes = text.toByteArray()
    val backToString = bytes.toString(Charsets.UTF_8)
    
    println("\nByte array conversions:")
    println("  Text: $text")
    println("  Bytes: ${bytes.toList()}")
    println("  Back to string: $backToString")
}
