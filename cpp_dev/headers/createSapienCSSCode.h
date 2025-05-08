#ifndef CREATE_SAPIEN_CSS_CODE_H
#define CREATE_SAPIEN_CSS_CODE_H

#include <iostream>
#include <string>

#include "Lexer.h"
#include "Parser.h"
#include "Emitter.h"

using namespace std;

string createSapienCSSCode(string srcCode)
{
    Lexer *lexer = new Lexer(srcCode);
    Emitter *emitter = new Emitter("output.js");

    Parser *parser = new Parser(lexer, emitter);
    parser->program();

    string output = emitter->writeFile();

    return output;
}

#endif