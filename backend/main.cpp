/* UPDATE VERSION [45] */

/*
==================================================
Dependencies
==================================================
*/
#include "httplib.h"
#include "json.hpp"
#include "Tokenizer.h"
#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
using namespace std;
using json = nlohmann::json;

/*
Sample Code:
!This Is A Comment, The Language Only Supports Single-Line Comments

!Below Is An Example Of output() Using String Literals
output("[SYSTEM MESSAGE] Program Start!");
output("");

!Below Are The 5 Primitive Data Types (int, double, char, bool, string)
INTEGER integer1 = 100;
DECIMAL decimal1 = 12345.67890;
CHARACTER character1 = '@';
BOOLEAN boolean1 = TRUE;
STRING string1 = "Sample String 1";

!Below Is An Example Of output() Using Variables
output("[SYSTEM MESSAGE] Variables:");
output("[SYSTEM MESSAGE] integer1:");
output(integer1);
output("[SYSTEM MESSAGE] decimal1:");
output(decimal1);
output("[SYSTEM MESSAGE] character1:");
output(character1);
output("[SYSTEM MESSAGE] boolean1:");
output(boolean1);
output("[SYSTEM MESSAGE] string1:");
output(string1);
output("");

!Below Are The 5 Constant Primitive Data Types (int, double, char, bool, string)
CONSTANT INTEGER CONSTANT_INTEGER_1 = 1234567890;
CONSTANT DECIMAL CONSTANT_DECIMAL_1 = 67890.12345;
CONSTANT CHARACTER CONSTANT_CHARACTER_1 = '#';
CONSTANT BOOLEAN CONSTANT_BOOLEAN_1 = FALSE;
CONSTANT STRING CONSTANT_STRING_1 = "Sample Constant String 1";

!Below Is An Example Of output() Using Constants
output("[SYSTEM MESSAGE] Constants:");
output("[SYSTEM MESSAGE] CONSTANT_INTEGER_1:");
output(CONSTANT_INTEGER_1);
output("[SYSTEM MESSAGE] CONSTANT_DECIMAL_1:");
output(CONSTANT_DECIMAL_1);
output("[SYSTEM MESSAGE] CONSTANT_CHARACTER_1:");
output(CONSTANT_CHARACTER_1);
output("[SYSTEM MESSAGE] CONSTANT_BOOLEAN_1:");
output(CONSTANT_BOOLEAN_1);
output("[SYSTEM MESSAGE] CONSTANT_STRING_1:");
output(CONSTANT_STRING_1);
output("");

!Below Is An Example Of The input() Function
INTEGER integer2;
DECIMAL decimal2;
CHARACTER character2;
BOOLEAN boolean2;
STRING string2;

output("[SYSTEM MESSAGE] Please Input A Valid Integer For integer2:");
input(integer2);
output("[SYSTEM MESSAGE] Please Input A Valid Decimal For decimal2:");
input(decimal2);
output("[SYSTEM MESSAGE] Please Input A Valid Character For character2:");
input(character2);
output("[SYSTEM MESSAGE] Please Input A Valid Boolean For boolean2:");
input(boolean2);
output("[SYSTEM MESSAGE] Please Input A Valid String For string2:");
input(string2);

!Below Is An Example Of Assignment Operator
INTEGER _integer3 = 999;
DECIMAL _decimal3 = 888.777;
CHARACTER _character3 = '*';
BOOLEAN _boolean3 = FALSE;
STRING _string3 = "Sample String 3";
INTEGER integer4 = _integer3;
output("[SYSTEM MESSAGE] integer4:");
output(integer4);
DECIMAL decimal4 = _decimal3;
output("[SYSTEM MESSAGE] decimal4:");
output(decimal4);
CHARACTER character4 = _character3;
output("[SYSTEM MESSAGE] character4:");
output(character4);
BOOLEAN boolean4 = _boolean3;
output("[SYSTEM MESSAGE] boolean4:");
output(boolean4);
STRING string4 = _string3;
output("[SYSTEM MESSAGE] string4:");
output(string4);
integer4 = 666;
output("[SYSTEM MESSAGE] integer4:");
output(integer4);
decimal4 = 555.444;
output("[SYSTEM MESSAGE] decimal4:");
output(decimal4);
character4 = '$';
output("[SYSTEM MESSAGE] character4:");
output(character4);
boolean4 = TRUE;
output("[SYSTEM MESSAGE] boolean4:");
output(boolean4);
string4 = "Sample String 4";
output("[SYSTEM MESSAGE] string4:");
output(string4);
integer4 = _integer3;
output("[SYSTEM MESSAGE] integer4:");
output(integer4);
decimal4 = _decimal3;
output("[SYSTEM MESSAGE] decimal4:");
output(decimal4);
character4 = _character3;
output("[SYSTEM MESSAGE] character4:");
output(character4);
boolean4 = _boolean3;
output("[SYSTEM MESSAGE] boolean4:");
output(boolean4);
string4 = _string3;
output("[SYSTEM MESSAGE] string4:");
output(string4);
integer4 = CONSTANT_INTEGER_1;
output("[SYSTEM MESSAGE] integer4:");
output(integer4);
decimal4 = CONSTANT_DECIMAL_1;
output("[SYSTEM MESSAGE] decimal4:");
output(decimal4);
character4 = CONSTANT_CHARACTER_1;
output("[SYSTEM MESSAGE] character4:");
output(character4);
boolean4 = CONSTANT_BOOLEAN_1;
output("[SYSTEM MESSAGE] boolean4:");
output(boolean4);
string4 = CONSTANT_STRING_1;
output("[SYSTEM MESSAGE] string4:");
output(string4);

!Below Is An Example Of Basic Arithmetic (Addition, Subtraction, Multiplication, Division)
INTEGER varInt1 = 2500;
INTEGER varInt2 = 7500;
INTEGER varInt3 = 5000;
INTEGER varInt4 = 10000;

INTEGER arithmeticExample1;
output("arithmeticExample1:");
output(arithmeticExample1);
arithmeticExample1 = 123 + varInt1 - varInt2 * 456 / varInt3 + varInt4 - 789;
output("arithmeticExample1:");
output(arithmeticExample1);

INTEGER arithmeticExample2 = 123;
output("arithmeticExample2:");
output(arithmeticExample2);
arithmeticExample2 = varInt1 + 123 - varInt2 * 456 / varInt3 + 789 - varInt4;
output("arithmeticExample2:");
output(arithmeticExample2);

INTEGER arithmeticExample3 = 456;
output("arithmeticExample3:");
output(arithmeticExample3);
arithmeticExample3 = arithmeticExample3 + 123 - varInt1 * varInt2 / 456 + varInt3 - varInt4 * 789;
output("arithmeticExample3:");
output(arithmeticExample3);

DECIMAL varDec1 = 2500.0;
DECIMAL varDec2 = 7500.0;
DECIMAL varDec3 = 5000.0;
DECIMAL varDec4 = 10000.0;

DECIMAL arithmeticExample4;
output("arithmeticExample4:");
output(arithmeticExample4);
arithmeticExample4 = 123.0 + varDec1 - varDec2 * 456.0 / varDec3 + varDec4 - 789.0;
output("arithmeticExample4:");
output(arithmeticExample4);

DECIMAL arithmeticExample5 = 123.0;
output("arithmeticExample5:");
output(arithmeticExample5);
arithmeticExample5 = varDec1 + 123.0 - varDec2 * 456.0 / varDec3 + 789.0 - varDec4;
output("arithmeticExample5:");
output(arithmeticExample5);

DECIMAL arithmeticExample6 = 456.0;
output("arithmeticExample6:");
output(arithmeticExample6);
arithmeticExample6 = arithmeticExample6 + 123.0 - varDec1 * varDec2 / 456.0 + varDec3 - varDec4 * 789.0;
output("arithmeticExample6:");
output(arithmeticExample6);

STRING varStr1 = " Variable String 1 ";
STRING varStr2 = " Variable String 2 ";
STRING varStr3 = " Variable String 3 ";
STRING varStr4 = " Variable String 4 ";

STRING arithmeticExample7;
output("arithmeticExample7:");
output(arithmeticExample7);
arithmeticExample7 = " Literal String 1 " + varStr1 + varStr2 + " Literal String 2 " + varStr3 + varStr4 + " Literal String 3 ";
output("arithmeticExample7:");
output(arithmeticExample7);

STRING arithmeticExample8 = " Arithmetic String 1 ";
output("arithmeticExample8:");
output(arithmeticExample8);
arithmeticExample8 = varStr1 + " Literal String 1 " + varStr2 + " Literal String 2 " + varStr3 + " Literal String 2 " + varStr4;
output("arithmeticExample8:");
output(arithmeticExample8);

STRING arithmeticExample9 = " Arithmetic String 2 ";
output("arithmeticExample9:");
output(arithmeticExample9);
arithmeticExample9 = arithmeticExample9 + " Literal String 1 " + varStr1 + varStr2 + " Literal String 2 " + varStr3 + varStr4 + " Literal String 3 ";
output("arithmeticExample9:");
output(arithmeticExample9);

!Below Is An Example Of Control Flow (If, Else If, Else)
output("[SYSTEM MESSAGE] Program Start!");
INTEGER controlFlowInt1 = 1;
INTEGER controlFlowInt2 = 1;
INTEGER controlFlowInt3 = 2;
IF (controlFlowInt1 EQ controlFlowInt2) AND (controlFlowInt2 NE controlFlowInt3) OR (controlFlowInt1 NE controlFlowInt3) BRANCH;
    output("1. INSIDE IF STATEMENT!");
ELSE IF (1 EQ 1) AND (1.1 NE 2.2) OR ('A' GT 'Z') AND ("STRING A" LT "STRING B") OR (TRUE EQ FALSE) AND (5 GE 3) OR (7.7 LE 8.8) BRANCH;
    output("2. INSIDE ELSE IF STATEMENT!");
ELSE BRANCH;
    output("3. INSIDE ELSE STATEMENT!");
END;

!Below Is An Example Of Arrays
ARRAY INTEGER intArray1 = [];
ARRAY INTEGER intArray2 = [1,2,3,4,5,6,7,8,9,10];
ARRAY DECIMAL decArray1 = [];
ARRAY DECIMAL decArray2 = [1.1,2.2,3.3,4.4,5.5];
ARRAY CHARACTER charArray1 = [];
ARRAY CHARACTER charArray2 = ['A','B','C','D','E','F','G'];
ARRAY BOOLEAN boolArray1 = [];
ARRAY BOOLEAN boolArray2 = [TRUE,FALSE,TRUE,FALSE,TRUE];
ARRAY STRING strArray1 = [];
ARRAY STRING strArray2 = ["STRING 1","STRING 2","STRING 3"];

INTEGER index1 = 2;
CONSTANT INTEGER index2 = 2;

INTEGER intArray2Item1 = intArray2[0];
INTEGER intArray2Item2 = intArray2[index1];
DECIMAL decArray2Item1 = decArray2[0];
DECIMAL decArray2Item2 = decArray2[index1];
CHARACTER charArray2Item1 = charArray2[0];
CHARACTER charArray2Item2 = charArray2[index1];
BOOLEAN boolArray2Item1 = boolArray2[0];
BOOLEAN boolArray2Item2 = boolArray2[index1];
STRING strArray2Item1 = strArray2[0];
STRING strArray2Item2 = strArray2[index1];

CONSTANT INTEGER CONSTANT_intArray2Item1 = intArray2[0];
CONSTANT INTEGER CONSTANT_intArray2Item2 = intArray2[index2];
CONSTANT DECIMAL CONSTANT_decArray2Item1 = decArray2[0];
CONSTANT DECIMAL CONSTANT_decArray2Item2 = decArray2[index2];
CONSTANT CHARACTER CONSTANT_charArray2Item1 = charArray2[0];
CONSTANT CHARACTER CONSTANT_charArray2Item2 = charArray2[index2];
CONSTANT BOOLEAN CONSTANT_boolArray2Item1 = boolArray2[0];
CONSTANT BOOLEAN CONSTANT_boolArray2Item2 = boolArray2[index2];
CONSTANT STRING CONSTANT_strArray2Item1 = strArray2[0];
CONSTANT STRING CONSTANT_strArray2Item2 = strArray2[index2];

output("");
output("ASSIGNMENT OPERATOR:");
intArray2Item1 = intArray2[1];
intArray2Item1 = intArray2[index];
output("intArray2Item1:");
output(intArray2Item1);
output("intArray2Item1:");
output(intArray2Item1);

output("END!");

ARRAY INTEGER intArray2 = [1,2,3,4,5,6,7,8,9,10];
ARRAY DECIMAL decArray2 = [1.1,2.2,3.3,4.4,5.5];
ARRAY CHARACTER charArray2 = ['A','B','C','D','E','F','G'];
ARRAY BOOLEAN boolArray2 = [TRUE,FALSE,TRUE,FALSE,TRUE];
ARRAY STRING strArray2 = ["STRING 1","STRING 2","STRING 3"];

INTEGER index1 = 1;
CONSTANT INTEGER index2 = 2;

INTEGER intArray2Item;
DECIMAL decArray2Item;
CHARACTER charArray2Item;
BOOLEAN boolArray2Item;
STRING strArray2Item;

output("INTEGER:");
intArray2Item = intArray2[0];
output("intArray2Item [0]:");
output(intArray2Item);
intArray2Item = intArray2[index1];
output("intArray2Item [index1]:");
output(intArray2Item);
intArray2Item = intArray2[index2];
output("intArray2Item [index2]:");
output(intArray2Item);

output("DECIMAL:");
decArray2Item = decArray2[0];
output("decArray2Item [0]:");
output(decArray2Item);
decArray2Item = decArray2[index1];
output("decArray2Item [index1]:");
output(decArray2Item);
decArray2Item = decArray2[index2];
output("decArray2Item [index2]:");
output(decArray2Item);

output("CHARACTER:");
charArray2Item = charArray2[0];
output("charArray2Item [0]:");
output(charArray2Item);
charArray2Item = charArray2[index1];
output("charArray2Item [index1]:");
output(charArray2Item);
charArray2Item = charArray2[index2];
output("charArray2Item [index2]:");
output(charArray2Item);

output("BOOLEAN:");
boolArray2Item = boolArray2[0];
output("boolArray2Item [0]:");
output(boolArray2Item);
boolArray2Item = boolArray2[index1];
output("boolArray2Item [index1]:");
output(boolArray2Item);
boolArray2Item = boolArray2[index2];
output("boolArray2Item [index2]:");
output(boolArray2Item);

output("STRING:");
strArray2Item = strArray2[0];
output("strArray2Item [0]:");
output(strArray2Item);
strArray2Item = strArray2[index1];
output("strArray2Item [index1]:");
output(strArray2Item);
strArray2Item = strArray2[index2];
output("strArray2Item [index2]:");
output(strArray2Item);

output("END!");

ARRAY INTEGER intArray2 = [1,2,3,4,5,6,7,8,9,10];

FOR LOOP (index; index LT intArray2Size; index = index + 1;) BRANCH;

SPLIT BY ()

[ "FOR LOOP" , "INTEGER index; index LT intArray2Size; index = index + 1;" , "BRANCH;" ]

SPLIT BY ; OF "index; index LT intArray2Size; index = index + 1;"

[ "INTEGER index" , " index LT intArray2Size" , " index = index + 1"]

TRIMSTRING

[ "index" , "index LT intArray2Size" , "index = index + 1"]


FOR LOOP (INTEGER index = 0; index LT intArray2Size; index = index + 1;) BRANCH;
FOR LOOP (INTEGER index = 0; index LT intArray2Size; index = index - 1;) BRANCH;
FOR LOOP (INTEGER index = 0; index LT intArray2Size; index++;) BRANCH;
FOR LOOP (INTEGER index = 0; index LT intArray2Size; index--;) BRANCH;
...
...
...
CODE HERE
...
...
...
FOR END;

WHILE LOOP (controlFlowInt1 EQ controlFlowInt2) AND (controlFlowInt2 NE controlFlowInt3) OR (controlFlowInt1 NE controlFlowInt3) BRANCH;
...
...
...
CODE HERE
...
...
...
WHILE END;





output(intArray1);
output(intArray2);
output(decArray1);
output(decArray2);
output(charArray1);
output(charArray2);
output(boolArray1);
output(boolArray2);
output(strArray1);
output(strArray2);

INTEGER intArray1Size = size(intArray1);
INTEGER intArray2Size = size(intArray2);
INTEGER decArray1Size = size(decArray1);
INTEGER decArray2Size = size(decArray2);
INTEGER charArray1Size = size(charArray1);
INTEGER charArray2Size = size(charArray2);
INTEGER boolArray1Size = size(boolArray1);
INTEGER boolArray2Size = size(boolArray2);
INTEGER strArray1Size = size(strArray1);
INTEGER strArray2Size = size(strArray2);

output("");
output("INTEGERS:");
output("");

output("intArray1:");
output(intArray1Size);
output("intArray2:");
output(intArray2Size);
output("decArray1:");
output(decArray1Size);
output("decArray2:");
output(decArray2Size);
output("charArray1:");
output(charArray1Size);
output("charArray2:");
output(charArray2Size);
output("boolArray1:");
output(boolArray1Size);
output("boolArray2:");
output(boolArray2Size);
output("strArray1:");
output(strArray1Size);
output("strArray2:");
output(strArray2Size);

CONSTANT INTEGER CONSTANT_intArray1Size = size(intArray1);
CONSTANT INTEGER CONSTANT_intArray2Size = size(intArray2);
CONSTANT INTEGER CONSTANT_decArray1Size = size(decArray1);
CONSTANT INTEGER CONSTANT_decArray2Size = size(decArray2);
CONSTANT INTEGER CONSTANT_charArray1Size = size(charArray1);
CONSTANT INTEGER CONSTANT_charArray2Size = size(charArray2);
CONSTANT INTEGER CONSTANT_boolArray1Size = size(boolArray1);
CONSTANT INTEGER CONSTANT_boolArray2Size = size(boolArray2);
CONSTANT INTEGER CONSTANT_strArray1Size = size(strArray1);
CONSTANT INTEGER CONSTANT_strArray2Size = size(strArray2);

output("");
output("CONSTANT INTEGERS:");
output("");

output("intArray1:");
output(CONSTANT_intArray1Size);
output("intArray2:");
output(CONSTANT_intArray2Size);
output("decArray1:");
output(CONSTANT_decArray1Size);
output("decArray2:");
output(CONSTANT_decArray2Size);
output("charArray1:");
output(CONSTANT_charArray1Size);
output("charArray2:");
output(CONSTANT_charArray2Size);
output("boolArray1:");
output(CONSTANT_boolArray1Size);
output("boolArray2:");
output(CONSTANT_boolArray2Size);
output("strArray1:");
output(CONSTANT_strArray1Size);
output("strArray2:");
output(CONSTANT_strArray2Size);

!Below Is An Example Of output() Using String Literals
output("[SYSTEM MESSAGE] Program Terminated...");

Sample Input:

1234567890 , 12345.67890 , % , TRUE , Sample Input String
*/

/*
==================================================
Main() Function
==================================================
*/
int main()
{
    const int PORT = 8080;
    std::cout << "[SERVER] main.cpp Program Start!" << std::endl;
    httplib::Server server;
    server.Options("/execute", [](const httplib::Request&, httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        response.set_header("Access-Control-Allow-Headers", "Content-Type");
    });
    server.Post("/execute", [](const httplib::Request& request, httplib::Response& response) {
        std::cout << "[SERVER] Recieved /execute POST Request!" << std::endl;
        std::cout << "[SERVER] Request.body:" << std::endl;
        std::cout << request.body << std::endl;
        response.set_header("Access-Control-Allow-Origin", "*");
        json requestBody = json::parse(request.body);
        json responseJSON;
        responseJSON["status"] = "recieved";
        responseJSON["body"] = requestBody;
        std::string output = "";
        Tokenizer tokenizer(requestBody["clientCode"]);
        tokenizer.tokenize();
        std::vector<std::string> tokensVec = tokenizer.getTokens();
        std::cout << "[SERVER] tokensVec Tokens:" << std::endl;
        for (int index = 0; index < tokensVec.size(); index++)
        {
            std::cout << "[SERVER] " << tokensVec[index] << std::endl;
        };
        std::cout << "[SERVER] tokensVec Size: " << tokensVec.size() << std::endl;
        if (tokensVec.size() == 0)
        {
            std::cout << "[SERVER] No Code To Execute!" << std::endl;
            output = "[SYSTEM ERROR] No Code To Execute!";
        }
        else
        {
            Parser parser(tokensVec);
            parser.parse();
            if (parser.isParsedAndASTBuiltSuccessfully())
            {
                std::cout << "[SERVER] Now Interpreting The Code..." << std::endl;
                Interpreter interpreter;
                if (interpreter.interpret(parser.getASTNodeRoot(), requestBody["clientInput"]))
                {
                    std::cout << "[SERVER] Interpreter Successfully Interpreted The Code!" << std::endl;
                }
                else
                {
                    std::cout << "[SERVER] Interpreter Unsuccessfully Interpreted The Code!" << std::endl;
                };
                std::vector<std::string> terminalOutputVec = interpreter.getTerminalOutputVec();
                for (int index = 0; index < terminalOutputVec.size(); index++)
                {
                    std::cout << "[SERVER] terminalOutputVec[" << index << "]:" << terminalOutputVec[index] << std::endl; 
                    output += terminalOutputVec[index] + "\n";
                };
            }
            else
            {
                std::string errorString = parser.getErrorString();
                std::cout << errorString << std::endl;
                output = errorString;
            };
        };
        responseJSON["output"] = output;
        response.set_content(responseJSON.dump(), "application/json");
        std::cout << "[SERVER] /execute POST Completed Successfully!" << std::endl;
    });
    std::cout << "[SERVER] main.cpp Program Running On Port {" << PORT << "}..." << std::endl;
    server.listen("0.0.0.0", PORT);
    return 0;
};