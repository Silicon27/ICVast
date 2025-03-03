//
// Created by David Yang on 2025-02-24.
//

#pragma once

inline ErrInfo errInfo;

#define SET_ERRINFO(TYPE, EXP_TOKEN) \
    do { \
    errInfo = { TYPE, tokens[pos].line, tokens[pos].column, tokens[pos].value, EXP_TOKEN }; \
    error::gen(errInfo); \
    } while (0)

struct Variable {
    std::string identifier; // Name of the variable.
    std::string type; // Type of the variable. Can be a primitive type or a user-defined type.
    std::string value; // Value of the variable. Can be an integer, float, or a string.
};

struct Function {
    std::string identifier; // Name of the function.
    std::string returnType; // Return type of the function.
    std::vector<std::string> parameters; // List of parameter types.
    std::vector<Variable> localVariables; // Variables declared in the function.
    std::string scopeLevel; // Name of its parent function/namespace (global if in global scope).
};

using SymbolInfo = std::variant<Variable, Function>;

namespace keyword {
    inline void _pfn(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "fn") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "fn");
    }

    inline void _pvar(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "var") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "var");
    }

    inline void _pif(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "if") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "if");
    }

    inline void _pelse(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "else") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "else");
    }

    inline void _pwhile(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "while") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "while");
    }

    inline void _pret(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "return") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "return");
    }

    inline void _pconst(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "const") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "const");
    }

    inline void _pruntime(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "runtime") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "runtime");
    }

    inline void _pstatic(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "static") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "static");
    }

    inline void _pint(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "int") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "int");
    }

    inline void _pfloat(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "float") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "float");
    }

    inline void _pdouble(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "double") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "double");
    }

    inline void _pchar(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "char") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "char");
    }

    inline void _pstring(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "string") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "string");
    }

    inline void _pvoid(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "void") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "void");
    }

    inline void _pbool(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "bool") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "bool");
    }
}

namespace symbol {
    inline void _seq(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "==") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "==");
    }

    inline void _sne(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "!=") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "!=");
    }

    inline void _sle(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "<=") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "<=");
    }

    inline void _sge(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ">=") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ">=");
    }

    inline void _popen(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "(") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "(");
    }

    inline void _pclose(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ")") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ")");
    }

    inline void _psquare_open(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "[") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "[");
    }

    inline void _psquare_close(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "]") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "]");
    }

    inline void _pcurly_open(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "{") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "{");
    }

    inline void _pcurly_close(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "}") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "}");
    }

    inline void _pcolon(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ":") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ":");
    }

    inline void _psemi(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ";") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ";");
    }

    inline void _pcomma(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ",") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ",");
    }

    inline void _peq(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "=") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "=");
    }

    inline void _pquote(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "\'") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "\'");
    }

    inline void _pdoublequote(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "\"") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "\"");
    }
}

namespace ascii {
    inline std::string _aname(int &pos, const std::vector<Token> &tokens) {
        if (std::ranges::all_of(tokens[pos].value, [](const char c) {
                return std::isalnum(c) || c == '_';
            })) {
            // Valid string found, increment position and return the name
            return tokens[pos++].value;
            }

        // Prepare and trigger an error for invalid input
        SET_ERRINFO(ErrorType::EXPECTED_IDENTIFIER, "valid identifier");
        return "";
    }
}

namespace combinators {
}

namespace abstract {

    inline std::string _value(int &pos, const std::vector<Token> &tokens, const std::string& type) {
        if (type == "int") {
            if (std::ranges::all_of(tokens[pos].value, [](const char c) {
                return std::isdigit(c);
            })) {
                return tokens[pos++].value;
            }
        } else if (type == "float") {
            // Ensure the string is non-empty and contains at most one '.'
            if (const std::string& val = tokens[pos].value; !val.empty() &&
                                                            std::ranges::count(val, '.') <= 1 &&
                                                            std::ranges::all_of(val, [](char c) { return std::isdigit(c) || c == '.'; }) &&
                                                            (val.front() != '.' && val.back() != '.'))
            {
                return tokens[pos++].value;
            }
        } else if (type == "string") {
            symbol::_pdoublequote PARGS;
            std::string str;
            while (tokens[pos].value != "\"" && tokens[pos - 1].value != "\\") {
                str += tokens[pos++].value;
            }
            symbol::_pdoublequote PARGS;
            return str;
        }
        return "";
    }

    inline std::string _isType(const std::string &str, const std::vector<std::string>& types, int &pos) {
        if (const auto it = std::ranges::find(types, str); it != types.end()) {
            ++pos;
            return *it; // Return the matching type
        }

        // Prepare and trigger an error for invalid input
        errInfo = {ErrorType::INVALID_TYPE, 0, 0, str};
        error::gen(errInfo);

        // Return an empty or fallback string if invalid
        return "";
    }

    inline std::tuple<std::string, std::string, std::optional<std::string>> _parg(int &pos, const std::vector<Token> &tokens, const std::vector<std::string>& types) {
        std::string name = ascii::_aname(pos, tokens);
        symbol::_pcolon(pos, tokens);
        std::string type = _isType(tokens[pos].value, types, pos);
        if (tokens[pos].value == "=") {
            symbol::_peq(pos, tokens);
            std::string value = _value(pos, tokens, type);
            return {name, type, value};
        }
        return {name, type, std::nullopt};
    }

    inline void _pparams(int &pos, const std::vector<Token> &tokens,
        const std::vector<std::string>& types, const std::string& parentFunction, std::map<std::string, SymbolInfo>& symbolTable) {

        symbol::_popen(pos, tokens);
        if (tokens[pos].value == ")") {
            // No parameters found, return early
            return;
        }
        // Attempt to match an argument.
        auto [name, type, value] = _parg(pos, tokens, types);

        std::get<Function>(symbolTable[parentFunction]).parameters.push_back(type);
        std::get<Function>(symbolTable[parentFunction]).localVariables.push_back(Variable{name, type, value.value_or("none")});

        // Continue matching arguments until the closing parenthesis is found.
        while (tokens[pos].value != ")") {
            if (tokens[pos].value == ","){
                // Attempt to match a comma symbol.
                // Using your symbol parser for comma.
                symbol::_pcomma(pos, tokens);

                // Then, match another argument.
                auto [name, type, value] = _parg(pos, tokens, types);
                std::get<Function>(symbolTable[parentFunction]).parameters.push_back(type);
                std::get<Function>(symbolTable[parentFunction]).localVariables.push_back(Variable{name, type, value.value_or("")});

            } else {
                // If the comma symbol is not found, then break the loop.
                break;
            }
        }

        symbol::_pclose(pos, tokens);
    }
}