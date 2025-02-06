#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "Lexer.h"

using namespace std;

class Parser
{
    Lexer *lexer;
    Token *currToken;
    Token *peekToken;

public:
    void nextToken()
    {
        currToken = peekToken;
        peekToken = lexer->getToken();
    }

    // Constructor
    Parser(Lexer *lexer) : lexer(lexer), currToken(nullptr), peekToken(nullptr)
    {
        nextToken();
        nextToken();
    }

    // Return true if the current token matches
    bool checkToken(TOKENTYPE tokType)
    {
        return tokType == currToken->getTokenType();
    }

    // Return true if the next token matches
    bool checkPeek(TOKENTYPE tokType)
    {
        return tokType == peekToken->getTokenType();
    }

    // match current token. else, error. advance current token
    void match(TOKENTYPE tokType)
    {
        if (!checkToken(tokType))
            cerr << "Expected " << Token::getTypeString(tokType) << ", got " << Token::getTypeString(currToken->getTokenType()) << endl;
        nextToken();
    }

    void shape() {}

    void attributes() {}

    void attribute_name() {}

    void value()
    {
        cout << "Value string" << endl;
        match(STRING);
    }

    void nl()
    {
        cout << "NEWLINE" << endl;
        match(NEWLINE);

        while (checkToken(NEWLINE))
            nextToken();
    }

    void statement() {}
};

#endif
