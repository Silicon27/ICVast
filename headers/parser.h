//
// Created by David Yang on 2025-02-22.
//

#pragma once
#include "errh.h"
#include "parsers.h"




class Parser {
private:
    std::vector<Token> &tokens;
    int currentToken;
    std::map<std::string, SymbolInfo> symbolTable;
    std::string scope = "global";
    std::vector<std::string> types = {"int", "float", "double", "char", "string", "bool", "void"};;

public:
    explicit Parser(std::vector<Token> &tokens)
        : tokens(tokens), currentToken(0)
    {
    }

    void parseFunction(int& pos) {

        std::cout << "Parsing function" << std::endl;
        keyword::_pfn PARGS // fn
        const std::string name = ascii::_aname PARGS // Function name
        symbolTable[name] = Function(name); // Add function to symbol table

        abstract::_pparams(pos, tokens, types, name, symbolTable); // Parameters



        for (const auto& [name, details] : symbolTable) {
            if (std::holds_alternative<Variable>(details)) {
                std::cout << "Variable: " << std::get<Variable>(details).identifier << " in " << name << std::endl;
            } else {
                std::cout << "Function: " << std::get<Function>(details).identifier << std::endl;
                std::cout << "Local variables: " << std::endl;
                for (const auto&[identifier, type, value] : std::get<Function>(details).localVariables) {
                    std::cout << identifier << " : " << type << " = " << value << std::endl;
                }
            }
        }
    }

    int parseVariable(int pos);

    void parse() {
        for (currentToken = 0; currentToken < tokens.size(); ++currentToken) {
            if (tokens[currentToken].type == TokenType::KEYWORD) {
                if (tokens[currentToken].value == "fn") {
                    parseFunction(currentToken);
                }
                // } else if (tokens[currentToken].value == "var") {
                //     parseVariable(currentToken);
                // }
            }
        }
    }
};
