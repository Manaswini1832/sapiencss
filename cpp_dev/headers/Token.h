#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum TOKENTYPE
{
    SHAPE,
    STRING,
    MAKE,
    WITH,
    IDENTIFIER,
    ATTRIBUTE,
    END_OF_FILE,
    NEWLINE,
    COMMA,
    SEMI_COLON
};

class Token
{
    TOKENTYPE tokenType;
    string word;

public:
    Token(TOKENTYPE tokenType, string word) : tokenType(tokenType), word(word) {}

    TOKENTYPE getTokenType()
    {
        return tokenType;
    }

    string getTokenWord()
    {
        return word;
    }

    static string getTypeString(TOKENTYPE type)
    {
        switch (type)
        {
        case SHAPE:
            return "SHAPE";
        case STRING:
            return "STRING";
        case MAKE:
            return "MAKE";
        case WITH:
            return "WITH";
        case IDENTIFIER:
            return "IDENTIFIER";
        case ATTRIBUTE:
            return "ATTRIBUTE";
        case END_OF_FILE:
            return "END_OF_FILE";
        case NEWLINE:
            return "NEWLINE";
        case SEMI_COLON:
            return "SEMI_COLON";
        case COMMA:
            return "COMMA";
        default:
            break;
        }
        return "";
    }
};

#endif