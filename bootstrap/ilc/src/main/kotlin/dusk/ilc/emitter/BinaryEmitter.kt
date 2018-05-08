package dusk.ilc.emitter

import dusk.ilc.opcodes.*
import dusk.ilc.program.ProgramData
import java.io.DataOutputStream
import java.io.OutputStream

object BinaryEmitter : Emitter {

	override fun emit(data: ProgramData, target: OutputStream) {
		val out = DataOutputStream(target)

		for (instr in data.instructions)
			writeInstruction(out, instr)
	}

	private fun writeInstruction(data: DataOutputStream, instruction: Instruction) {
		data.write(instruction.opcode.id)
		instruction.args.forEach { writeArgument(data, it) }
	}

	private fun writeString(data: DataOutputStream, string: String) {
		val bytes = string.toByteArray()
		data.writeInt(bytes.size)
		data.write(bytes)
	}

	private fun writeStringArray(data: DataOutputStream, array: Array<String>) {
		data.writeInt(array.size)
		for (str in array)
			writeString(data, str)
	}

	private fun writeTypeArray(data: DataOutputStream, array: Array<out Type<*>>) {
		writeByteArray(data, array.map { it.id.toByte() }.toByteArray())
	}

	private fun writeByteArray(data: DataOutputStream, array: ByteArray) {
		data.writeInt(array.size)
		data.write(array)
	}

	private fun writeArgument(data: DataOutputStream, arg: Argument<*>) {
		when(arg) {
			is ArgInt8 -> data.write(arg.value.toInt())
			is ArgUnsigned8 -> data.write(arg.value.toInt())

			is ArgInt16 -> data.writeShort(arg.value.toInt())
			is ArgUnsigned16 -> data.writeShort(arg.value.toInt())

			is ArgInt32 -> data.writeInt(arg.value)
			is ArgUnsigned32 -> data.writeInt(arg.value)

			is ArgInt64 -> data.writeLong(arg.value)
			is ArgUnsigned64 -> data.writeLong(arg.value)

			is ArgFloat32 -> data.writeFloat(arg.value)
			is ArgFloat64 -> data.writeDouble(arg.value)

			is ArgIdentifier -> writeString(data, arg.value)
			is ArgString -> writeString(data, arg.value)

			is ArgTypeArray -> writeTypeArray(data, arg.value)
			is ArgStringArray -> writeStringArray(data, arg.value)
			is ArgByteArray -> writeByteArray(data, arg.value)
		}
	}

}