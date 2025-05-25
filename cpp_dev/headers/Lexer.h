#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

#include "Token.h"

using namespace std;

class Lexer
{
    string source;
    char currChar;
    int currPos;

    static const unordered_set<string> validShapes;
    bool withKeywordReaced;

public:
    Lexer(string source) : source(source), currPos(-1)
    {
        withKeywordReaced = false;
        nextChar();
    }

    void nextChar()
    {
        currPos++;
        if (currPos >= (int)(source.length()))
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
        if (currPos + 1 >= (int)(source.length()))
            return '\0';
        return source[currPos + 1];
    }

    // debug
    void printAllTokens()
    {
        cout << "PRINTING ALL TOKENS" << endl;
        int p = currPos;
        while (p < source.length())
        {
            cout << source[p] << endl;
            p++;
        }
    }

    void skipComments()
    {
        if (currChar == '#')
        {
            while (currChar != '\n')
            {
                nextChar();
            }
            nextChar();
        }
    }

    bool isShape(string &str)
    {
        string cleanedStr;

        for (auto c : str)
        {
            if (c != ' ')
                cleanedStr += c;
        }
        return validShapes.find(cleanedStr) != validShapes.end();
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
            // cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        nextChar();

        string tokenStr = source.substr(startPos, currPos - startPos - 1);
        if (!withKeywordReaced)
            return new Token(IDENTIFIER, tokenStr);
        return new Token(STRING, tokenStr);
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
            // cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        string tokenStr = source.substr(startPos, currPos - startPos);

        if (isShape(tokenStr))
            return new Token(SHAPE, tokenStr);
        if (tokenStr == "MAKE")
            return new Token(MAKE, tokenStr);
        if (tokenStr == "WITH")
        {
            withKeywordReaced = true;
            return new Token(WITH, tokenStr);
        }

        return new Token(INVALID, tokenStr);
    }

    Token *getAttributeToken()
    {
        int startPos = currPos;
        while (!isspace(currChar) && currChar != '\0')
        {
            if (currChar == '\n')
            {
                // cerr << "Missing space after attribute" << endl;
                return nullptr;
            }
            nextChar();
        }

        if (currChar == '\0')
        {
            // cerr << "Error: Unterminated string literal!" << endl;
            return nullptr;
        }

        return new Token(ATTRIBUTE, source.substr(startPos, currPos - startPos));
    }

    Token *getToken(bool &errorBool, string &errorMessage)
    {
        skipComments();

        // skip whitespace, comma, semicolon
        while ((isspace(currChar) && currChar != '\n'))
        {
            nextChar();
        }
        if (currChar == '\n')
        {
            while (currChar == '\n')
            {
                nextChar();
            }
            return new Token(NEWLINE, "NEWLINE");
        }
        if (currChar == ',')
        {
            nextChar();
            return new Token(COMMA, ",");
        }
        if (currChar == ';')
        {
            nextChar();
            withKeywordReaced = false;
            return new Token(SEMI_COLON, ";");
        }
        if (currChar == '"')
        {
            nextChar();
            return getStringToken();
        }
        if (currChar == '\0')
        {
            return new Token(END_OF_FILE, "END_OF_FILE");
        }
        if (currChar == '#')
        {
            skipComments();
            while (currChar == '#')
            {
                skipComments();
            }
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

// Static definition of valid shapes
const unordered_set<string> Lexer::validShapes = {"RECTANGLE", "CIRCLE", "LINE"};

#endif