//
// Created by David Yang on 2025-02-24.
//

#pragma once
#include "parser.h"

inline ErrInfo errInfo;

inline std::map<int, std::string> unfilteredLines;

inline void set_unfilteredLines(const std::map<int, std::string>& lines) {
    unfilteredLines = lines;
}

inline std::string currfilePath;

inline void set_filePath(const std::string& path) {
    currfilePath = path;
}

#define SET_ERRINFO(TYPE, EXP_TOKEN) \
    do { \
    errInfo = { TYPE, tokens[pos].line, tokens[pos].column, unfilteredLines[tokens[pos].line], EXP_TOKEN, currfilePath }; \
    error::gen(errInfo); \
    } while (0)

using SymbolInfo = std::variant<struct Variable, struct Function, struct Namespace>;

struct Namespace {
    std::string identifier; // Name of the namespace.
    std::unordered_map<std::string, SymbolInfo> symbols; // Variables and functions declared in the namespace.
};

struct Variable {
    std::string identifier; // Name of the variable.
    std::string type; // Type of the variable. Can be a primitive type or a user-defined type.
    std::string value; // Value of the variable. Can be an integer, float, or a string.
    std::string scopeLevel; // Name of its parent function/namespace (global if in global scope).
};

struct Function {
    std::string identifier; // Name of the function.
    std::string returnType; // Return type of the function.
    std::vector<std::string> parameters; // List of parameter types.
    std::vector<Variable> localVariables; // Variables declared in the function.
    std::string scopeLevel; // Name of its parent function/namespace (global if in global scope).
    std::vector<Token> body; // List of tokens that make up the function body.
};



namespace keyword {
    namespace noErr {
        inline std::string _rpstdlib(int &pos, const std::vector<Token> &tokens) {
            if (tokens[pos].value == "stdlib") {
                ++pos;
                return "stdlib";
            }
            throw std::runtime_error("Expected keyword 'stdlib'");
        }
    }

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

    inline void _pmerge(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "merge") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "merge");
    }

    inline void _pas(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "as") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "as");
    }

    inline void _pextern(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "extern") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "extern");
    }

    inline void _pstdlib(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "stdlib") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_KEYWORD, "stdlib");
    }
}

namespace symbol {
    namespace noErr {
        inline void _pdoublequote(int &pos, const std::vector<Token> &tokens) {
            if (tokens[pos].value == "\"") {
                ++pos;
                return;
            }
            throw std::runtime_error("Expected symbol '\"'");
        }
    }

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

    inline void _pplus(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "+") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "+");
    }

    inline void _pminus(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "-") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "-");
    }

    inline void _parrow(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "->") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "->");
    }

    inline void _patsign(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "@") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "@");
    }

    inline void _phash(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "#") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "#");
    }

    inline void _pdollar(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "$") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "$");
    }

    inline void _pmod(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "%") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "%");
    }

    inline void _pand(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "&") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "&");
    }

    inline void _pquestion(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "?") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "?");
    }

    inline void _pexcl(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "!") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "!");
    }

    inline void _plt(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "<") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "<");
    }

    inline void _pgt(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == ">") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, ">");
    }

    inline void _ppipe(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "|") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "|");
    }

    inline void _pcaret(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "^") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "^");
    }

    inline void _ptilde(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].value == "~") {
            ++pos;
            return;
        }
        SET_ERRINFO(ErrorType::EXPECTED_SYMBOL, "~");
    }
}

namespace ascii {
    namespace noErr {
        inline std::string _aname(int &pos, const std::vector<Token> &tokens) {
            // Check if value is composed solely of valid identifier characters (alnum or '_')
            // and also ensure it's not exclusively composed of digits.
            if (const std::string &value = tokens[pos].value; std::ranges::all_of(value, [](char c) {
                                                                  return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
                                                              }) &&
                                                              !std::ranges::all_of(value, [](char c) {
                                                                  return std::isdigit(static_cast<unsigned char>(c));
                                                              })) {
                // Valid identifier found, increment position and return the name
                return tokens[pos++].value;
                                                              }


            // Prepare and trigger an error for invalid input
            throw std::runtime_error("Invalid identifier");
        }

        inline std::string _adigit(int &pos, const std::vector<Token> &tokens) {
            if (tokens[pos].type == TokenType::NUMBER) {
                return tokens[pos++].value;
            }
            throw std::runtime_error("Invalid number");
        }

        inline std::string _pstring(int &pos, const std::vector<Token> &tokens) {
            symbol::_pdoublequote (pos, tokens);
            std::string str;
            while (tokens[pos].value != "\"" && tokens[pos - 1].value != "\\") {
                str += tokens[pos++].value;
            }
            symbol::_pdoublequote (pos, tokens);
            return str;
        }
    }

    inline std::string _aname(int &pos, const std::vector<Token> &tokens) {
        // Check if value is composed solely of valid identifier characters (alnum or '_')
        // and also ensure it's not exclusively composed of digits.
        if (const std::string &value = tokens[pos].value; std::ranges::all_of(value, [](char c) {
                                                              return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
                                                          }) &&
                                                          !std::ranges::all_of(value, [](char c) {
                                                              return std::isdigit(static_cast<unsigned char>(c));
                                                          })) {
            // Valid identifier found, increment position and return the name
            return tokens[pos++].value;
                                                          }


        // Prepare and trigger an error for invalid input
        SET_ERRINFO(ErrorType::EXPECTED_IDENTIFIER, "VALID IDENTIFIER");
        return "";
    }

    inline std::string _adigit(int &pos, const std::vector<Token> &tokens) {
        if (tokens[pos].type == TokenType::NUMBER) {
            return tokens[pos++].value;
        }
        SET_ERRINFO(ErrorType::EXPECTED_NUMBER, "NUMBER");
        return "";
    }

    inline std::string _pstring(int &pos, const std::vector<Token> &tokens) {
        symbol::noErr::_pdoublequote (pos, tokens);
        std::string str;
        while (tokens[pos].value != "\"" && tokens[pos - 1].value != "\\") {
            str += tokens[pos++].value;
        }
        symbol::noErr::_pdoublequote (pos, tokens);
        return str;
    }
}

namespace combinators {
    inline void _pparse_until(int &pos, const std::vector<Token> &tokens, const std::string& delimiter) {
        while (tokens[pos].value != delimiter) {
            ++pos;
        }
    }

    template<auto... Parsers>
    auto _por(int &pos, const std::vector<Token> &tokens) {
        // Call each parser in the parameter pack
        for (auto p : {Parsers...}) {
            try {
                p (pos, tokens);
                return p;
            } catch (const ErrInfo& _) {
                continue;
            }
        }
    }

    template<auto... Parsers>
    auto _ror(int &pos, const std::vector<Token> &tokens) {
        // Call each parser in the parameter pack
        for (auto p : {Parsers...}) {
            try {
                auto val = p (pos, tokens);
                return std::make_tuple(val, p);
                //catch all errors
            } catch (const std::runtime_error &_) {
                continue;
            }
        }

        // Prepare and trigger an error for invalid input
        SET_ERRINFO(ErrorType::EXPECTED_ONE_OF, "VALID OPTIONS");
        throw std::runtime_error("No valid parser succeeded");
    }
}

namespace abstract {

    namespace noErr {
        inline std::string _pmodule(int &pos, const std::vector<Token> &tokens) {
            std::string location = ascii::_pstring (pos, tokens);
            if (!std::filesystem::exists(location)) {
                throw std::runtime_error("File not found");
            }
            return location;
        }
    }

    // Add expression parsing functions here
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
            symbol::_pdoublequote (pos, tokens);
            std::string str;
            while (tokens[pos].value != "\"") {
                if (tokens[pos].value == "\\\"") {
                    str += "\"";
                    pos++;
                }
                else
                    str += tokens[pos++].value;
            }
            symbol::_pdoublequote (pos, tokens);
            return str;
        }
        return "";
    }

    inline std::string _isType(const std::string &str, const std::vector<std::string>& types, int &pos,const std::vector<Token> &tokens) {
        if (const auto it = std::ranges::find(types, str); it != types.end()) {
            ++pos;
            return *it; // Return the matching type
        }

        // Prepare and trigger an error for invalid input
        SET_ERRINFO(ErrorType::INVALID_TYPE, "VALID TYPE");

        // Return an empty or fallback string if invalid
        return "";
    }

    inline std::tuple<std::string, std::string, std::optional<std::string>> _parg(int &pos, const std::vector<Token> &tokens, const std::vector<std::string>& types) {
        std::string name = ascii::_aname(pos, tokens);
        symbol::_pcolon(pos, tokens);
        std::string type = _isType(tokens[pos].value, types, pos, tokens);
        if (tokens[pos].value == "=") {
            symbol::_peq(pos, tokens);
            std::string value = _value(pos, tokens, type);
            return {name, type, value};
        }
        return {name, type, std::nullopt};
    }

    inline void _pparams(int &pos, const std::vector<Token> &tokens,
        const std::vector<std::string>& types, const std::string& parentFunction, std::unordered_map<std::string, SymbolInfo>& symbolTable) {

        symbol::_popen(pos, tokens);
        if (tokens[pos].value == ")") {
            // No parameters found, return early
            symbol::_pclose(pos, tokens);
            return;
        }
        // Attempt to match an argument.
        auto [name, type, value] = _parg(pos, tokens, types);

        std::get<Function>(symbolTable[parentFunction]).parameters.push_back(type);
        std::get<Function>(symbolTable[parentFunction]).localVariables.push_back(Variable{name, type, value.value_or("None")});

        // Continue matching arguments until the closing parenthesis is found.
        while (tokens[pos].value != ")") {
            if (tokens[pos].value == ","){
                // Attempt to match a comma symbol.
                // Using your symbol parser for comma.
                symbol::_pcomma(pos, tokens);

                // Then, match another argument.
                auto [name, type, value] = _parg(pos, tokens, types);
                std::get<Function>(symbolTable[parentFunction]).parameters.push_back(type);
                std::get<Function>(symbolTable[parentFunction]).localVariables.push_back(Variable{name, type, value.value_or("None")});

            } else {
                // If the comma symbol is not found, then break the loop.
                break;
            }
        }

        symbol::_pclose(pos, tokens);
    }

    inline Variable _pcall_arg(int &pos, const std::vector<Token> &tokens, const std::unordered_map<std::string, SymbolInfo>& symbolTable) {
        if (const auto [val, func] = combinators::_ror<ascii::noErr::_aname, ascii::noErr::_adigit>(pos, tokens); func == ascii::noErr::_aname) {
            try {
                return std::get<Variable>(symbolTable.at(val));
            } catch (const std::out_of_range& e) {
                SET_ERRINFO(ErrorType::EXPECTED_IDENTIFIER, "VALID IDENTIFIER");
            } catch (const std::bad_variant_access& e) {
                SET_ERRINFO(ErrorType::INVALID_TYPE, "VALID TYPE");
            }
        } else if (func == ascii::noErr::_adigit) {
            return Variable{"", "int", val, ""};
        }
        SET_ERRINFO(ErrorType::EXPECTED_IDENTIFIER, "VALID IDENTIFIER");
        return Variable{"", "", "", ""};
    }

    // create another _pparams version that returns a vector of strings of the values of the parameters
    inline std::vector<Variable> _pcall_params(int &pos, const std::vector<Token> &tokens, const std::unordered_map<std::string, SymbolInfo>& symbolTable) {
        symbol::_popen(pos, tokens);
        std::vector<Variable> arguments;
        if (tokens[pos].value == ")") {
            // No parameters found, return early
            return arguments;
        }
        // Attempt to match an argument.
        const Variable value = _pcall_arg(pos, tokens, symbolTable);

        arguments.push_back(value);

        // Continue matching arguments until the closing parenthesis is found.
        while (tokens[pos].value != ")") {
            if (tokens[pos].value == ","){
                // Attempt to match a comma symbol.
                // Using your symbol parser for comma.
                symbol::_pcomma(pos, tokens);

                // Then, match another argument.
                const Variable value = _pcall_arg(pos, tokens, symbolTable);
                arguments.push_back(value);
            } else {
                // If the comma symbol is not found, then break the loop.
                break;
            }
        }

        symbol::_pclose(pos, tokens);
        return arguments;
    }

    inline std::string _pvar_val(int &pos, const std::vector<Token> &tokens, const std::unordered_map<std::string, SymbolInfo>& symbolTable) {
        try {
            return std::get<Variable>(symbolTable.at(tokens[pos++].value)).value;
        } catch (const std::out_of_range& e) {
            SET_ERRINFO(ErrorType::EXPECTED_IDENTIFIER, "VALID IDENTIFIER");
        } catch (const std::bad_variant_access& e) {
            SET_ERRINFO(ErrorType::INVALID_TYPE, "VALID TYPE");
        }
        return "";
    }

    inline std::string _pmodule(int &pos, const std::vector<Token> &tokens) {
        std::string location = ascii::_pstring (pos, tokens);
        if (!std::filesystem::exists(location)) {
            SET_ERRINFO(ErrorType::FILE_NOT_FOUND, "VALID MODULE FILE PATH");
        }
        return location;
    }
}