//
// Created by David Yang on 2025-02-24.
//

#pragma once

#include <iostream>
#include <string>

enum class ErrorType {
    // Unexpected errors (1000-1005)
    UNEXPECTED_TOKEN = 1000,
    UNEXPECTED_EOF = 1001,
    UNEXPECTED_SYMBOL = 1002,
    UNEXPECTED_KEYWORD = 1003,
    UNEXPECTED_IDENTIFIER = 1004,
    UNEXPECTED_NUMBER = 1005,

    // Expected errors (2000-2004)
    EXPECTED_TOKEN = 2000,
    EXPECTED_SYMBOL = 2001,
    EXPECTED_KEYWORD = 2002,
    EXPECTED_IDENTIFIER = 2003,
    EXPECTED_NUMBER = 2004,
    EXPECTED_PARAMETER_DECLARATION = 2005,

    // Runtime errors (3000-3007)
    OUT_OF_BOUNDS = 3000,
    INVALID_TYPE = 3001,
    OUT_OF_MEMORY = 3002,
    INVALID_OPERATION = 3003,
    INVALID_SYNTAX = 3004,
    INVALID_STATE = 3005,
    INVALID_ARGUMENT = 3006,
    INVALID_RETURN = 3007,

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

inline std::string red = "\033[1;31m";
inline std::string reset = "\033[0m";

namespace error {
    inline int gen(ErrInfo &errInfo) {
        std::string message;

        switch (errInfo.errType) {
            case ErrorType::UNEXPECTED_TOKEN:     message = "Unexpected token"; break;
            case ErrorType::UNEXPECTED_EOF:        message = "Unexpected end of file"; break;
            case ErrorType::UNEXPECTED_SYMBOL:     message = "Unexpected symbol"; break;
            case ErrorType::UNEXPECTED_KEYWORD:    message = "Unexpected keyword"; break;
            case ErrorType::UNEXPECTED_IDENTIFIER: message = "Unexpected identifier"; break;
            case ErrorType::UNEXPECTED_NUMBER:     message = "Unexpected number"; break;
            case ErrorType::EXPECTED_TOKEN:        message = "Expected token"; break;
            case ErrorType::EXPECTED_SYMBOL:       message = "Expected symbol"; break;
            case ErrorType::EXPECTED_KEYWORD:      message = "Expected keyword"; break;
            case ErrorType::EXPECTED_IDENTIFIER:   message = "Expected identifier"; break;
            case ErrorType::EXPECTED_NUMBER:       message = "Expected number"; break;
            case ErrorType::OUT_OF_BOUNDS:         message = "Out of bounds error"; break;
            case ErrorType::INVALID_TYPE:          message = "Invalid type error"; break;
            case ErrorType::OUT_OF_MEMORY:         message = "Out of memory error"; break;
            case ErrorType::INVALID_OPERATION:     message = "Invalid operation error"; break;
            case ErrorType::INVALID_SYNTAX:        message = "Invalid syntax error"; break;
            case ErrorType::INVALID_STATE:         message = "Invalid state error"; break;
            case ErrorType::INVALID_ARGUMENT:      message = "Invalid argument error"; break;
            case ErrorType::INVALID_RETURN:        message = "Invalid return error"; break;
            case ErrorType::UNKNOWN:               message = "Unknown error"; break;
            default:                               message = "Unknown error [Default Case]"; break;
        }

        std::cout << red << "Error (" << static_cast<int>(errInfo.errType) << ") at line " << errInfo.line;
        if (errInfo.column > 0) std::cout << ":" << errInfo.column;
        std::cout << "\n";

        if (!errInfo.sourceLine.empty()) {
            std::cout << errInfo.sourceLine << "\n";
            if (errInfo.column > 0 && errInfo.column <= static_cast<int>(errInfo.sourceLine.length())) {
                std::cout << std::string(errInfo.column - 1, ' ') << "^\n";
            }
        }

        std::cout << message << reset << "\n";
        return static_cast<int>(errInfo.errType);
    }
}