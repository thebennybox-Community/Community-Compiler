package dusk.ilc.parser

import dusk.ilc.opcodes.*
import dusk.ilc.program.ProgramData
import java.io.DataInputStream
import java.io.InputStream

object BinaryParser: Parser {
	override fun parse(input: InputStream): ProgramData {
		val data = DataInputStream(input)

		val result = ArrayList<Instruction>()

		//TODO: This may exit early
		while(data.available() > 0){
			val instr = readInstruction(data)
			result.add(instr)
		}

		return ProgramData(result)
	}

	private fun readInstruction(data: DataInputStream): Instruction {
		val opcode = OpCodes.codes[data.read()]
		val args = opcode.argumentTypes.map { readArgument(data, it) }

		return Instruction(opcode, args)
	}

	private fun readString(data: DataInputStream): String {
		val bytes = ByteArray(data.readInt())
		data.read(bytes)
		return String(bytes)
	}

	private fun readStringArray(data: DataInputStream): Array<String> {
		return Array(data.readInt()) { readString(data) }
	}

	private fun readByteArray(data: DataInputStream): ByteArray {
		val array = ByteArray(data.readInt())
		data.read(array)
		return array
	}

	private fun readTypeArray(data: DataInputStream): Array<Type<*>> {
		return readByteArray(data).map { types[it.toInt()] }.toTypedArray()
	}

	private fun readArgument(data: DataInputStream, type: Type<*>): Argument<*> {
		return when(type) {
			TypeInt8 -> ArgInt8(data.readByte())
			TypeUnsigned8 -> ArgUnsigned8(data.readByte())

			TypeInt16 -> ArgInt16(data.readShort())
			TypeUnsigned16 -> ArgUnsigned16(data.readShort())

			TypeInt32 -> ArgInt32(data.readInt())
			TypeUnsigned32 -> ArgUnsigned32(data.readInt())

			TypeInt64 -> ArgInt64(data.readLong())
			TypeUnsigned64 -> ArgUnsigned64(data.readLong())

			TypeFloat32 -> ArgFloat32(data.readFloat())
			TypeFloat64 -> ArgFloat64(data.readDouble())

			TypeIdentifier -> ArgIdentifier(readString(data))
			TypeString -> ArgString(readString(data))

			TypeType -> ArgType(types[data.read()])

			TypeStringArray -> ArgStringArray(readStringArray(data))
			TypeTypeArray -> ArgTypeArray(readTypeArray(data))
			TypeByteArray -> ArgByteArray(readByteArray(data))

			else -> ArgVoid
		}
	}
}