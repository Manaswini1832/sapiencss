#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

#include "Token.h"

using namespace std;

class Lexer
{
    string source;
    char currChar;
    int currPos;

public:
    Lexer(string source) : source(source), currPos(-1)
    {
        nextChar();
    }

    void nextChar()
    {
        currPos++;
        if (currPos >= source.length())
        {
            currChar = '\0'; // EOF
        }
        else
        {
            currChar = source[currPos];
        }
    }

    char peek()
    {
        if (currPos + 1 >= source.length())
            return '\0';
        return source[currPos + 1];
    }

    void skipComments()
    {
        if (currChar == '#')
        {
            while (currChar != '\n')
                nextChar();
        }
    }

    Token *getStringToken()
    {
        int startPos = currPos;
        while (currChar != '"' && currChar != '\0')
        {
            nextChar();
        }

        if (currChar == '\0')
        {
            cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        nextChar();

        return new Token(STRING, source.substr(startPos, currPos - startPos - 1));
    }

    Token *getKeyWordToken()
    {
        int startPos = currPos;
        while (isupper(currChar) && currChar != '\0')
        {
            nextChar();
        }

        if (currChar == '\0')
        {
            cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        return new Token(KEYWORD, source.substr(startPos, currPos - startPos));
    }

    Token *getAttributeToken()
    {
        int startPos = currPos;
        while (!isspace(currChar) && currChar != '\0')
        {
            if (currChar == '\n')
            {
                cerr << "Missing space after attribute" << endl;
                return nullptr;
            }
            nextChar();
        }

        if (currChar == '\0')
        {
            cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        return new Token(ATTRIBUTE, source.substr(startPos, currPos - startPos));
    }

    Token *getToken()
    {
        skipComments();

        // skip whitespace, comma, semicolon
        while (isspace(currChar) || currChar == ',' || currChar == ';')
        {
            nextChar();
        }

        if (currChar == '"')
        {
            nextChar();
            return getStringToken();
        }
        if (currChar == '\0')
        {
            return new Token(END_OF_FILE, "\0");
        }
        if (currChar == '\n')
        {
            return new Token(NEWLINE, "\n");
        }

        // Handle keywords and attributes
        if (isupper(currChar))
        {
            return getKeyWordToken();
        }
        if (isalpha(currChar))
        {
            return getAttributeToken();
        }

        return nullptr;
    }

    // getters
    char getCurrChar()
    {
        return currChar;
    }
};

#endif