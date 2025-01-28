#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Lexer.h"

using namespace std;

int main()
{
    string srcCode, line;

    // reading file input
    string filePath = "sapcss.txt";
    ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "Couldn't open source code" << endl;
        return 1;
    }

    while (getline(file, line))
    {
        srcCode += line;
        srcCode += '\n';
    }

    // lexer working on the source code from the file
    Lexer *lexer = new Lexer(srcCode);

    Token *token = lexer->getToken();
    if (token == nullptr)
    {
        cerr << "Error: Failed to read token!" << endl;
        return 1;
    }
    TOKENTYPE type = token->getTokenType();
    string word = token->getTokenWord();
    string typeString = token->getTypeString(type);

    while (type != END_OF_FILE)
    {
        cout << word << " : " << typeString << endl;

        token = lexer->getToken();
        if (token == nullptr)
        {
            cout << lexer->getCurrChar() << endl;
            cerr << "Error: Failed to read token!" << endl;
            return 1;
        }
        type = token->getTokenType();
        word = token->getTokenWord();
        typeString = token->getTypeString(type);
    }

    return 0;
}