package dusk.ilc.util

import dusk.ilc.opcodes.*

class InstructionList(val instructions: MutableList<Instruction> = ArrayList()) : MutableList<Instruction> by instructions {

	fun add(code: OpCode, vararg args: Argument<*>) {
		add(Instruction(code, args.toList()))
	}

	fun addNoOperation() = add(OpCodes.NOOP)

	fun addPushU8(value: Byte) = add(OpCodes.PU08, ArgUnsigned8(value))
	fun addPushU16(value: Short) = add(OpCodes.PU16, ArgUnsigned16(value))
	fun addPushU32(value: Int) = add(OpCodes.PU32, ArgUnsigned32(value))
	fun addPushU64(value: Long) = add(OpCodes.PU64, ArgUnsigned64(value))
	fun addPushI8(value: Byte) = add(OpCodes.PI08, ArgInt8(value))
	fun addPushI16(value: Short) = add(OpCodes.PI16, ArgInt16(value))
	fun addPushI32(value: Int) = add(OpCodes.PI32, ArgInt32(value))
	fun addPushI64(value: Long) = add(OpCodes.PI64, ArgInt64(value))
	fun addPushF32(value: Float) = add(OpCodes.PF32, ArgFloat32(value))
	fun addPushF64(value: Double) = add(OpCodes.PF64, ArgFloat64(value))

	fun addPushTrue() = add(OpCodes.PTRU)
	fun addPushFalse() = add(OpCodes.PFLS)
	fun addPushStr(value: String) = add(OpCodes.PSTR, ArgString(value))
	fun addPushFunction(name: String) = add(OpCodes.PFUN, ArgIdentifier(name))
	fun addPushLabel(name: String) = add(OpCodes.PLBL, ArgIdentifier(name))

	fun addCast(type: Type<*>) = add(OpCodes.CAST, ArgType(type))
	fun addDelete() = add(OpCodes.DELE)
	fun addSwap() = add(OpCodes.SWAP)
	fun addDuplicate() = add(OpCodes.DUPE)

	fun addCompareEqual() = add(OpCodes.CMPE)
	fun addCompareGreaterThan() = add(OpCodes.CMPG)
	fun addCompareGreaterorEqual() = add(OpCodes.CPGE)
	fun addCompareLessThan() = add(OpCodes.CMPL)
	fun addCompareLessorEqual() = add(OpCodes.CPLE)

	fun addFunction(name: String) = add(OpCodes.FUNC, ArgIdentifier(name))
	fun addReturn() = add(OpCodes.RETN)
	fun addCall(name: String) = add(OpCodes.CALL, ArgIdentifier(name))
	fun addCallStack(returnType: Type<*>, vararg types: Type<*>) = add(
			OpCodes.CALS,
			ArgIdentifier(returnType.name),
			ArgTypeArray(types)
	)

	fun addLabel(name: String) = add(OpCodes.LABL, ArgIdentifier(name))
	fun addJump(name: String) = add(OpCodes.JUMP, ArgIdentifier(name))
	fun addJumpEqualZero(name: String) = add(OpCodes.JEQZ, ArgIdentifier(name))
	fun addJumpNotEqualZero(name: String) = add(OpCodes.JNEZ, ArgIdentifier(name))
	fun addJumpGreaterThanZero(name: String) = add(OpCodes.JGTZ, ArgIdentifier(name))
	fun addJumpGreaterEqualZero(name: String) = add(OpCodes.JGEZ, ArgIdentifier(name))
	fun addJumpLessThanZero(name: String) = add(OpCodes.JLTZ, ArgIdentifier(name))
	fun addJumpLessEqualZero(name: String) = add(OpCodes.JLEZ, ArgIdentifier(name))

	fun addLoadLocal(name: String) = add(OpCodes.LLOC, ArgIdentifier(name))
	fun addStoreLocal(name: String) = add(OpCodes.SLOC, ArgIdentifier(name))
	fun addAddressLocal(name: String) = add(OpCodes.ADRL, ArgIdentifier(name))
	fun addLoadArgument(name: String) = add(OpCodes.LARG, ArgIdentifier(name))
	fun addStoreArgument(name: String) = add(OpCodes.SARG, ArgIdentifier(name))
	fun addAddressArgument(name: String) = add(OpCodes.ADRA, ArgIdentifier(name))
	fun addLoadGlobal(name: String) = add(OpCodes.LGLO, ArgIdentifier(name))
	fun addStoreGlobal(name: String) = add(OpCodes.SGLO, ArgIdentifier(name))
	fun addAddressGlobal(name: String) = add(OpCodes.ADRG, ArgIdentifier(name))
	fun addRead() = add(OpCodes.READ)
	fun addWrite() = add(OpCodes.WRIT)

	fun addIntegerAdd() = add(OpCodes.IADD)
	fun addIntegerSubtract() = add(OpCodes.ISUB)
	fun addIntegerMultiply() = add(OpCodes.IMUL)
	fun addIntegerDivide() = add(OpCodes.IDIV)
	fun addIntegerRemainder() = add(OpCodes.IMOD)
	fun addIntegerNegate() = add(OpCodes.INEG)

	fun addFloatAdd() = add(OpCodes.FADD)
	fun addFloatSubtract() = add(OpCodes.FSUB)
	fun addFloatMultiply() = add(OpCodes.FMUL)
	fun addFloatDivide() = add(OpCodes.FDIV)
	fun addFloatRemainder() = add(OpCodes.FMOD)
	fun addFloatNegate() = add(OpCodes.FNEG)

	fun addBitwiseLeftShift() = add(OpCodes.BSHL)
	fun addBitwiseRightShift() = add(OpCodes.BSHR)
	fun addBitwiseAnd() = add(OpCodes.BAND)
	fun addBitwiseOr() = add(OpCodes.BWOR)
	fun addBitwiseXor() = add(OpCodes.BXOR)

//	fun addPushU8(value: Int) = add(OpCodes.PU8_, ArgUnsigned8(value.toByte()))
//	fun addPushU16(value: Int) = add(OpCodes.PU16, ArgUnsigned16(value.toShort()))
//	fun addPushU32(value: Long) = add(OpCodes.PU32, ArgUnsigned32(value.toInt()))
//	fun addPushU64(value: Long) = add(OpCodes.PU64, ArgUnsigned64(value))
//
//	fun addPushI8(value: Int) = add(OpCodes.PI8_, ArgUnsigned8(value.toByte()))
//	fun addPushI16(value: Int) = add(OpCodes.PI16, ArgUnsigned16(value.toShort()))
//	fun addPushI32(value: Long) = add(OpCodes.PI32, ArgUnsigned32(value.toInt()))
//	fun addPushI64(value: Long) = add(OpCodes.PI64, ArgUnsigned64(value))
//
//	fun addPushF32(value: Float) = add(OpCodes.PF32, ArgFloat32(value))
//	fun addPushF64(value: Double) = add(OpCodes.PF32, ArgFloat64(value))
//
//	fun addPushString(value: String) = add(OpCodes.PSTR, ArgString(value))
//	fun addPushBoolean(value: Boolean) = add(if(value) OpCodes.PTRU else OpCodes.PFLS)
//
//	fun addPushFunc(value: String) = add(OpCodes.PFN_, ArgIdentifier(value))
//
//	fun addDelete() = add(OpCodes.POP_)
//	fun addCopy() = add(OpCodes.COPY)
//
//	fun addLoadLocal(offset: Int) = add(OpCodes.LLOC, ArgUnsigned32(offset))
//	fun addStoreLocal(offset: Int) = add(OpCodes.SLOC, ArgUnsigned32(offset))
//	fun addLoadArg(offset: Int) = add(OpCodes.LARG, ArgUnsigned32(offset))
//	fun addStoreArg(offset: Int) = add(OpCodes.SARG, ArgUnsigned32(offset))
//	fun addAddressLocal(offset: Int) = add(OpCodes.ADRL, ArgUnsigned32(offset))
//	fun addAddressArg(offset: Int) = add(OpCodes.ADRA, ArgUnsigned32(offset))
//
//	fun addAdd() = add(OpCodes.ADD_)
//	fun addSubtract() = add(OpCodes.SUB_)
//	fun addMultiply() = add(OpCodes.MUL_)
//	fun addDivide() = add(OpCodes.DIV_)
//	fun addNegate() = add(OpCodes.NEG_)
//
//	fun addBitwiseShiftLeft() = add(OpCodes.SHL_)
//	fun addBitwiseShiftRight() = add(OpCodes.SHR_)
//	fun addBitwiseAnd() = add(OpCodes.AND_)
//	fun addBitwiseOr() = add(OpCodes.OR__)
//	fun addBitwiseXor() = add(OpCodes.XOR_)
//
//	fun addCompareEqual() = add(OpCodes.IFEQ)
//	fun addCompareGreaterThan() = add(OpCodes.IFGT)
//	fun addCompareLessThan() = add(OpCodes.IFLT)
//
//	fun addCall(name: String, vararg types: Type<*>) = add(OpCodes.CALL, ArgIdentifier(name), ArgTypeArray(types))
//
//	fun addReturn() = add(OpCodes.RET_)
//	fun addLabel(name: String, vararg types: Type<*>) = add(OpCodes.LABL, ArgIdentifier(name), ArgTypeArray(types))
//
//	fun addJump(name: String) = add(OpCodes.JUMP, ArgIdentifier(name))
//	fun addJumpIfZero(name: String) = add(OpCodes.JEZ_, ArgIdentifier(name))
//
//	fun addStackInvoke(vararg types: Type<*>) = add(OpCodes.INVK, ArgTypeArray(types))

}