package dusk.ilc.parser

import dusk.ilc.opcodes.*
import dusk.ilc.program.ProgramData
import dusk.ilc.util.unescape
import java.io.InputStream

object TextParser : Parser {

	private data class ArgumentParser(val regex: Regex, val converter: (String) -> Argument<*>)

//	val intRegex = Regex("^(-?(0x)?[1-9][0-9]*|0)$")
	val intRegex = Regex("^(-?[1-9][0-9]*|0|-?0x[0-9A-Fa-f]+)$")
	val floatRegex = Regex("^-?([1-9][0-9]*|0)(\\.[0-9]+)?$")

	private val argumentParsers = mapOf(
			TypeInt8 to ArgumentParser(intRegex, { ArgInt8(parseInt(it).toByte()) }),
			TypeInt16 to ArgumentParser(intRegex, { ArgInt16(parseInt(it).toShort()) }),
			TypeInt32 to ArgumentParser(intRegex, { ArgInt32(parseInt(it).toInt()) }),
			TypeInt64 to ArgumentParser(intRegex, { ArgInt64(parseInt(it)) }),

			TypeUnsigned8 to ArgumentParser(intRegex, { ArgUnsigned8(parseInt(it).toByte()) }),
			TypeUnsigned16 to ArgumentParser(intRegex, { ArgUnsigned16(parseInt(it).toShort()) }),
			TypeUnsigned32 to ArgumentParser(intRegex, { ArgUnsigned32(parseInt(it).toInt()) }),
			TypeUnsigned64 to ArgumentParser(intRegex, { ArgUnsigned64(parseInt(it)) }),

			TypeFloat32 to ArgumentParser(floatRegex, { ArgFloat32(it.toFloat()) }),
			TypeFloat64 to ArgumentParser(floatRegex, { ArgFloat64(it.toDouble()) }),

			TypeString to ArgumentParser(Regex("^\"[^\"]*\"$"), { ArgString(it.substring(1, it.length - 1).unescape()) }),
//			TypeIdentifier to ArgumentParser(Regex("^\"[^\"]*\"$"), { ArgString(it.substring(1, it.length - 1)) })
			TypeIdentifier to ArgumentParser(Regex("^[a-zA-Z_]+[a-zA-Z0-9_]*$"), { ArgIdentifier(it) }),
			TypeType to ArgumentParser(Regex("^[a-zA-Z_]+[a-zA-Z0-9_]*$"), { ArgType(typeNameMap[it]!!) })
	)

	private fun parseInt(value: String): Long {

		if('x' !in value)
			return value.toLong()

		return value.replace("0x", "").toLong(16)
	}

	override fun parse(input: InputStream): ProgramData {
		return ProgramData(input.bufferedReader().useLines {
			it
					.filterNot { it.startsWith('#') || it.isBlank() }
					.flatMap { it.split(";").asSequence() }
					.map(String::trim).map(::parseLine)
					.filterNotNull().toList()
		})
	}

	private fun parseLine(line: String): Instruction? {
		val name = line.takeWhile { !it.isWhitespace() }
//		if (name.endsWith(':'))
//			return Instruction(OpCodes.LABL, arrayOf(ArgIdentifier(name.removeSuffix(":"))))

		val code = OpCodes.nameMap[name.toLowerCase().padEnd(4, ' ')] ?: throw IllegalStateException("Unrecognized opcode \"$name\"")

		val argsString = line.removePrefix(name).trim()
		val args = parseArguments(code, argsString)

		return Instruction(code, args)
	}

	private fun parseArguments(code: OpCode, args: String): List<Argument<*>> {
		val remaining = StringBuffer(args)

		val result = code.argumentTypes.map {
			val (length, value) = parseSingleArgument(it, remaining)!!
			remaining.delete(0, length)
			remaining.delete(0, remaining.takeWhile(Char::isWhitespace).length)

			value
		}

		if(remaining.isNotEmpty())
			throw IllegalStateException("Trailing content on instruction: \"$remaining\"")

		return result
	}

	private fun parseSingleArgument(type: Type<*>, string: CharSequence): Pair<Int, Argument<*>>? {
		if(type == TypeVoid)
			return 0 to ArgVoid

		if (type is ArrayType<*>) {
			if(!string.startsWith("["))
				throw IllegalStateException("Expected array but value did not start with `[`: $string")

			val parser = argumentParsers[type.type]!!
			var remaining = string.removePrefix("[")

			val result = ArrayList<Argument<*>>()

			while(remaining.isNotEmpty()) {
				if(remaining.startsWith("]")) {
					remaining = remaining.substring(1).trim()
					break
				}

				val (length, value) = tryParser(parser, remaining)
						?: throw IllegalStateException("Failed to parse \"$string\" using $parser")

				result.add(value)

				remaining = remaining.substring(length).trim()
			}

			return (string.length - remaining.length) to when(type) {
				is TypeTypeArray -> ArgTypeArray(result.map { it.value as Type<*> }.toTypedArray())
				is TypeStringArray -> ArgStringArray(result.map { it.value as String }.toTypedArray())
				is TypeByteArray -> ArgByteArray(result.map { it.value as Byte }.toByteArray())
				else -> throw IllegalStateException("Unrecognized array type: $type")
			}

		} else {
			return tryParser(argumentParsers[type]!!, string)
		}
	}

	private fun tryParser(parser: ArgumentParser, text: CharSequence): Pair<Int, Argument<*>>? {
		val builder = StringBuilder()

		var foundLast = false

		for (i in text.indices) {
			builder.append(text[i])

			val matcher = parser.regex.toPattern().matcher(builder)
			val found = matcher.find()
			val partial = matcher.hitEnd()

			if (found && !partial)
				return i + 1 to parser.converter(builder.toString())

			if (!partial) {
				if (foundLast) {
					val string = builder.substring(0, builder.length - 1)
					return string.length to parser.converter(string)
				}
				throw IllegalStateException("Tried to parse with $parser, but failed on input \"$text\"")
			}

			foundLast = found
		}

		if (foundLast)
			return builder.length to parser.converter(builder.toString())

		throw IllegalStateException("Tried to parse with $parser, but failed on input \"$text\"")
	}
}