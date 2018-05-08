package dusk.ilc.opcodes

object OpCodes {

	val codes: Array<out OpCode> = arrayOf(
			//			0x0   0x1   0x2   0x3   0x4   0x5   0x6   0x7   0x8   0x9   0xA   0xB   0xC   0xD   0xE   0xF
			/* 0x0 */   PU08, PU16, PU32, PU64, PI08, PI16, PI32, PI64, PF32, PF64, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x1 */   PTRU, PFLS, PSTR, PFUN, PLBL, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x2 */   CAST, DELE, SWAP, DUPE, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x3 */   CMPE, CMPG, CPGE, CMPL, CPLE, CPNE, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x4 */   FUNC, RETN, CALL, CALS, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x5 */   LABL, JUMP, JEQZ, JNEZ, JGTZ, JGEZ, JLTZ, JLEZ, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x6 */   LLOC, SLOC, ADRL, LARG, SARG, ADRA, LGLO, SGLO, ADRG, READ, WRIT, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x7 */   IADD, ISUB, IMUL, IDIV, INEG, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x8 */   FADD, FSUB, FMUL, FDIV, FNEG, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0x9 */   BSHL, BSHR, BAND, BWOR, BXOR, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xA */   NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xB */   NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xC */   NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xD */   NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xE */   EXFN, INFN, FPRM, FLOC, GLOB, DATA, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP,
			/* 0xF */   NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP, NOOP
	)

	val nameMap = codes.map { it.name.toLowerCase() to it }.toMap()

	// ------- Push Numeric -------
	abstract class Push(id: Int, type: Type<*>): OpCode(id, type)

	object PU08 : Push(0x00, TypeUnsigned8)
	object PU16 : Push(0x01, TypeUnsigned16)
	object PU32 : Push(0x02, TypeUnsigned32)
	object PU64 : Push(0x03, TypeUnsigned64)

	object PI08 : Push(0x04, TypeInt8)
	object PI16 : Push(0x05, TypeInt16)
	object PI32 : Push(0x06, TypeInt32)
	object PI64 : Push(0x07, TypeInt64)

	object PF32 : Push(0x08, TypeFloat32)
	object PF64 : Push(0x09, TypeFloat64)

	// ------- Push Misc -------
	object PTRU : Push(0x10, TypeVoid)
	object PFLS : Push(0x11, TypeVoid)
	object PSTR : Push(0x12, TypeString)
	object PFUN : Push(0x13, TypeIdentifier)
	object PLBL : Push(0x14, TypeIdentifier)

	// ------- Stack Operations -------
	object CAST : OpCode(0x20, TypeType)
	object DELE : OpCode(0x21)
	object SWAP : OpCode(0x22)
	object DUPE : OpCode(0x23)

	// ------- Comparison -------
	object CMPE : OpCode(0x30)

	object CMPG : OpCode(0x31)
	object CPGE : OpCode(0x32)

	object CMPL : OpCode(0x33)
	object CPLE : OpCode(0x34)

	object CPNE : OpCode(0x35)

	// ------- Functions -------
	object FUNC : OpCode(0x40, TypeIdentifier)
	object RETN : OpCode(0x41)
	object CALL : OpCode(0x42, TypeIdentifier)
	object CALS : OpCode(0x43, TypeType, TypeTypeArray)

	// ------- Labels -------
	object LABL : OpCode(0x50, TypeIdentifier)

	object JUMP : OpCode(0x51, TypeIdentifier)

	object JEQZ : OpCode(0x52, TypeIdentifier)
	object JNEZ : OpCode(0x53, TypeIdentifier)

	object JGTZ : OpCode(0x54, TypeIdentifier)
	object JGEZ : OpCode(0x55, TypeIdentifier)

	object JLTZ : OpCode(0x56, TypeIdentifier)
	object JLEZ : OpCode(0x57, TypeIdentifier)

	// ------- Memory Operations -------
	abstract class LoadStore(id: Int): OpCode(id, TypeIdentifier)

	object LLOC : LoadStore(0x60)
	object SLOC : LoadStore(0x61)
	object ADRL : LoadStore(0x62)

	object LARG : LoadStore(0x63)
	object SARG : LoadStore(0x64)
	object ADRA : LoadStore(0x65)

	object LGLO : LoadStore(0x66)
	object SGLO : LoadStore(0x67)
	object ADRG : LoadStore(0x68)

	object READ : OpCode(0x69)
	object WRIT : OpCode(0x6A)

	// ------- Integer Arithmetic -------
	object IADD : OpCode(0x70)
	object ISUB : OpCode(0x71)
	object IMUL : OpCode(0x72)
	object IDIV : OpCode(0x73)
	object IMOD : OpCode(0x74)
	object INEG : OpCode(0x75)

	// ------- Float Arithmetic -------
	object FADD : OpCode(0x80)
	object FSUB : OpCode(0x81)
	object FMUL : OpCode(0x82)
	object FDIV : OpCode(0x83)
	object FMOD : OpCode(0x84)
	object FNEG : OpCode(0x85)

	// ------- Bitwise -------
	object BSHL : OpCode(0x90)
	object BSHR : OpCode(0x91)
	object BAND : OpCode(0x92)
	object BWOR : OpCode(0x93)
	object BXOR : OpCode(0x94)

	// ------- Meta -------
	object EXFN : OpCode(0xE0, TypeIdentifier, TypeType, TypeTypeArray)
	object INFN : OpCode(0xE1, TypeIdentifier, TypeType)
	object FPRM : OpCode(0xE1, TypeIdentifier, TypeIdentifier, TypeType)
	object FLOC : OpCode(0xE2, TypeIdentifier, TypeIdentifier, TypeType)
	object GLOB : OpCode(0xE3, TypeIdentifier, TypeIdentifier)
	object DATA : OpCode(0xE4, TypeIdentifier, TypeByteArray)

	// ------- Reserved -------
	object NOOP : OpCode(0xFF)
}