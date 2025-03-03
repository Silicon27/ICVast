#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#define PARGS (pos, tokens);

#include "headers/lexer.h"
#include "headers/parser.h"

void printTree(const std::vector<Token>& tokenizedList)
{
    for (const auto &[type, value, line, column] : tokenizedList) {
        std::cout << "Line " << line << ": " << value << " [";
        switch (type) {
            case TokenType::KEYWORD:
                std::cout << "KEYWORD";
            break;
            case TokenType::SYMBOL:
                std::cout << "SYMBOL";
            break;
            case TokenType::IDENTIFIER:
                std::cout << "IDENTIFIER";
            break;
            case TokenType::NUMBER:
                std::cout << "NUMBER";
            break;
            default:
                std::cout << "UNKNOWN";
            break;
        }
        std::cout << "]\n";
    }
}

int main(const int argc, char* argv[])
{
    std::string input;
    for (size_t i = 1; i < argc; i++)
    {
        input = argv[i];
    }

    std::cout << "Input: " << input << std::endl;

    auto file = std::fstream(input, std::ios::in);

    auto lex = Lexer(file);

    std::vector<Token> tokenizedOutput = lex.tokenize();

    printTree(tokenizedOutput);

    Parser parser(tokenizedOutput);
    parser.parse();
    return 0;
}
