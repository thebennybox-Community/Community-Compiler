package dusk.compiler

fun main(args: Array<String>) {
    var lexer = Lexer()

    val tokens = lexer.lex("""
extern printf(str x, ...);

fn main(args: str[]) {
    var srcdirs: list = ["src"];
    var srcdir_declared = false;
    loop(arg in args) {
        if(arg == "") {
            continue;
        } else {
            if(!srcdir_declared) {
                srcdirs[0] = arg;
                srcdir_declared = true;
            } else {
                srcdirs.add(arg);
            }
        }
    }
    printf("%s", srcdirs.toStr());
}

0.2
0.2f32
2u32
2i64
2f32
2.2u64
0x1_Bfu32
0b10_01i6
0u644
0u32t
'c'
'cc'
'\\'
'\u0414'
'\uABrE'
'ö'
'\U0001F926'
"test"
""")

    for(token in tokens) {
        println("${token.line}:${token.column} ${token.kind} ${token.text}")
    }
}
