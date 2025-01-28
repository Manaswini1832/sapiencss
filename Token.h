#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum TOKENTYPE
{
    STRING,
    KEYWORD,
    ATTRIBUTE,
    END_OF_FILE,
    NEWLINE
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

    string getTypeString(TOKENTYPE type)
    {
        switch (type)
        {
        case STRING:
            return "STRING";
        case KEYWORD:
            return "KEYWORD";
        case ATTRIBUTE:
            return "ATTRIBUTE";
        case END_OF_FILE:
            return "END_OF_FILE";
        case NEWLINE:
            return "NEWLINE";
        default:
            break;
        }
        return "";
    }
};

#endif