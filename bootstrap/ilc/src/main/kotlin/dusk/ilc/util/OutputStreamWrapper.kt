package dusk.ilc.util

import java.io.FilterOutputStream
import java.io.OutputStream

class OutputStreamWrapper(stream: OutputStream): FilterOutputStream(stream) {
	override fun close() {}
}