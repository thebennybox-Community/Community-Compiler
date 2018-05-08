package dusk.ilc.emitter

import dusk.ilc.program.ProgramData
import java.io.OutputStream

interface Emitter {
	fun emit(data: ProgramData, target: OutputStream)
}