#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_set>

enum class TokenType {
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    NUMBER,
    UNKNOWN,
    eof
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

inline bool isKeyword(const std::string &str) {
    static const std::unordered_set<std::string> keywords = {
        "if", "else", "while", "return", "fn", "var", "int", "float", "double", "char",
        "string", "bool", "void", "runtime", "static", "const", "merge", "as", "extern",
        "stdlib", "any", "true", "false", "namespace"
    };
    return keywords.contains(str);
}

class Lexer {
public:
    explicit Lexer(std::istream &inputStream)
        : input(inputStream), currentPos(0), lineNumber(1)
    {
        symbols = {
            "\\\"", "\\\'", "\\\t", "\\\n", "\\\r", "\\\v", "\\\f", "\\\b", "\\\a",
            "==", "!=", "<=", ">=", "->", "::",
            "=", "+", "-", "*", "/", "(", ")", "{", "}", ";", ",", ":", "\"", "\'",
            "\\", "@", "#", "$", "%", "&", "?", "!", "<", ">", "|", "^", "~"
        };

        std::ranges::sort(symbols,
                          [](const std::string &a, const std::string &b) {
                              return a.size() > b.size();
                          });
    }

    std::tuple<std::vector<Token>, std::vector<Token>, std::map<int, std::string>> tokenize() {
        std::vector<Token> tokens;
        std::string line;

        while (std::getline(input, line)) {
            currentLine = line;
            currentPos = 0;
            spaces = "";

            while (currentPos < currentLine.size()) {
                const char currentChar = currentLine[currentPos];

                if (std::isspace(currentChar)) {
                    spaces += currentChar;
                    ++currentPos;
                    continue;
                }

                if (std::isdigit(currentChar)) {
                    tokens.push_back(tokenizeNumber());
                    continue;
                }

                if (std::isalpha(currentChar) || currentChar == '_') {
                    tokens.push_back(tokenizeIdentifier());
                    continue;
                }

                if (isSymbolStart(currentChar)) {
                    tokens.push_back(tokenizeSymbol());
                    continue;
                }

                tokens.push_back({TokenType::UNKNOWN, std::string(1, currentChar), lineNumber, static_cast<int>(currentPos + 1)});
                unfilteredTokens.push_back({TokenType::UNKNOWN, std::string(1, currentChar) + spaces, lineNumber, static_cast<int>(currentPos + 1)});
                ++currentPos;
            }
            this->unfilteredLines[lineNumber] = line;
            ++lineNumber;
        }

        tokens.push_back({TokenType::eof, "", lineNumber, 0});
        unfilteredTokens.push_back({TokenType::eof, "", lineNumber, 0});
        return {tokens, unfilteredTokens, this->unfilteredLines};
    }

private:
    std::istream &input;
    std::string currentLine;
    std::size_t currentPos;
    int lineNumber;
    std::map<int, std::string> unfilteredLines;
    std::vector<std::string> symbols;
    std::string spaces;
    std::vector<Token> unfilteredTokens;

    Token tokenizeNumber() {
        const int column = static_cast<int>(currentPos) + 1;
        std::string number;
        while (currentPos < currentLine.size() && std::isdigit(currentLine[currentPos])) {
            number.push_back(currentLine[currentPos]);
            ++currentPos;
        }
        unfilteredTokens.push_back({TokenType::NUMBER, spaces + number, lineNumber, column});
        spaces.clear();
        return {TokenType::NUMBER, number, lineNumber, column};
    }

    Token tokenizeIdentifier() {
        const int column = static_cast<int>(currentPos) + 1;
        std::string ident;
        while (currentPos < currentLine.size() &&
               (std::isalnum(currentLine[currentPos]) || currentLine[currentPos] == '_')) {
            ident.push_back(currentLine[currentPos]);
            ++currentPos;
        }
        const TokenType type = isKeyword(ident) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
        unfilteredTokens.push_back({type, spaces + ident, lineNumber, column});
        spaces.clear();
        return {type, ident, lineNumber, column};
    }

    Token tokenizeSymbol() {
        const int column = static_cast<int>(currentPos) + 1;
        for (const auto &sym : symbols) {
            const std::size_t len = sym.size();
            if (currentPos + len <= currentLine.size() &&
                currentLine.substr(currentPos, len) == sym) {
                currentPos += len;
                unfilteredTokens.push_back({TokenType::SYMBOL, spaces + sym, lineNumber, column});
                spaces.clear();
                return {TokenType::SYMBOL, sym, lineNumber, column};
            }
        }

        const char unknownChar = currentLine[currentPos];
        ++currentPos;
        unfilteredTokens.push_back({TokenType::UNKNOWN, spaces + std::string(1, unknownChar), lineNumber, column});
        spaces.clear();
        return {TokenType::UNKNOWN, std::string(1, unknownChar), lineNumber, column};
    }

    [[nodiscard]] bool isSymbolStart(char c) const {
        return std::ranges::any_of(symbols, [c](const std::string &sym) {
            return !sym.empty() && sym[0] == c;
        });
    }
};