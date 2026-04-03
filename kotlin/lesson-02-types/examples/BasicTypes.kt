// File: BasicTypes.kt
// Purpose: Demonstrate Kotlin basic types
// Run:     kotlinc BasicTypes.kt -include-runtime -d BasicTypes.jar

fun main() {
    // Integer types
    val byte: Byte = 127           // 8-bit
    val short: Short = 32767       // 16-bit
    val int: Int = 2147483647      // 32-bit
    val long: Long = 9223372036854775807L  // 64-bit
    
    println("Integer types:")
    println("  Byte: $byte")
    println("  Short: $short")
    println("  Int: $int")
    println("  Long: $long")
    
    // Floating-point types
    val float: Float = 3.14159f    // 32-bit
    val double: Double = 3.141592653589793  // 64-bit
    
    println("\nFloating-point types:")
    println("  Float: $float")
    println("  Double: $double")
    
    // Type inference
    val inferredInt = 42           // Int
    val inferredLong = 42L         // Long
    val inferredDouble = 3.14      // Double
    val inferredFloat = 3.14f      // Float
    
    println("\nType inference:")
    println("  inferredInt: ${inferredInt::class.simpleName}")
    println("  inferredLong: ${inferredLong::class.simpleName}")
    println("  inferredDouble: ${inferredDouble::class.simpleName}")
    println("  inferredFloat: ${inferredFloat::class.simpleName}")
    
    // Number literals
    val decimal = 42
    val hex = 0xFF
    val binary = 0b1010
    val million = 1_000_000
    
    println("\nNumber literals:")
    println("  Decimal: $decimal")
    println("  Hex: $hex")
    println("  Binary: $binary")
    println("  Million: $million")
    
    // Boolean type
    val bool: Boolean = true
    println("\nBoolean: $bool")
    
    // Character type
    val char: Char = 'K'
    println("Character: $char")
    
    // String type
    val string: String = "Kotlin"
    println("String: $string")
}
