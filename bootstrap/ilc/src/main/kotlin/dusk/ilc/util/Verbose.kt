package dusk.ilc.util

object Verbose {

	var enabled: Boolean
		get() = Flags.verbose
		set(value) { Flags.verbose = value }

	fun println(vararg values: Any?) {
		doIf { System.out.println(values) }
	}

	fun printf(format: String, vararg values: Any?) {
		doIf { System.out.printf(format, *values) }
	}

	inline fun doIf(body: () -> Unit) {
		if(enabled)
			body()
	}

}