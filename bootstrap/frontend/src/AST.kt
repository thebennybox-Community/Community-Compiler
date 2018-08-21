package dusk.compiler

abstract class ASTNode(val parent: ASTNode?)

class ASTCodeBlock(parent: ASTNode?, val name: String?, val children: List<ASTNode>) : ASTNode(parent)

class ASTFunction(parent: ASTCodeBlock, val name: String, val affixPos: int, val returnType: ASTType, parameters: List<ASTParameter>) : ASTNode(parent)

class ASTParameter(parent: ASTFunction, val name: String, val type: ASTType) : ASTNode(parent)

class ASTType(parent: ASTNode, val name: String) : ASTNode(parent) {
    override fun equals(other: Any?) = (other as? ASTType)?.name == name
    override fun hashCode() = name.hashCode()
}

class ASTVariable(parent: ASTNode, val name: String, val type: ASTType, val constant: Boolean, val initialization: ASTExpression?) : ASTNode(parent)

class ASTReturnStatement(parent: ASTNode, val expression: ASTExpression) : ASTNode(parent)

class ASTIfStatement(parent: ASTNode, val expression: ASTExpression, val codeBlock: ASTCodeBlock, val elseCodeBlock: ASTCodeBlock?) : ASTNode(parent)

class ASTLoopStatement(parent: ASTNode, val expression: ASTExpression, val codeBlock: ASTCodeBlock) : ASTNode(parent)

abstract class ASTExpression(parent: ASTNode) : ASTNode(parent) {
    val returnType by lazy(::resolveReturnType)

    protected abstract fun resolveReturnType(): ASTType
}

class ASTFunctionCall(parent: ASTNode, functionName: String, parameterExpressions: List<ASTExpression>) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        TODO("not implemented")
    }
}

class ASTCodeBlockExpression(parent: ASTNode, val children: List<ASTNode>) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        TODO("not implemented") 
        
        
    }

}

class ASTIfExpression(parent: ASTNode, val expression: ASTExpression, codeBlock: ASTExpression, elseCodeBlock: ASTExpression) : ASTNode(parent)

class ASTStringLiteral(parent: ASTNode, val stringValue: String) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        return ASTType(this, "str")
    }
}

class ASTBooleanLiteral(parent: ASTNode, val booleanValue: Boolean) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        return ASTType(this, "bool")
    }
}

class ASTNumberLiteral(parent: ASTNode, val numberValue: Long, val signed: Boolean, val size: Int?) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        return ASTType(this, (if(signed) "i" else "u") + (size ?: "32"))
    }
}

class ASTDecimalLiteral(parent: ASTNode, val numberValue: Double, val size: Int?) : ASTExpression(parent) {
    override fun resolveReturnType(): ASTType {
        return ASTType(this, "f" + (size ?: "32"))
    }
}
