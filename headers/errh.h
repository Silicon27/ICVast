//
// Created by David Yang on 2025-02-24.
//

#pragma once

#include <iostream>
#include <string>

struct SEGFAULTErrContext {
    const char* current_line;
    int line_number;
    int column_number;
};

// Use a single atomic pointer for thread safety
inline std::atomic<SEGFAULTErrContext*> error_context{nullptr};

inline void signalHandler(int signal) {
    constexpr auto red = "\033[1;31m";
    constexpr auto reset = "\033[0m";

    std::cerr << red << "\n⚠️ FATAL RUNTIME ERROR ⚠️\n" << reset;
    std::cerr << red << "Received signal: " << reset << signal << " (SIGSEGV)\n";

    std::cout << "Error context: " << error_context.load()->column_number << std::endl;

    if (const SEGFAULTErrContext* ctx = error_context.load()) {
        std::cerr << red << "Crash location: " << reset
                  << "Line " << ctx->line_number
                  << ":" << ctx->column_number << "\n";
        std::cerr << red << "Context: " << reset << "\n";
        std::cerr << "  " << ctx->current_line << "\n";
        std::cerr << "  " << std::string(ctx->column_number - 1, ' ')
                  << red << "^" << reset << "\n";
    }

    std::cerr << red << "Emergency shutdown initiated..." << reset << "\n";
    std::_Exit(signal);
}

enum class ErrorType {
    // Unexpected errors (1000-1007)
    UNEXPECTED_TOKEN = 1000,
    UNEXPECTED_EOF = 1001,
    UNEXPECTED_SYMBOL = 1002,
    UNEXPECTED_KEYWORD = 1003,
    UNEXPECTED_IDENTIFIER = 1004,
    UNEXPECTED_NUMBER = 1005,
    UNEXPECTED_OPERATOR = 1006,
    UNEXPECTED_STRING = 1007,
    UNEXPECTED_NAMED_FUNCTION = 1008,

    // Expected errors (2000-2007)
    EXPECTED_TOKEN = 2000,
    EXPECTED_SYMBOL = 2001,
    EXPECTED_KEYWORD = 2002,
    EXPECTED_IDENTIFIER = 2003,
    EXPECTED_NUMBER = 2004,
    EXPECTED_PARAMETER_DECLARATION = 2005,
    EXPECTED_OPERATOR = 2006,
    EXPECTED_STRING = 2007,
    EXPECTED_ONE_OF = 2008,

    // Runtime errors (3000-3010)
    OUT_OF_BOUNDS = 3000,
    INVALID_TYPE = 3001,
    OUT_OF_MEMORY = 3002,
    INVALID_OPERATION = 3003,
    INVALID_SYNTAX = 3004,
    INVALID_STATE = 3005,
    INVALID_ARGUMENT = 3006,
    INVALID_RETURN = 3007,
    DIVISION_BY_ZERO = 3008,
    NULL_POINTER = 3009,
    STACK_OVERFLOW = 3010,

    // File system errors (4000-4002)
    FILE_NOT_FOUND = 4000,
    PERMISSION_DENIED = 4001,
    FILE_READ_ERROR = 4002,

    // Generic unknown error (9999)
    UNKNOWN = 9999
};


struct ErrInfo {
    ErrorType errType;
    int line;
    int column = -1;
    std::string sourceLine;
    std::string expected;
};

inline constexpr std::string bold_red = "\033[1;31m";
inline constexpr std::string cyan = "\033[0;36m";
inline constexpr std::string green = "\033[0;32m";
inline constexpr std::string yellow = "\033[0;33m";
inline constexpr std::string reset = "\033[0m";

namespace error {
    inline std::string format_expected(const std::string& expected) {
        if (expected == "VALID TYPE") return "valid type";
        if (expected == "VALID IDENTIFIER") return "valid identifier";
        if (expected == "VALID SYMBOL") return "valid symbol";
        if (expected == "VALID OPERATOR") return "valid operator";
        return "'" + expected + "'";
    }

    inline int gen(ErrInfo &errInfo) {
        std::string message;
        std::string category;
        int code = static_cast<int>(errInfo.errType);

        // Determine error category
        if (code >= 1000 && code < 2000) category = "Syntax Error";
        else if (code >= 2000 && code < 3000) category = "Syntax Error";
        else if (code >= 3000 && code < 4000) category = "Runtime Error";
        else if (code >= 4000 && code < 5000) category = "File Error";
        else category = "Unknown Error";

        switch (errInfo.errType) {
            case ErrorType::UNEXPECTED_TOKEN:              message = "Unexpected token"; break;
            case ErrorType::UNEXPECTED_EOF:                message = "Unexpected end of file"; break;
            case ErrorType::UNEXPECTED_SYMBOL:             message = "Unexpected symbol"; break;
            case ErrorType::UNEXPECTED_KEYWORD:            message = "Unexpected keyword"; break;
            case ErrorType::UNEXPECTED_IDENTIFIER:         message = "Unexpected identifier"; break;
            case ErrorType::UNEXPECTED_NUMBER:             message = "Unexpected number"; break;
            case ErrorType::UNEXPECTED_OPERATOR:           message = "Unexpected operator"; break;
            case ErrorType::UNEXPECTED_STRING:             message = "Unexpected string literal"; break;
            case ErrorType::UNEXPECTED_NAMED_FUNCTION:     message = "Unexpected named function"; break;

            case ErrorType::EXPECTED_TOKEN:                message = "Expected token"; break;
            case ErrorType::EXPECTED_SYMBOL:               message = "Expected symbol"; break;
            case ErrorType::EXPECTED_KEYWORD:              message = "Expected keyword"; break;
            case ErrorType::EXPECTED_IDENTIFIER:           message = "Expected identifier"; break;
            case ErrorType::EXPECTED_NUMBER:               message = "Expected number"; break;
            case ErrorType::EXPECTED_PARAMETER_DECLARATION:message = "Expected parameter declaration"; break;
            case ErrorType::EXPECTED_OPERATOR:             message = "Expected operator"; break;
            case ErrorType::EXPECTED_STRING:               message = "Expected string literal"; break;
            case ErrorType::EXPECTED_ONE_OF:               message = "Expected one of"; break;

            case ErrorType::OUT_OF_BOUNDS:                 message = "Out of bounds error"; break;
            case ErrorType::INVALID_TYPE:                  message = "Invalid type error"; break;
            case ErrorType::OUT_OF_MEMORY:                 message = "Out of memory error"; break;
            case ErrorType::INVALID_OPERATION:             message = "Invalid operation error"; break;
            case ErrorType::INVALID_SYNTAX:                message = "Invalid syntax error"; break;
            case ErrorType::INVALID_STATE:                 message = "Invalid state error"; break;
            case ErrorType::INVALID_ARGUMENT:              message = "Invalid argument error"; break;
            case ErrorType::INVALID_RETURN:                message = "Invalid return error"; break;
            case ErrorType::DIVISION_BY_ZERO:              message = "Division by zero error"; break;
            case ErrorType::NULL_POINTER:                  message = "Null pointer error"; break;
            case ErrorType::STACK_OVERFLOW:                message = "Stack overflow error"; break;

            case ErrorType::FILE_NOT_FOUND:                message = "File not found error"; break;
            case ErrorType::PERMISSION_DENIED:             message = "Permission denied error"; break;
            case ErrorType::FILE_READ_ERROR:               message = "File read error"; break;

            case ErrorType::UNKNOWN:                       message = "Unknown error"; break;
            default:                                       message = "Unknown error [Default Case]"; break;
        }


        // Error header
        std::cerr << bold_red << "➔ " << category << ": " << reset
                  << message << " " << cyan << "[" << code << "]" << reset << "\n";

        // Location information
        std::cerr << cyan << "  ╰─▶ " << reset
                  << "At line " << errInfo.line;
        if (errInfo.column > 0) std::cerr << ":" << errInfo.column;
        std::cerr << "\n";

        // Source code snippet
        if (!errInfo.sourceLine.empty()) {
            std::cerr << yellow << "   │ " << reset << errInfo.sourceLine << "\n";
            if (errInfo.column > 0 && errInfo.column <= static_cast<int>(errInfo.sourceLine.length())) {
                std::cerr << yellow << "   ╰─" << std::string(std::max(0, errInfo.column - 2), '-')
                            << green << "^" << reset << "\n";
            }
        }

        // Expected information
        if (!errInfo.expected.empty()) {
            std::cerr << green << "   ➔ " << reset << bold_red << "Help: " << reset;

            if (code >= 2000 && code < 3000) {
                std::cerr << "Expected " << format_expected(errInfo.expected);
            } else if (code >= 3000 && code < 4000) {
                std::cerr << "Required " << format_expected(errInfo.expected);
            } else {
                std::cerr << message << ": " << format_expected(errInfo.expected);
            }

            std::cerr << "\n";
        }

        std::exit(code);
    }
}