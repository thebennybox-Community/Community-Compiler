package dusk.ilc.util

fun ByteArray.toUnsignedListString(): String {
	return joinToString(",") {
		(it.toInt() and 0xFF).toString()
	}
}