#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

    // Rule: program ::= {statement}
    void program()
    {
        cout << "PROGRAM" << endl;
        statement();
    }

    // Rule: statement ::= "MAKE" "SHAPE" "IDENTIFIER" "WITH" attributes SEMI_COLON nl
    void statement()
    {
        make();
        match(SHAPE);
        match(IDENTIFIER);
        match(WITH);
        attributes();
        match(SEMI_COLON);
        nl();
    }

    void make()
    {
        cout << "STATEMENT-MAKE" << endl;
        if (checkToken(MAKE))
        {
            nextToken();
        }
        else
        {
            cerr << "Expected make, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    void shape()
    {
        if (checkToken(SHAPE))
        {
            nextToken();
        }
        else
        {
            cerr << "Expected shape, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    // Rule: attributes ::= attribute (, attribute)*
    void attributes()
    {
        attribute();
        while (checkToken(COMMA))
        {
            nextToken(); // Consume COMMA
            attribute();
        }
    }

    // Rule: attribute ::= attribute_name value
    void attribute()
    {
        attribute_name();
        value();
    }

    // Rule: attribute_name ::= "color" | "width" | "height" | "x" | "y" | "radius" | "length" | "rotate"
    void attribute_name()
    {
        // Define a list of valid attribute names
        vector<string> validAttributes = {
            "color", "width", "height", "x", "y", "radius", "length", "rotate", "margin", "padding" // Add more attributes as needed
        };

        // Check if the current token is a string and matches one of the valid attribute names
        if (checkToken(STRING))
        {
            string attributeName = currToken->getTokenWord(); // Assuming Token has a method to get the token's string value

            // Check if the attributeName is in the list of valid attributes
            if (find(validAttributes.begin(), validAttributes.end(), attributeName) != validAttributes.end())
            {
                nextToken(); // Consume the valid attribute name
            }
            else
            {
                cerr << "Expected a valid attribute name, but got \"" << attributeName << "\"" << endl;
            }
        }
        else
        {
            cerr << "Expected a string attribute name, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    // Rule: value ::= string
    void value()
    {
        if (checkToken(STRING))
        {
            nextToken(); // Consume the string value
        }
        else
        {
            cerr << "Expected a value (STRING), but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    // Rule: nl ::= "\n"
    void nl()
    {
        cout << "NEWLINE" << endl;
        if (checkToken(NEWLINE))
        {
            nextToken(); // Consume the newline token
        }
        else
        {
            cerr << "Expected newline, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }

        while (checkToken(NEWLINE))
            nextToken(); // Consume additional newlines if any
    }
};

#endif
