/* UPDATE VERSION [33] */

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
CONSTANT INTEGER CONSTANT_INTEGER_1 = 999;
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