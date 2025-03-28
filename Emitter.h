#ifndef EMITTER_H
#define EMITTER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Token.h"
#include "Lexer.h"

using namespace std;

class Emitter
{
    string fullPath;
    string header = "";
    string code = "";

public:
    Emitter(string fullPath) : fullPath(fullPath) {}

    void emit(string in_code)
    {
        this->code += in_code;
    }

    void emitLine(string in_code)
    {
        this->code += in_code;
        this->code += "\n";
    }

    void headerLine(string in_code)
    {
        this->header += in_code; // Corrected line
        this->header += "\n";
    }

    void writeFile()
    {
        // with open(self.fullPath, 'w') as outputFile:
        //     outputFile.write(self.header + self.code)
        /*cout << "------------------------------------------" << endl;
        cout << "FINAL CODE" << endl;
        cout << "------------------------------------------" << endl;*/
        cout << header << endl;
        cout << code << endl;
    }
};

#endif
