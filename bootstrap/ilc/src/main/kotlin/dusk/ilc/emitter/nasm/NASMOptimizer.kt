package dusk.ilc.emitter.nasm

object NASMOptimizer {
	fun optimize(list: NASMInstructionList): Int {
		val beforeSize = list.size

		list.removeIf { it is Add && it.b.text == "0" }
		list.removeIf { it is Subtract && it.b.text == "0" }
		list.removeIf { it is Move && it.source == it.target }

		var i = 0
		while(i < list.size - 1) {
			val a = list[i++]
			val b = list[i]

			if(a is Push && b is Pop) {
				list.removeAt(i--)
				list.removeAt(i)

				list.add(i, Move(b.target, a.source))
			}
		}

		return if(beforeSize > list.size)
			1 + optimize(list)
		else
			1
	}
}