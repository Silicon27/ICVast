//
// Created by David Yang on 2025-02-22.
//

#pragma once
#include <set>
#include <utility>

#include "errh.h"
#include "parsers.h"


inline std::vector<std::string> split(const std::string &str, const char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(str);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


class Parser {
private:
    std::vector<Token> &tokens;
    int currentToken;
    std::map<std::string, SymbolInfo> globalSymbolTable;
    std::string scope;
    std::vector<std::string> types = {"int", "float", "double", "char", "string", "bool", "void"};;

public:
    explicit Parser(std::vector<Token> &tokens, std::string scope = "global")
        : tokens(tokens), currentToken(0), scope(std::move(scope))
    {
    }
    // This would mean to ignore function internals until used
     static std::vector<Token> getScope(int& pos, const std::vector<Token> &tokens) {
        int amount = 0;
        SEGFAULTErrContext ctx = {unfilteredLines[tokens[pos].line].c_str(), tokens[pos].line, tokens[pos].column};
        const int initialPos = pos;
        do {
            if (tokens[pos].column != 0) {
                ctx = {unfilteredLines[tokens[pos].line].c_str(), tokens[pos].line, tokens[pos].column};
                error_context.store(&ctx);
            }

            if (tokens[pos].value == "{") {
                ++amount;
            } else if (tokens[pos].value == "}") {
                --amount;
            }
            ++pos;
        } while (amount != 0);
        pos--;
        return {tokens.begin() + initialPos + 1, tokens.begin() + (pos-1)};
    }

    void parseFunction(int& pos) {

        std::cout << "Parsing function" << std::endl;
        keyword::_pfn PARGS // fn
        const std::string name = ascii::_aname PARGS // Function name

        globalSymbolTable[name] = Function(name); // Instantiating a new function object (_pparams require pre-existing function instance to add too it)

        abstract::_pparams(pos, tokens, types, name, globalSymbolTable); // Parameters

        symbol::_parrow PARGS // ->
        const std::string returnType = abstract::_isType(tokens[pos].value, types, pos, tokens); // Return type

        const std::vector<Token> body = getScope PARGS // Function body

        std::get<Function>(globalSymbolTable[name]).returnType = returnType;
        std::get<Function>(globalSymbolTable[name]).body = body;
        std::get<Function>(globalSymbolTable[name]).scopeLevel = this->scope;
    }

    void parseVariable(int& pos) {
        std::cout << "Parsing variable" << std::endl;
        keyword::_pvar PARGS // var
        const std::string name = ascii::_aname PARGS // Variable name
        symbol::_pcolon PARGS // :
        const std::string type = abstract::_isType(tokens[pos].value, types, pos, tokens); // Type
        symbol::_peq PARGS // =
        const std::string value = abstract::_value(pos, tokens, type); // Value
        symbol::_psemi PARGS // ;

        globalSymbolTable[name] = Variable(name, type, value, this->scope);
    }

    void parseMerge(int& pos) {
        std::cout << "Parsing merge" << std::endl;
        keyword::_pmerge PARGS // merge
        std::string moduleLoc = abstract::_pmodule PARGS
        keyword::_pas PARGS
        std::string alias = ascii::_aname PARGS
        symbol::_psemi PARGS // ;

        std::ifstream file(moduleLoc);
        Lexer lexer(file);
        auto [moduleTokens, moduleUnfilteredLines] = lexer.tokenize();

        // Save the original unfiltered lines and set the new ones
        auto originalUnfilteredLines = unfilteredLines;
        set_unfilteredLines(moduleUnfilteredLines);

        Parser parser(moduleTokens, alias);
        parser.parse();
        pos--;

        // Restore the original unfiltered lines
        set_unfilteredLines(originalUnfilteredLines);

        // Get the symbol table
        auto moduleSymbolTable = parser.getSymbolTable();

        Namespace ns = Namespace(alias, moduleSymbolTable);
        globalSymbolTable[alias] = ns;
    }

    void parse() {
        for (currentToken = 0; currentToken < tokens.size(); ++currentToken) {
            if (tokens[currentToken].type == TokenType::KEYWORD) {
                if (tokens[currentToken].value == "fn")
                    parseFunction(currentToken);
                else if (tokens[currentToken].value == "var")
                    parseVariable(currentToken);
                else if (tokens[currentToken].value == "merge")
                    parseMerge(currentToken);

            } else if (tokens[currentToken].type == TokenType::eof) {
                return;
            }
        }
    }

    [[nodiscard]] std::map<std::string, SymbolInfo> getSymbolTable() {
        return globalSymbolTable;
    }
};
