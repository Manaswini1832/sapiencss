#include <iostream>
#include <string>
#include "createSapienCSSCode.h"
using namespace std;

int main()
{
    // sapienCSS input
    string srcCode = "\
    MAKE RECTANGLE \"r1\" WITH color \"orange\", x \"10\", y \"10\", width \"5\", height \"10\";\n\
    MAKE RECTANGLE \"r2\" WITH color \"red\", x \"10\", y \"10\", width \"5\", height \"10\";\n\
    ";

    // creating sapien css code
    string outputCode = createSapienCSSCode(srcCode);

    // canvas js output
    cout << outputCode << endl;

    return 0;
}