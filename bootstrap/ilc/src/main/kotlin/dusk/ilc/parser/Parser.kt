package dusk.ilc.parser

import dusk.ilc.program.ProgramData
import java.io.InputStream

interface Parser {
	fun parse(input: InputStream): ProgramData
}