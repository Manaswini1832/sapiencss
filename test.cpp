#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"
#include "Emitter.h"

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

    Lexer *lexer = new Lexer(srcCode);
    Emitter *emitter = new Emitter("output.js");

    Parser *parser = new Parser(lexer, emitter);
    parser->program();

    emitter->writeFile();

    // cout << "Compiling completed" << endl;

    // LEXER testing
    /*Token *token = lexer->getToken();
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
        if (type == MAKE)
            cout << "------------------------" << endl;
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
    }*/

    return 0;
}