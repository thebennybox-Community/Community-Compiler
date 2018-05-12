package dusk.ilc.program

import dusk.ilc.opcodes.Type

/**
 * A map of variable name to variable type (for locals, globals, and arguments)
 */
typealias Variables = Map<String, Type<*>>
