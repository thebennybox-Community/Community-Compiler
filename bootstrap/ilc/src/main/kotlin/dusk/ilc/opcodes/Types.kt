package dusk.ilc.opcodes

import dusk.ilc.util.escape

interface Type<out T> {
	val id: Int
	val name: String
	val size: Int
}

abstract class AbstractType<T>(override val id: Int, override val name: String, override val size: Int) : Type<T> {
	override fun toString(): String {
		return name
	}
}

abstract class IntegerType<T>(id: Int, name: String, size: Int) : AbstractType<T>(id, name, size)
abstract class UnsignedType<T>(id: Int, name: String, size: Int) : AbstractType<T>(id, name, size)
abstract class FloatType<T>(id: Int, name: String, size: Int) : AbstractType<T>(id, name, size)
abstract class StringType(id: Int, name: String, size: Int) : AbstractType<String>(id, name, size)
abstract class ArrayType<T>(id: Int, name: String, val type: Type<*>) : AbstractType<T>(id, name, 4)

abstract class Argument<T>(val type: Type<T>, val value: T) {
	override fun toString(): String {
		if (value == null)
			return ""
		if (value is String && type == TypeString)
			return "\"${value.escape()}\""
		if (value is Array<*>)
			return value.joinToString(" ", "[", "]") { it.toString().escape() }
		if (value is ByteArray)
			return value.joinToString(" ", "[", "]") { "0x"+(it.toInt() and 0xFF).toString(16).toUpperCase() }
		return value.toString()
	}
}

///////////////////////////////////////////////////////////////////////

val types = listOf<Type<*>>(
		/* 0x0 */ TypeUnsigned8,
		/* 0x1 */ TypeUnsigned16,
		/* 0x2 */ TypeUnsigned32,
		/* 0x3 */ TypeUnsigned64,
		/* 0x4 */ TypeInt8,
		/* 0x5 */ TypeInt16,
		/* 0x6 */ TypeInt32,
		/* 0x7 */ TypeInt64,
		/* 0x8 */ TypeFloat32,
		/* 0x9 */ TypeFloat64,
		/* 0xA */ TypeString,
		/* 0xB */ TypeIdentifier,
		/* 0xC */ TypeVoid,
		/* 0xD */ TypeVoid,
		/* 0xE */ TypeVoid,
		/* 0xF */ TypeVoid,
		/* 0x? */ TypeStringArray, TypeTypeArray, TypeByteArray, TypeType
)
val typeNameMap = types.map { it.name to it }.toMap()

object TypeUnsigned8 		: UnsignedType<Byte>		(0x0, "u8", 1)
object TypeUnsigned16 		: UnsignedType<Short>		(0x1, "u16", 2)
object TypeUnsigned32 		: UnsignedType<Int>			(0x2, "u32", 4)
object TypeUnsigned64 		: UnsignedType<Long>		(0x3, "u64", 8)

object TypeInt8 			: IntegerType<Byte>			(0x4, "i8", 1)
object TypeInt16 			: IntegerType<Short>		(0x5, "i16", 2)
object TypeInt32 			: IntegerType<Int>			(0x6, "i32", 4)
object TypeInt64 			: IntegerType<Long>			(0x7, "i64", 8)

object TypeFloat32 			: FloatType<Float>			(0x8, "f32", 4)
object TypeFloat64 			: FloatType<Double>			(0x9, "f64", 8)

object TypeString 			: StringType				(0xA, "str", 4)
object TypeIdentifier 		: StringType				(0xB, "ptr", 4)

object TypeVoid 			: AbstractType<Nothing?>	(0xF, "void", 0)

object TypeStringArray 		: ArrayType<Array<String>>	(0xFF, "?", TypeString)

object TypeByteArray		: ArrayType<ByteArray>		(0xFE, "?", TypeUnsigned8)
object TypeType		 		: AbstractType<Type<*>>		(0xFD, "?", 1)
object TypeTypeArray 		: ArrayType<Array<Type<*>>>	(0xFC, "?", TypeType)
//object TypeTypeArray 		: ArrayType<Array<String>>	(0xFB, "?", TypeType)

///////////////////////////////////////////////////////////////////////

class ArgUnsigned8(value: Byte) : Argument<Byte>(TypeUnsigned8, value)
class ArgUnsigned16(value: Short) : Argument<Short>(TypeUnsigned16, value)
class ArgUnsigned32(value: Int) : Argument<Int>(TypeUnsigned32, value)
class ArgUnsigned64(value: Long) : Argument<Long>(TypeUnsigned64, value)

class ArgInt8(value: Byte) : Argument<Byte>(TypeInt8, value)
class ArgInt16(value: Short) : Argument<Short>(TypeInt16, value)
class ArgInt32(value: Int) : Argument<Int>(TypeInt32, value)
class ArgInt64(value: Long) : Argument<Long>(TypeInt64, value)

class ArgFloat32(value: Float) : Argument<Float>(TypeFloat32, value)
class ArgFloat64(value: Double) : Argument<Double>(TypeFloat64, value)

class ArgString(value: String) : Argument<String>(TypeString, value)

object ArgVoid : Argument<Nothing?>(TypeVoid, null)

class ArgStringArray(value: Array<String>) : Argument<Array<String>>(TypeStringArray, value)

class ArgIdentifier(value: String) : Argument<String>(TypeIdentifier, value)
class ArgType(value: Type<*>) : Argument<Type<*>>(TypeType, value)

class ArgTypeArray(value: Array<out Type<*>>) : Argument<Array<out Type<*>>>(TypeTypeArray, value) {
	constructor(types: Collection<Type<*>>) : this(types.toTypedArray())
}


class ArgByteArray(value: ByteArray) : Argument<ByteArray>(TypeByteArray, value)
