#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <csignal>
#include <thread>
#include <future>
#include <variant>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <cstdlib>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
    #include <csignal>
#endif

#define ICVAST_VERSION "1.0.0"

#define INTERPRETER_NAME "ICVAST"

#define PARGS (pos, *tokens);

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
    std::signal(SIGSEGV, signalHandler);

    std::string input;
    for (size_t i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            std::cout << "Usage: " << argv[0] << " [input file]" << std::endl;
            continue;
        } else if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--version") {
            std::cout << "ICVAST version " << ICVAST_VERSION << std::endl;
            continue;
        }
        input = argv[i];
    }
    if (input.empty()) {
        // TODO: instead of this, in the future, we can use a REPL
        std::cerr << INTERPRETER_NAME << ": ";
        std::cerr << "\033[31m" << "error: " << "No input file provided" << "\033[0m" << std::endl;
        return 0;
    }

    std::cout << "Input: " << input << std::endl;



    auto file = std::fstream(input, std::ios::in);

    auto lex = Lexer(file);

    auto [tokenizedOutput, unfilteredLines] = lex.tokenize();

    // printTree(tokenizedOutput);

    set_unfilteredLines(unfilteredLines);

    Parser parser(std::make_unique<std::vector<Token>>(tokenizedOutput), input);
    parser.parse();
    return 0;
}
