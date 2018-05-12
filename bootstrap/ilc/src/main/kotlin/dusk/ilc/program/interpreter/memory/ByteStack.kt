package dusk.ilc.program.interpreter.memory

import sun.misc.Unsafe
import java.nio.ByteBuffer
import java.util.Stack

class ByteStack(
		val buffer: ByteBuffer = ByteBuffer.allocate(1024),
		val origin: Int = 0,
		val size: Int = buffer.limit()
) {

	private val sizes = Stack<Byte>()

	var position: Int = origin
		private set

	fun pushI8(value: Byte) {
		val valueSize: Byte = 1
		if(origin + valueSize >= size) stackOverflow()
		buffer.put(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	fun pushI16(value: Short) {
		val valueSize: Byte = 2
		if(origin + valueSize >= size) stackOverflow()
		buffer.putShort(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	fun pushI32(value: Int) {
		val valueSize: Byte = 4
		if(origin + valueSize >= size) stackOverflow()
		buffer.putInt(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	fun pushI64(value: Long) {
		val valueSize: Byte = 8
		if(origin + valueSize >= size) stackOverflow()
		buffer.putLong(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	fun pushF32(value: Float) {
		val valueSize: Byte = 4
		if(origin + valueSize >= size) stackOverflow()
		buffer.putFloat(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	fun pushF64(value: Double) {
		val valueSize: Byte = 8
		if(origin + valueSize >= size) stackOverflow()
		buffer.putDouble(position, value)
		sizes.push(valueSize)
		position += valueSize
	}

	private fun stackOverflow(): Nothing {
		throw StackOverflowError()
	}
}