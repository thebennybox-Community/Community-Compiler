package dusk.ilc.util

fun String.escape(): String {
	val result = StringBuilder()

	for(c in this) {
		val escaped = "tbnrf\'\"\\"
		val unescaped = "\t\b\n\r\u000C\'\"\\"
		when {
//			c in basics -> result.append('\\').append(c)
			c in unescaped -> {
				result.append('\\').append(escaped[unescaped.indexOf(c)])
			}
			c <= 31.toChar() -> result.append("\\u").append(String.format("%04X", c.toInt()))
			else -> result.append(c)
		}
	}

	return result.toString()
}

//fun String.unescape(): String {
//	val result = StringBuilder()
//
//	val escaped = "tbnrf\'\"\\"
//	val unescaped = "\t\b\n\r\u000C\'\"\\"
//
////	for((a, b) in zipWithNext()) {
//
//	var i = 0
//	while(i in indices) {
//		val a = this[i++]
//		val b = this[i]
//
//		if(a == '\\' && b in escaped) {
//			val escapedIndex = escaped.indexOf(b)
//			result.append(unescaped[escapedIndex])
////			val i = escaped.indexOf(b)
//		}
//	}
//
//	return result.toString()
//}

fun String.unescape(): String {

	val sb = StringBuilder(this.length)

	var i = 0
	while (i < this.length) {
		var ch = this[i]
		if (ch == '\\') {
			val nextChar = if (i == this.length - 1)
				'\\'
			else
				this[i + 1]
			// Octal escape?
			if (nextChar in '0'..'7') {
				var code = "" + nextChar
				i++
				if (i < this.length - 1 && this[i + 1] >= '0' && this[i + 1] <= '7') {
					code += this[i + 1]
					i++
					if (i < this.length - 1 && this[i + 1] >= '0' && this[i + 1] <= '7') {
						code += this[i + 1]
						i++
					}
				}
				sb.append(Integer.parseInt(code, 8).toChar())
				i++
				continue
			}
			when (nextChar) {
				'\\' -> ch = '\\'
				'b' -> ch = '\b'
//				'f' -> ch = '\f'
				'n' -> ch = '\n'
				'r' -> ch = '\r'
				't' -> ch = '\t'
				'\"' -> ch = '\"'
				'\'' -> ch = '\''
				'u' -> {
					if (i >= this.length - 5) {
						ch = 'u'
					}else {
						val code = Integer.parseInt(
								"" + this[i + 2] + this[i + 3]
										+ this[i + 4] + this[i + 5], 16)
						sb.append(Character.toChars(code))
						i += 5
						i++
					}
				}
			}
			i++
		}
		sb.append(ch)
		i++
	}
	return sb.toString()
}