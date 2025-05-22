#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Token.h"
#include "Lexer.h"
#include "Emitter.h"

using namespace std;

// Parser Class
class Parser
{
    Lexer *lexer;
    Emitter *emitter;
    Token *currToken;
    Token *peekToken;
    string errorMessage = "";
    bool errorBool = false;
    unordered_map<string, string> values; // stores attribute values read while parsing a line in string form

public:
    ~Parser()
    {
        values.clear();
        errorBool = false;
        errorMessage = "";
    }

    void nextToken()
    {
        currToken = peekToken;
        peekToken = lexer->getToken(errorBool, errorMessage);
        if (!peekToken)
        {
            peekToken = new Token(INVALID, "INVALID_TOKEN");
        }
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
        {
            // cerr << "Expected " << Token::getTypeString(tokType) << ", got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Missing " + Token::getTypeString(tokType) + ".";
            return;
        }
        nextToken();
    }

    // Rule: program ::= {statement}
    string program()
    {
        emitter->headerLine("function draw() {");
        emitter->headerLine("  const canvas = document.getElementById(\"canvas\");");
        emitter->headerLine("  if (canvas.getContext) {");
        emitter->headerLine("    const ctx = canvas.getContext(\"2d\");");
        emitter->headerLine("    ctx.clearRect(0, 0, canvas.width, canvas.height);");
        statement();
        if (errorBool)
        {
            emitter->emitLine("\n    // ERROR : " + errorMessage);
            return errorMessage;
        }
        emitter->emitLine("  }");
        emitter->emitLine("}");
        emitter->emitLine("draw();");
        return errorMessage;
    }

    // Rule: statement ::= "MAKE" "SHAPE" "IDENTIFIER" "WITH" attributes SEMI_COLON nl
    void statement()
    {
        while (peekToken->getTokenType() != END_OF_FILE)
        {

            if (currToken && currToken->getTokenWord() == "#")
            {
                nextToken();
            }

            make();
            if (errorBool)
                return;
            shape();
            if (errorBool)
                return;
            identifier();
            if (errorBool)
                return;
            match(WITH);
            if (errorBool)
                return;
            attributes();
            if (errorBool)
                return;
            match(SEMI_COLON);
            if (errorBool)
                return;
            nl();
        }
    }

    void make()
    {
        if (checkToken(MAKE))
        {
            nextToken();
        }
        else
        {
            // cerr << "Expected make, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Expected MAKE got " + currToken->getTokenWord() + " instead.";
            return;
        }
    }

    void shape()
    {
        if (checkToken(SHAPE))
        {
            values["shape"] = currToken->getTokenWord();
            nextToken();
        }
        else
        {
            // cerr << "Expected shape, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Invalid shape : " + currToken->getTokenWord() + ". SapienCSS currently supports : RECTANGLE, CIRCLE, LINE.";
            return;
        }
    }

    void identifier()
    {
        if (checkToken(IDENTIFIER))
        {
            values["identifier"] = currToken->getTokenWord();
            nextToken();
        }
        else
        {
            // cerr << "Expected identifier, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Expected identifier but got " + currToken->getTokenWord() + " instead.";
            return;
        }
    }

    // Rule: attributes ::= attribute (, attribute)*
    void attributes()
    {
        attribute();
        if (errorBool)
            return;
        if (!values.empty() && !checkToken(COMMA))
        {
            errorBool = true;
            errorMessage = "MISSING COMMA";
            return;
        }

        while (checkToken(COMMA))
        {
            nextToken(); // Consume COMMA
            attribute();
            if (errorBool)
                return;
        }

        if (!values.empty() && !checkToken(COMMA) && !checkToken(SEMI_COLON))
        {
            errorBool = true;
            if (currToken->getTokenWord() == "NEWLINE")
                errorMessage = "MISSING SEMICOLON";
            else
                errorMessage = "MISSING COMMA";
            return;
        }
    }

    // Rule: attribute ::= attribute_name value
    void attribute()
    {
        string attribute_entry_name;
        attribute_name(attribute_entry_name);
        if (errorBool)
            return;
        value(attribute_entry_name);
        if (errorBool)
            return;
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
            attribute_entry_name = currToken->getTokenWord();
            nextToken(); // Consume the valid attribute name
        }
        else
        {
            // cerr << "Expected a valid attribute name, but got \"" << attributeName << "\"" << endl;
            errorBool = true;
            errorMessage = "Expected a valid attribute name, but got \"" + attributeName + "\". Please refer to the docs for a list of supported attributes.";
            return;
        }
    }

    // Rule: value ::= string
    void value(string &attribute_entry_name)
    {
        if (checkToken(STRING))
        {
            values[attribute_entry_name] = currToken->getTokenWord();
            nextToken(); // Consume the string value
        }
        else
        {
            // cerr << "Expected a value (STRING), but got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Expected a valid value, but got \"" + currToken->getTokenWord() + "\". Please refer to the docs for the correct way to give values to attributes.";
            return;
        }
    }

    // Rule: nl ::= "\n"
    void nl()
    {
        if (checkToken(NEWLINE))
        {
            makeShape();
            values.clear();
            nextToken(); // Consume the newline token
        }
        else
        {
            // cerr << "Expected newline, but got " << Token::getTypeString(currToken->getTokenType()) << endl;
            errorBool = true;
            errorMessage = "Expected newline, but got " + Token::getTypeString(currToken->getTokenType());
            return;
        }

        while (checkToken(NEWLINE))
            nextToken(); // Consume additional newlines if any
    }

    void makeShape()
    {
        emitter->emitLine("\n    // " + values["identifier"]); // id of the shape will be put as a comment
        if (values["shape"] == "RECTANGLE")
        {
            emitter->emitLine("    let x = " + values["x"] + ", y = " + values["y"] + ", width = " + values["width"] + ", height = " + values["height"] + ";");
            emitter->emitLine("    ctx.fillStyle = \"" + values["color"] + "\";");
            emitter->emitLine("    ctx.fillRect(x, y, width, height);");
        }
        else if (values["shape"] == "CIRCLE")
        {
            emitter->emitLine("    x = " + values["x"] + "; y = " + values["y"] + ";");
            emitter->emitLine("    let radius = " + values["radius"] + ";");
            emitter->emitLine("    ctx.beginPath();");
            emitter->emitLine("    ctx.arc(x, y, radius, 0, 2 * Math.PI);");
            emitter->emitLine("    ctx.fillStyle = \"" + values["color"] + "\";");
            emitter->emitLine("    ctx.fill();");
        }
        else if (values["shape"] == "LINE")
        {
            emitter->emitLine("    x = " + values["x"] + "; y = " + values["y"] + ";");
            emitter->emitLine("    let length = " + values["length"] + ";");
            emitter->emitLine("    ctx.beginPath();");
            emitter->emitLine("    ctx.moveTo(x, y);");
            emitter->emitLine("    ctx.lineTo(x + length, y);");
            emitter->emitLine("    ctx.strokeStyle = \"" + values["color"] + "\";");
            emitter->emitLine("    ctx.stroke();");
        }
        else
        {
            emitter->emitLine("    // ERROR : Invalid syntax. Can't draw this shape");
        }
    }
};

#endif
