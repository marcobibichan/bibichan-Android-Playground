// File: CommandLineArgs.kt
// Purpose: Demonstrate command-line argument handling
// Run:     kotlinc CommandLineArgs.kt -include-runtime -d Args.jar
//          java -jar Args.jar arg1 arg2 arg3

/**
 * Main function with command-line arguments.
 * args: Array of strings passed from command line.
 */
fun main(args: Array<String>) {
    // Check if arguments were provided
    if (args.isEmpty()) {
        println("Usage: program <arguments>")
        println("Example: program arg1 arg2 arg3")
        return
    }
    
    // Print all arguments
    println("Number of arguments: ${args.size}")
    
    // Using indices
    for (i in args.indices) {
        println("args[$i] = ${args[i]}")
    }
    
    // Using withIndex
    for ((index, value) in args.withIndex()) {
        println("Argument $index: $value")
    }
    
    // Using forEach
    args.forEach { arg ->
        println("Argument: $arg")
    }
    
    // Using forEach with implicit it
    args.forEach {
        println("Arg: $it")
    }
}
