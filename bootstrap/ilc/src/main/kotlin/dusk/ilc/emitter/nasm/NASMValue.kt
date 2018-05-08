package dusk.ilc.emitter.nasm

interface NASMValue {
	val text: String

	interface Source: NASMValue
	interface Target: NASMValue

	data class Raw(override val text: String): Source

	data class LiteralInt(val value: Long): Source {
		override val text = value.toString()
	}

	data class LiteralFloat32(val value: Float): Source {
		override val text = "__float32__( $value )"
	}

	data class LiteralFloat64(val value: Double): Source {
		override val text = "__float64__( $value )"
	}
}
