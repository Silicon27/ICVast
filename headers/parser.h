//
// Created by David Yang on 2025-02-22.
//

#pragma once

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
    std::unique_ptr<std::vector<Token>> tokens; // Change to unique_ptr
    std::unique_ptr<std::vector<Token>> unfilteredTokens;
    int currentToken;
    std::unordered_map<std::string, SymbolInfo> globalSymbolTable;
    std::string scope;
    std::vector<std::string> types = {"int", "float", "double", "char", "string", "bool", "void", "any"};
    std::string filePath;

public:
    explicit Parser(std::unique_ptr<std::vector<Token>> tokens, std::unique_ptr<std::vector<Token>> unfilteredTokens, const std::string& filePath, std::string scope = "global")
        : tokens(std::move(tokens)), unfilteredTokens(std::move(unfilteredTokens)), currentToken(0), scope(std::move(scope)), filePath(filePath)
    {
        set_filePath(filePath);
    }

    // This would mean to ignore function internals until used
    static std::tuple<std::vector<Token>, std::vector<Token>> getScope(int& pos, const std::vector<Token> &tokens, const std::vector<Token> &unfilteredTokens) {
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
        return {{tokens.begin() + initialPos + 1, tokens.begin() + pos}, {unfilteredTokens.begin() + initialPos + 1, unfilteredTokens.begin() + pos}};
    }

    static void setPos2ScopeEnd(int &pos, const std::vector<Token> &tokens) {
        int amount = 0;
        const int initialPos = pos;
        do {
            if (tokens[pos].value == "{") {
                ++amount;
            } else if (tokens[pos].value == "}") {
                --amount;
            }
            ++pos;
        } while (amount != 0);
    }

    void parseFunction(int& pos) {
        std::cout << "Parsing function" << std::endl;
        keyword::_pfn PARGS // fn
        const std::string name = ascii::_aname PARGS // Function name

        globalSymbolTable[name] = Function(name); // Instantiating a new function object (_pparams require pre-existing function instance to add too it)

        abstract::_pparams(pos, *tokens, types, name, globalSymbolTable, *unfilteredTokens); // Parameters

        symbol::_parrow PARGS // ->
        const std::string returnType = abstract::_isType((*tokens)[pos].value, types, pos, *tokens); // Return type

        auto [body, unfilteredBody] = getScope (pos, *tokens, *unfilteredTokens); // Function body

        std::get<Function>(globalSymbolTable[name]).returnType = returnType;
        std::get<Function>(globalSymbolTable[name]).body = body;
        std::get<Function>(globalSymbolTable[name]).unfilteredBody = unfilteredBody;
        std::get<Function>(globalSymbolTable[name]).scopeLevel = this->scope;
    }

    void parseVariable(int& pos) {
        std::cout << "Parsing variable" << std::endl;
        keyword::_pvar PARGS // var
        const std::string name = ascii::_aname PARGS // Variable name
        symbol::_pcolon PARGS // :
        const std::string type = abstract::_isType((*tokens)[pos].value, types, pos, *tokens); // Type
        symbol::_peq PARGS // =
        const std::string value = abstract::_value(pos, *tokens, type, *unfilteredTokens, globalSymbolTable); // Value

        globalSymbolTable[name] = Variable(name, type, value, this->scope);
    }

    void parseMerge(int& pos) {
        std::cout << "Parsing merge" << std::endl;
        keyword::_pmerge PARGS // merge
        if (const auto [val, func] =
            combinators::_ror<abstract::noErr::_pmodule, keyword::noErr::_rpstdlib> PARGS func == abstract::noErr::_pmodule)
            {

            std::string moduleLoc = val; // Module location
            keyword::_pas PARGS
            std::string alias = ascii::_aname PARGS

            std::ifstream file(moduleLoc);
            Lexer lexer(file);
            auto [moduleTokens, moduleUnfiltered, moduleUnfilteredLines] = lexer.tokenize();

            // Save the original unfiltered lines and set the new ones
            auto originalUnfilteredLines = unfilteredLines;
            set_unfilteredLines(moduleUnfilteredLines);

            std::string originalFilePath = filePath;
            set_filePath(moduleLoc);

            Parser parser(std::make_unique<std::vector<Token>>(moduleTokens), std::make_unique<std::vector<Token>>(moduleUnfiltered), moduleLoc, alias);
            parser.parse();

            // Restore the original unfiltered lines
            set_unfilteredLines(originalUnfilteredLines);

            // Restore the original file path
            set_filePath(originalFilePath);

            // Get the symbol table
            auto moduleSymbolTable = parser.getSymbolTable();

            auto ns = Namespace(alias, moduleSymbolTable);
            globalSymbolTable[alias] = ns;
        } else if (func == keyword::noErr::_rpstdlib) {
            symbol::_patsign PARGS
            std::string loc = ascii::_pstring PARGS
            keyword::_pas PARGS
            std::string alias = ascii::_aname PARGS

            const char* stdlibPath = std::getenv("CVAST_STDLIB");
            if (stdlibPath == nullptr) {
                errInfo = { ErrorType::EXPECTED_ENV_VAR, (*tokens)[pos].line, (*tokens)[pos].column, unfilteredLines[(*tokens)[pos].line], "CVAST_STDLIB", currfilePath };
                error::gen(errInfo);
            }
            std::cout << "stdlib path: " << stdlibPath << std::endl;
            // check if is a directory (foulder) first, if not, add .cv and check again
            std::string fullPath = std::string(stdlibPath) + "/" + loc;
            if (!std::filesystem::is_directory(fullPath)) {
                fullPath += ".cv";
            } else {
                // TODO: handle directory merging later (merge all files in directory), error for now
                errInfo = { ErrorType::EXPECTED_ONE_OF, (*tokens)[pos].line, (*tokens)[pos].column, unfilteredLines[(*tokens)[pos].line], "File, not directory", currfilePath };
                error::gen(errInfo);
            }

            // check if file exists
            if (!std::filesystem::exists(fullPath)) {
                errInfo = { ErrorType::EXPECTED_ONE_OF, (*tokens)[pos - 3].line, (*tokens)[pos - 3].column, unfilteredLines[(*tokens)[pos - 3].line], "Invalid stdlib file", currfilePath };
                error::gen(errInfo);
            }

            std::ifstream file(fullPath);
            Lexer lexer(file);
            auto [moduleTokens, moduleUnfiltered, moduleUnfilteredLines] = lexer.tokenize();

            // Save the original unfiltered lines and set the new ones
            auto originalUnfilteredLines = unfilteredLines;
            set_unfilteredLines(moduleUnfilteredLines);

            std::string originalFilePath = filePath;
            set_filePath(fullPath);

            Parser parser(std::make_unique<std::vector<Token>>(moduleTokens), std::make_unique<std::vector<Token>>(moduleUnfiltered), fullPath, alias);
            parser.parse();

            // Restore the original unfiltered lines
            set_unfilteredLines(originalUnfilteredLines);

            // Restore the original file path
            set_filePath(originalFilePath);

            // Get the symbol table
            auto moduleSymbolTable = parser.getSymbolTable();

            auto ns = Namespace(alias, moduleSymbolTable);
            globalSymbolTable[alias] = ns;
        }
    }

    void parseExtern(int &pos) {
        std::cout << "Parsing extern" << std::endl;
        keyword::_pextern PARGS // extern
        const std::string action = ascii::_pstring PARGS // Action
        if (action == "writescr") {
            symbol::_popen PARGS // (
            const std::string message = abstract::_pvar_val(pos, *tokens, globalSymbolTable); // Message
            symbol::_pclose PARGS // )
            std::cout << message << "\n";
        } else if (action == "readscr") {
            symbol::_popen PARGS // (
            symbol::_pclose PARGS // )
        } else {
            errInfo = { ErrorType::EXPECTED_ONE_OF, (*tokens)[pos].line, (*tokens)[pos].column, unfilteredLines[(*tokens)[pos].line], "writescr, readscr", currfilePath };
            error::gen(errInfo);
        }
    }

    void parseReturn(int &pos) {
        std::cout << "Parsing return" << std::endl;
        keyword::_preturn PARGS // return
    }

    void parseElseIf(int &pos) {
        std::cout << "Parsing else if" << std::endl;
        pos--;
        parseIf(pos);
    }

    void parseIf(int &pos) {
        std::cout << "Parsing if" << std::endl;
        keyword::_pif PARGS // if
        symbol::_popen PARGS // (

        // Use ConditionParser for the condition
        std::vector<Token> conditionTokens;
        while (pos < tokens->size() && (*tokens)[pos].value != ")") {
            conditionTokens.push_back((*tokens)[pos++]);
        }

        ConditionParser conditionParser(conditionTokens, globalSymbolTable);
        const bool conditionResult = conditionParser.evaluate();

        symbol::_pclose PARGS // )

        if (conditionResult) {
            // Parse then-block
            auto [body, unfilteredBody] = getScope(pos, *tokens, *unfilteredTokens);
            Parser bodyParser(std::make_unique<std::vector<Token>>(body),
                             std::make_unique<std::vector<Token>>(unfilteredBody),
                             filePath, scope);
            bodyParser.set_globalSymbolTable(globalSymbolTable);
            bodyParser.parse();
            symbol::_pcurly_close PARGS // }
            // Skip else and else if blocks if present
            while ((*tokens)[pos].value != "else" || (*tokens)[pos + 1].value != "if") {
                if ((*tokens)[pos].value == "else" && (*tokens)[pos + 1].value == "if") {
                    keyword::_pelse PARGS // else
                    keyword::_pif PARGS // if
                    // there are conditions to check within the () so parse it but still skip the block
                    setPos2ScopeEnd(pos, *tokens);
                } else if ((*tokens)[pos].value == "else") {
                    keyword::_pelse PARGS // else
                    setPos2ScopeEnd(pos, *tokens);
                } else {
                    break;
                }
            }
        } else {
            // Skip else block if present
            setPos2ScopeEnd(pos, *tokens);
        }

        try {
            keyword::noErr::_pelse PARGS
            keyword::noErr::_pif PARGS // if
            parseElseIf(pos);
        } catch (const std::runtime_error& e) {}

        try {
            keyword::noErr::_pelse PARGS
            if (!conditionResult) {
                // Parse else-block
                auto [body, unfilteredBody] = getScope(pos, *tokens, *unfilteredTokens);
                Parser bodyParser(std::make_unique<std::vector<Token>>(body),
                                 std::make_unique<std::vector<Token>>(unfilteredBody),
                                 filePath, scope);
                bodyParser.set_globalSymbolTable(globalSymbolTable);
                bodyParser.parse();
            } else {
                // Skip then block if present
                setPos2ScopeEnd(pos, *tokens);
            }
        } catch (const std::runtime_error& e) {}
        pos--;
    }

    // ========================================================================================================
    //                                              Helper functions
    // ========================================================================================================

    void parseFunctionCall(int &pos) {
        std::cout << "Parsing function call" << std::endl;
        const std::string name = ascii::_aname PARGS // Function name
        const std::vector<Variable> arguments = abstract::_pcall_params(pos, *tokens, globalSymbolTable, *unfilteredTokens);
        std::cout << "Function call to " << name << " with arguments: ";
        for (const auto &arg : arguments) {
            std::cout << arg.value << " ";
        }
        std::cout << std::endl;

        // Validate function call parameter types
        for (size_t i = 0; i < arguments.size(); i++) {
            if (std::get<Function>(globalSymbolTable[name]).parameters[i] != arguments[i].type && std::get<Function>(globalSymbolTable[name]).parameters[i] != "any") {
                errInfo = { ErrorType::INVALID_TYPE, (*tokens)[pos].line, (*tokens)[pos].column, unfilteredLines[(*tokens)[pos].line], "Valid type", currfilePath };
                error::gen(errInfo);
            }
        }

        // create a copy of the global symbol table and add the function arguments to it
        std::unordered_map<std::string, SymbolInfo> localSymbolTable = globalSymbolTable;

        for (size_t i = 0; i < arguments.size(); i++) {
            localSymbolTable[std::get<Function>(globalSymbolTable[name]).localVariables[i].identifier] = arguments[i];
        }

        Parser parser(std::make_unique<std::vector<Token>>(std::get<Function>(globalSymbolTable[name]).body), std::make_unique<std::vector<Token>>(std::get<Function>(globalSymbolTable[name]).unfilteredBody), filePath, name);
        parser.set_globalSymbolTable(localSymbolTable);
        parser.parse();
    }

    void parseScopedFunctionCall(int &pos, const Function& func) {
        const std::string name = ascii::_aname PARGS // Function name
        const std::vector<Variable> arguments = abstract::_pcall_params(pos, *tokens, globalSymbolTable, *unfilteredTokens);

        // Validate function call parameter types
        for (size_t i = 0; i < arguments.size(); i++) {
            if (func.parameters[i] != arguments[i].type && func.parameters[i] != "any") {
                errInfo = { ErrorType::INVALID_TYPE, (*tokens)[pos].line, (*tokens)[pos].column, unfilteredLines[(*tokens)[pos].line], "Valid type", currfilePath };
                error::gen(errInfo);
            }
        }

        std::unordered_map<std::string, SymbolInfo> localSymbolTable = globalSymbolTable;
        for (size_t i = 0; i < arguments.size(); i++) {
            localSymbolTable[func.localVariables[i].identifier] = arguments[i];
        }

        Parser parser(std::make_unique<std::vector<Token>>(func.body), std::make_unique<std::vector<Token>>(func.unfilteredBody), filePath, name);
        parser.set_globalSymbolTable(localSymbolTable);
        parser.parse();
    }

    void scope_resolve(int &pos) {
        if (const std::variant<Variable, Function> result = abstract::_pscope_resolve(pos, *tokens, globalSymbolTable);
            std::holds_alternative<Variable>(result)) {

            std::cout << "Variable found" << std::endl;
        } else if (std::holds_alternative<Function>(result)) {
            parseScopedFunctionCall(pos, std::get<Function>(result));
        }
    }

    void parse() {
        for (currentToken = 0; currentToken < tokens->size(); ++currentToken) {
            if ((*tokens)[currentToken].type == TokenType::KEYWORD) {
                if ((*tokens)[currentToken].value == "fn")
                    parseFunction(currentToken);
                else if ((*tokens)[currentToken].value == "var")
                    parseVariable(currentToken);
                else if ((*tokens)[currentToken].value == "merge")
                    parseMerge(currentToken);
                else if ((*tokens)[currentToken].value == "extern")
                    parseExtern(currentToken);
                else if ((*tokens)[currentToken].value == "if") {
                    parseIf(currentToken);
                }
            }

            if ((*tokens)[currentToken].type == TokenType::IDENTIFIER) {
                auto it = globalSymbolTable.find((*tokens)[currentToken].value);
                if (it == globalSymbolTable.end()) {
                    // Do nothing
                } else {
                    // Match found
                    if (std::holds_alternative<Function>(it->second)) {
                        // Function
                        std::cout << "Function found" << std::endl;
                        parseFunctionCall(currentToken);
                    } else if (std::holds_alternative<Variable>(it->second)) {
                        // Variable
                        std::cout << "Variable found" << std::endl;
                    } else if (std::holds_alternative<Namespace>(it->second)) {
                        // Namespace
                        std::cout << "Namespace found" << std::endl;
                        scope_resolve(currentToken);
                    }
                }
            }

            if ((*tokens)[currentToken].type == TokenType::eof) {
                return;
            }
        }
    }

    [[nodiscard]] std::unordered_map<std::string, SymbolInfo> getSymbolTable() {
        return globalSymbolTable;
    }

    void set_globalSymbolTable(const std::unordered_map<std::string, SymbolInfo>& symbolTable) {
        globalSymbolTable = symbolTable;
    }
};