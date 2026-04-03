// File: HelloWorld.kt
// Purpose: Basic Hello World program in Kotlin
// Run:     kotlinc HelloWorld.kt -include-runtime -d HelloWorld.jar
//          java -jar HelloWorld.jar

// No package declaration needed for simple programs

/**
 * Main entry point of the program.
 * This function is called when the program starts.
 */
fun main() {
    // println: prints a line to standard output
    // String template: $name is replaced with the value
    val language = "Kotlin"
    println("Hello, $language!")
    
    // Expression body function call
    val result = add(10, 20)
    println("10 + 20 = $result")
}

/**
 * Adds two integers and returns the result.
 * This is an expression body function.
 */
fun add(a: Int, b: Int) = a + b
