#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "createSapienCSSCode.h"
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

    // creating sapien css code
    string outputCode = createSapienCSSCode(srcCode);

    cout << "From test main file" << endl;
    cout << outputCode << endl;

    return 0;
}