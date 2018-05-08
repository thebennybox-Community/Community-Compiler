package dusk.ilc.emitter

import dusk.ilc.program.ProgramData
import java.io.OutputStream

object TextEmitter : Emitter {

	override fun emit(data: ProgramData, target: OutputStream) {
		target.bufferedWriter().use { out ->
			data.instructions.forEach { instr ->
				out.appendln(instr.toString())
//				if(i.opcode != OpCodes.LABL) {
//					it.append('\t').appendln(i.toString())
//				}else{
//					if(1 in i.args.indices && (i.args[1].value as? Array<*>)?.isEmpty() != null) {
//						it.appendln("${i.args[0]}:")
//					}else {
//						it.appendln("${i.args.joinToString(" ")}:")
//					}
//				}
			}
		}
	}

}