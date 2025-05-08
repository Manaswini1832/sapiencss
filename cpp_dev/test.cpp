#include <iostream>
#include <string>
#include "headers/createSapienCSSCode.h"
using namespace std;

int main()
{
    // sapienCSS input
    string srcCode = "\
    MAKE RECTANGLE \"r1\" WITH color \"red\", x \"10\", y \"10\", width \"5\", height \"10\";\n\
    MAKE CIRCLE \"c1\" WITH color \"yellow\", x \"20\", y \"30\", radius \"5\";\n\
    MAKE LINE \"l1\" WITH color \"teal\", x \"5\", y \"15\", length \"15\";\n\
    ";

    // creating sapien css code
    string outputCode = createSapienCSSCode(srcCode);

    // canvas js output
    cout << outputCode << endl;

    return 0;
}

// emsdk_env.bat
// emcc sapienCSSCompiler.cpp -o sapienCSSCompiler.js -s WASM=1 -s EXPORTED_FUNCTIONS="['_subtract', '_sapienCSSCompiler', '_freeResult']" -s EXPORTED_RUNTIME_METHODS="['cwrap','UTF8ToString']"
// MAKE RECTANGLE "r1" WITH color "red", x "10", y "10", width "5", height "10";
// MAKE CIRCLE "c1" WITH color "yellow", x "20", y "30", radius "5";
// MAKE LINE "l1" WITH color "teal", x "5", y "15", length "15";