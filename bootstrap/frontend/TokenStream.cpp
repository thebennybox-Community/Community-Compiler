//
// Created by myvar on 4/29/18.
//

#include "TokenStream.h"

enum class State {
    Start,
    NumberLiteral,
    Symbol
};


bool isSymbolChar(char c) {
    return isalpha(c) || isdigit(c) || c == '_';
}

bool isKeyword(int startingIndex, std::string src, std::string keyword) {
	bool isKeyword = true;

	for (int keywordIndex = 1; keywordIndex < keyword.length(); ++keywordIndex) {
		int srcIndex = startingIndex + keywordIndex;
		char srcChar = src[srcIndex];
		char keywordChar = keyword[keywordIndex];

		isKeyword = isKeyword && (srcChar == keywordChar);

		if (keywordIndex == (keyword.length() - 1))
			isKeyword = isKeyword && !isSymbolChar(src[srcIndex + 1]);

		if (!isKeyword)
			return false;
	}

	return isKeyword;
}

void TokenStream::lex(std::string src) {

    src = " " + src + " ";

    Token tmp;

    State state = State::Start;

    bool inComment = false;
    bool inMulComment = false;
    bool inString = false;

    int col = -1;
    int row = 1;

	// Start at 1 because a space was added earlier.
    for (int i = 1; i < src.length(); ++i) {
        char c = src[i];

        if (c == '\n') {
            row++;
            col = -1;
        }
        col++;

        if (inComment || inMulComment) {
            if (inComment && c == '\n') {
                inComment = false;
                tokens.push_back(tmp);
                tmp = {};
            } else if (inMulComment && c == '*' && src[i + 1] == '/') {
                inMulComment = false;
                tokens.push_back(tmp);
                tmp = {};
                i++;
            } else {
                tmp.raw += c;
            }
        } else {
            if (inString) {
                if (c == '"' && src[i - 1] != '\\') {
                    inString = false;
                    tokens.push_back(tmp);
                    tmp = {};
                } else {
                    tmp.raw += c;
                }
            } else {

                if (c == '/' && src[i + 1] == '/') {
                    inComment = true;
                    i++;
                    tmp = {};
                    tmp.row = row;
                    tmp.column = col;
                    tmp.offset = i;
                    tmp.tokenType = TokenType::SingleLineComment;
                    continue;
                }

                if (c == '/' && src[i + 1] == '*') {
                    inMulComment = true;
                    i++;
                    tmp = {};
                    tmp.row = row;
                    tmp.column = col;
                    tmp.offset = i;
                    tmp.tokenType = TokenType::MultilineComment;
                    continue;
                }

                if (c == '"') {
                    inString = true;
                    tmp = {};
                    tmp.row = row;
                    tmp.column = col;
                    tmp.offset = i;
                    tmp.tokenType = TokenType::StringLiteral;
                    continue;
                }

                // here we can start doing syntax
                switch (state) {
                    case State::Start: {
                        bool aerror = false;
                        if (c == '{') {
                            tokens.push_back({col, row, i, "{", TokenType::OpenCurlyBracket});
                        } else if (c == '}') {
                            tokens.push_back({col, row, i, "}", TokenType::CloseCurlyBracket});
                        } else if (c == ':') {
                            tokens.push_back({col, row, i, ":", TokenType::Colon});
                        } else if (c == '=') {
                            if (src[i + 1] == '=') {
                                tokens.push_back({col, row, i, "==", TokenType::CmpEqual});
                                i++;
                            } else {
                                tokens.push_back({col, row, i, "=", TokenType::Equal});
                            }
                        } else if (c == '>') {
                            tokens.push_back({col, row, i, ">", TokenType::GreaterThan});
                        } else if (c == '<') {
                            tokens.push_back({col, row, i, "<", TokenType::LessThan});
                        } else if (c == '!') {
                            if (src[i + 1] == '=') {
                                tokens.push_back({col, row, i, "!=", TokenType::NotEqual});
                                i++;
                            } else {
                                tokens.push_back({col, row, i, "!", TokenType::Not});
                            }
                        } else if (c == '+') {
                            tokens.push_back({col, row, i, "+", TokenType::Plus});
                        } else if (c == '@') {
                            tokens.push_back({col, row, i, "@", TokenType::At});
                        } else if (c == '-') {
                            tokens.push_back({col, row, i, "-", TokenType::Minus});
                        } else if (c == '/') {
                            tokens.push_back({col, row, i, "/", TokenType::Divide});
                        } else if (c == '*') {
                            tokens.push_back({col, row, i, "*", TokenType::Mul});
                        } else if (c == ';') {
                            tokens.push_back({col, row, i, ";", TokenType::SemiColon});
                        } else if (c == '(') {
                            tokens.push_back({col, row, i, "(", TokenType::OpenBracket});
                        } else if (c == ')') {
                            tokens.push_back({col, row, i, ")", TokenType::CloseBracket});
                        } else if (c == '[') {
                            tokens.push_back({col, row, i, "[", TokenType::OpenSquareBracket});
                        } else if (c == ']') {
                            tokens.push_back({col, row, i, "]", TokenType::CloseSquareBracket});
                        } else if (c == ',') {
                            tokens.push_back({col, row, i, ",", TokenType::Comma});
                        } else if (c == '.') {
                            tokens.push_back({col, row, i, ".", TokenType::Dot});
                        } else if (isdigit(c)) {
                            state = State::NumberLiteral;
                            tmp = {};
                        } else if (isalpha(c) || c == '_') {

                            switch (c) {
                                case 'f':
                                    if (isKeyword(i, src, "false")) {
                                        i += 4;
                                        tokens.push_back({col, row, i, "false", TokenType::Boolean});
                                        tmp = {};
                                    } else if (src[i + 1] == 'n' && !isSymbolChar(src[i + 2])) {
                                        i += 1;
                                        tokens.push_back({col, row, i, "fn", TokenType::Fn});
                                        tmp = {};

                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 't':
                                    if (isKeyword(i, src, "true")) {
                                        i += 3;
                                        tokens.push_back({col, row, i, "true", TokenType::Boolean});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'd':
                                    if (isKeyword(i, src, "dec")) {
                                        i += 2;
                                        tokens.push_back({col, row, i, "dec", TokenType::Dec});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'o':
                                    if (isKeyword(i, src, "op")) {
                                        i += 1;
                                        tokens.push_back({col, row, i, "op", TokenType::Op});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'i':
                                    if (isKeyword(i, src, "in")) {
                                        i += 1;
                                        tokens.push_back({col, row, i, "in", TokenType::In});
                                        tmp = {};
                                    } else if (isKeyword(i, src, "if")) {
                                        i += 1;
                                        tokens.push_back({col, row, i, "if", TokenType::If});
                                        tmp = {};
                                    } else if (isKeyword(i, src, "impl")) {
                                        i += 3;
                                        tokens.push_back({col, row, i, "impl", TokenType::Impl});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'l':
                                    if (isKeyword(i, src, "loop")) {
                                        i += 3;
                                        tokens.push_back({col, row, i, "loop", TokenType::Loop});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'c':
                                    if (isKeyword(i, src, "continue")) {
                                        i += 7;
                                        tokens.push_back({col, row, i, "continue", TokenType::Continue});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'r':
                                    if (isKeyword(i, src, "return")) {
                                        i += 5;
                                        tokens.push_back({col, row, i, "return", TokenType::Return});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'p':
                                    if (isKeyword(i, src, "prefix")) {
                                        i += 5;
                                        tokens.push_back({col, row, i, "prefix", TokenType::Prefix});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 's':
                                    if (isKeyword(i, src, "struct")) {
                                        i += 5;
                                        tokens.push_back({col, row, i, "struct", TokenType::Struct});
                                        tmp = {};
                                    } else if (isKeyword(i, src, "set")) {
                                        i += 2;
                                        tokens.push_back({col, row, i, "set", TokenType::Set});
                                        tmp = {};
                                    } else if (isKeyword(i, src, "suffix")) {
                                        i += 5;
                                        tokens.push_back({col, row, i, "suffix", TokenType::Suffix});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'b':
                                    if (isKeyword(i, src, "break")) {
                                        i += 4;
                                        tokens.push_back({col, row, i, "break", TokenType::Break});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'e':
                                    if (isKeyword(i, src, "else")) {
                                        i += 3;
                                        tokens.push_back({col, row, i, "else", TokenType::Else});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                case 'g':
                                    if (isKeyword(i, src, "get")) {
                                        i += 2;
                                        tokens.push_back({col, row, i, "get", TokenType::Get});
                                        tmp = {};
                                    } else {
                                        state = State::Symbol;
                                        tmp = {};
                                    }
                                    break;
                                default:
                                    state = State::Symbol;
                                    tmp = {};
                                    break;
                            }

                        } else if (c != '\n' && c != ' ' && c != '\t') {
                            Token x = {col, row, i, std::string(1, c), TokenType::Unknown};
                            errors.push_back({ErrorType::UnexpectedCharacter, x});
                            tmp = {};
                            aerror = true;
                        }

                        if (!aerror) {
                            tmp.raw += c;
                        }
                    }
                        break;
                    case State::NumberLiteral:
                        if (!(c == 'f' || c == 'i' || c == 'u' || c == '.' || c == '_') && !isdigit(c)) {
                            state = State::Start;

                            tmp.row = row;
                            tmp.column = col;
                            tmp.offset = i;
                            tmp.tokenType = TokenType::NumberLiteral;
                            tokens.push_back(tmp);
                            tmp = {};
                            i--;
                            col--;

                        } else {
                            tmp.raw += c;
                        }
                        break;
                    case State::Symbol:
                        if (!isalpha(c) && !isdigit(c) && c != '_') {
                            tmp.row = row;
                            tmp.column = col;
                            tmp.offset = i;
                            tmp.tokenType = TokenType::Symbol;
                            tokens.push_back(tmp);
                            tmp = {};

                            state = State::Start;

                            i--;
                            col--;
                        } else {
                            tmp.raw += c;
                        }
                        break;
                }
            }
        }
    }
}