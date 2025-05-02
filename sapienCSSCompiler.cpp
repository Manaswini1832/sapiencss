#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Token Class
enum TOKENTYPE
{
    SHAPE,
    STRING,
    MAKE,
    WITH,
    IDENTIFIER,
    ATTRIBUTE,
    END_OF_FILE,
    NEWLINE,
    COMMA,
    SEMI_COLON
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

    static string getTypeString(TOKENTYPE type)
    {
        switch (type)
        {
        case SHAPE:
            return "SHAPE";
        case STRING:
            return "STRING";
        case MAKE:
            return "MAKE";
        case WITH:
            return "WITH";
        case IDENTIFIER:
            return "IDENTIFIER";
        case ATTRIBUTE:
            return "ATTRIBUTE";
        case END_OF_FILE:
            return "END_OF_FILE";
        case NEWLINE:
            return "NEWLINE";
        case SEMI_COLON:
            return "SEMI_COLON";
        case COMMA:
            return "COMMA";
        default:
            break;
        }
        return "";
    }
};

// Lexer Class
class Lexer
{
    string source;
    char currChar;
    int currPos;

    static const unordered_set<string> validShapes;
    static bool withKeywordReaced;

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
            cerr << "Error: Unterminated string literal!" << endl;
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
            cerr << "Error: Unterminated string literal!" << endl;
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

        return nullptr;
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
        while ((isspace(currChar) && currChar != '\n'))
        {
            nextChar();
        }
        if (currChar == '\n')
        {
            nextChar();
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
bool Lexer::withKeywordReaced = false;

// Emitter Class
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

    string writeFile()
    {
        string output = header + code;
        return output;
    }
};

// Parser Class
class Parser
{
    Lexer *lexer;
    Emitter *emitter;
    Token *currToken;
    Token *peekToken;
    unordered_map<string, string> values; // stores attribute values read while parsing a line in string form

public:
    void nextToken()
    {
        currToken = peekToken;
        peekToken = lexer->getToken();
    }

    // Constructor
    Parser(Lexer *lexer, Emitter *emitter) : lexer(lexer), emitter(emitter), currToken(nullptr), peekToken(nullptr)
    {
        nextToken();
        nextToken();
    }

    // Return true if the current token matches
    bool checkToken(TOKENTYPE tokType)
    {
        return tokType == currToken->getTokenType();
    }

    // Return true if the next token matches
    bool checkPeek(TOKENTYPE tokType)
    {
        return tokType == peekToken->getTokenType();
    }

    // match current token. else, error. advance current token
    void match(TOKENTYPE tokType)
    {
        if (!checkToken(tokType))
            cerr << "Expected " << Token::getTypeString(tokType) << ", got " << Token::getTypeString(currToken->getTokenType()) << endl;
        else
        {
            // cout << Token::getTypeString(tokType) << endl;
        }
        nextToken();
    }

    // Rule: program ::= {statement}
    void program()
    {
        // cout << "PROGRAM" << endl;
        emitter->headerLine("function draw() {");
        emitter->headerLine("  const canvas = document.getElementById(\"canvas\");");
        emitter->headerLine("  if (canvas.getContext) {");
        emitter->headerLine("    const ctx = canvas.getContext(\"2d\");");
        statement();
        // cout << "After statement" << endl;
        emitter->emitLine("  }");
        emitter->emitLine("}");
        emitter->emitLine("draw();");
    }

    // Rule: statement ::= "MAKE" "SHAPE" "IDENTIFIER" "WITH" attributes SEMI_COLON nl
    void statement()
    {
        while (peekToken->getTokenType() != END_OF_FILE)
        {
            make();
            shape();
            identifier();
            match(WITH);
            attributes();
            match(SEMI_COLON);
            nl();
        }
    }

    void make()
    {
        // cout << "STATEMENT-MAKE" << endl;
        if (checkToken(MAKE))
        {
            nextToken();
        }
        else
        {
            cerr << "Expected make, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    void shape()
    {
        if (checkToken(SHAPE))
        {
            // cout << "SHAPE" << endl;
            values["shape"] = currToken->getTokenWord();
            nextToken();
        }
        else
        {
            cerr << "Expected shape, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    void identifier()
    {
        if (checkToken(IDENTIFIER))
        {
            // cout << "IDENTIFIER" << endl;
            values["identifier"] = currToken->getTokenWord();
            nextToken();
        }
        else
        {
            cerr << "Expected identifier, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    // Rule: attributes ::= attribute (, attribute)*
    void attributes()
    {
        attribute();
        while (checkToken(COMMA))
        {
            nextToken(); // Consume COMMA
            attribute();
        }
    }

    // Rule: attribute ::= attribute_name value
    void attribute()
    {
        string attribute_entry_name;
        attribute_name(attribute_entry_name);
        value(attribute_entry_name);
    }

    // Rule: attribute_name ::= "color" | "width" | "height" | "x" | "y" | "radius" | "length" | "rotate"
    void attribute_name(string &attribute_entry_name)
    {
        // Define a list of valid attribute names
        vector<string> validAttributes = {
            "color", "width", "height", "x", "y", "radius", "length", "rotate", "margin", "padding" // Add more attributes as needed
        };

        // Check if the current token is a string and matches one of the valid attribute names
        string attributeName = currToken->getTokenWord(); // Assuming Token has a method to get the token's string value

        // Check if the attributeName is in the list of valid attributes
        if (find(validAttributes.begin(), validAttributes.end(), attributeName) != validAttributes.end())
        {
            // cout << "ATTRIBUTE" << endl;
            attribute_entry_name = currToken->getTokenWord();
            nextToken(); // Consume the valid attribute name
        }
        else
        {
            cerr << "Expected a valid attribute name, but got \"" << attributeName << "\"" << endl;
        }
    }

    // Rule: value ::= string
    void value(string &attribute_entry_name)
    {
        if (checkToken(STRING))
        {
            // cout << "VALUE = " << endl;
            values[attribute_entry_name] = currToken->getTokenWord();
            nextToken(); // Consume the string value
        }
        else
        {
            cerr << "Expected a value (STRING), but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }
    }

    // Rule: nl ::= "\n"
    void nl()
    {
        // cout << "NEWLINE" << endl;
        if (checkToken(NEWLINE))
        {
            makeShape();
            values.clear();
            nextToken(); // Consume the newline token
        }
        else
        {
            cerr << "Expected newline, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
        }

        while (checkToken(NEWLINE))
            nextToken(); // Consume additional newlines if any
    }

    void makeShape()
    {
        emitter->emitLine("// " + values["identifier"]); // id of the shape will be put as a comment
        if (values["shape"] == "RECTANGLE")
        {
            emitter->emitLine("let x = " + values["x"] + ", y = " + values["y"] + ", width = " + values["width"] + ", height = " + values["height"] + ";");
            emitter->emitLine("ctx.fillStyle = \"" + values["color"] + "\";");
            emitter->emitLine("ctx.fillRect(x, y, width, height);");
        }
        else if (values["shape"] == "CIRCLE")
        {
            emitter->emitLine("x = " + values["x"] + "; y = " + values["y"] + ";");
            emitter->emitLine("let radius = " + values["radius"] + ";");
            emitter->emitLine("ctx.beginPath();");
            emitter->emitLine("ctx.arc(x, y, radius, 0, 2 * Math.PI);");
            emitter->emitLine("ctx.fillStyle = \"" + values["color"] + "\";");
            emitter->emitLine("ctx.fill();");
        }
        else if (values["shape"] == "LINE")
        {

            emitter->emitLine("x = " + values["x"] + "; y = " + values["y"] + ";");
            emitter->emitLine("let length = " + values["length"] + ";");
            emitter->emitLine("ctx.beginPath();");
            emitter->emitLine("ctx.moveTo(x, y);");
            emitter->emitLine("ctx.lineTo(x + length, y);");
            emitter->emitLine("ctx.strokeStyle = \"" + values["color"] + "\";");
            emitter->emitLine("ctx.stroke();");
        }
        else
        {
            emitter->emitLine("//Invalid syntax. Can't draw this shape");
        }
    }
};

string sapienCSSCompiler(string srcCode)
{
    Lexer *lexer = new Lexer(srcCode);
    Emitter *emitter = new Emitter("output.js");

    Parser *parser = new Parser(lexer, emitter);
    parser->program();

    string output = emitter->writeFile();

    return output;
}