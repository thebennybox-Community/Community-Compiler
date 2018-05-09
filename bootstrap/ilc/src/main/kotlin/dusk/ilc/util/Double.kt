package dusk.ilc.util

import java.nio.ByteBuffer

fun Double.toByteArray(): ByteArray {
	return ByteBuffer.wrap(ByteArray(8)).putDouble(this).array()
}