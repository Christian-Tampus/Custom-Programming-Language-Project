/* UPDATE VERSION [31] */

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
Sample Code Input:

INTEGER _integer1;
INTEGER integer2;

INTEGER _integer3 = 123;
INTEGER integer4 = -456;

output("_integer3:");
output(_integer3);
output("integer4:");
output(integer4);

CONSTANT INTEGER _integer5 = 5000;
CONSTANT INTEGER integer6 = 6000;

output("_integer5:");
output(_integer5);
output("integer6:");
output(integer6);

DECIMAL _decimal1;
DECIMAL decimal2;

DECIMAL _decimal3 = 789.123;
DECIMAL decimal4 = -101112.4567890;

output("_decimal3:");
output(_decimal3);
output("decimal4:");
output(decimal4);

CHARACTER _character1;
CHARACTER character2;

CHARACTER _character3 = 'A';
CHARACTER character4 = ' ';

output("_character3:");
output(_character3);
output("character4:");
output(character4);

BOOLEAN _boolean1;
BOOLEAN boolean2;

BOOLEAN _boolean3 = TRUE;
BOOLEAN boolean4 = FALSE;

output("_boolean3:");
output(_boolean3);
output("boolean4:");
output(boolean4);

STRING _string1;
STRING string2;

STRING _string3 = "SAMPLE STRING 1";
STRING string4 = " SAMPLE STRING 2 ";

output("_string3:");
output(_string3);
output("string4:");
output(string4);

!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)

!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)
!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)

!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)
!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)
!This Is A Single-Line Comment (This Language Only Has Single-Line Comments)

output("[SYSTEM MESSAGE] Program Start!");

output("String 1");
output(" String 2");

output("String 3 ");
output(" String 4 ");
output("   String 5   ");

!REMEMBER TO MAKE TERMINAL CSS HAVE SCROLLBARS!
output("   abcdefghijklmnopqrstuvwxyz   ABCDEFGHIJKLMNOPQRSTUVWXYZ   1234567890   !@#$%^&*()-=_+\|]}[{;'/.,:"?><"}]   ");

INTEGER _integer1;
INTEGER integer2;

CONSTANT INTEGER _integer3 = 3; //CONSTANTS MUST BE DECLARED & ASSIGNED!
CONSTANT INTEGER _integer4 = 4; //CONSTANTS MUST BE DECLARED & ASSIGNED!

DECIMAL _decimal1;
DECIMAL decimal2;

CONSTANT DECIMAL _decimal3 = 3.3; //CONSTANTS MUST BE DECLARED & ASSIGNED!
CONSTANT DECIMAL decimal4 = 4.4; //CONSTANTS MUST BE DECLARED & ASSIGNED!

CHARACTER _character1;
CHARACTER character2;

CONSTANT CHARACTER _character3 = 'C'; //CONSTANTS MUST BE DECLARED & ASSIGNED!
CONSTANT CHARACTER character4 = 'D'; //CONSTANTS MUST BE DECLARED & ASSIGNED!

BOOLEAN _boolean1;
BOOLEAN boolean2;

CONSTANT BOOLEAN _boolean3 = TRUE; //CONSTANTS MUST BE DECLARED & ASSIGNED!
CONSTANT BOOLEAN boolean4 = FALSE; //CONSTANTS MUST BE DECLARED & ASSIGNED!

STRING _string1;
STRING string2;

CONSTANT STRING _string3 = "STRING 4"; //CONSTANTS MUST BE DECLARED & ASSIGNED!
CONSTANT STRING string4 = "STRING 4"; //CONSTANTS MUST BE DECLARED & ASSIGNED!

output("[SYSTEM MESSAGE] Program Terminated...");

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
                if (interpreter.interpret(parser.getASTNodeRoot()))
                {
                    std::cout << "[SERVER] Interpreter Successfully Interpreted The Code!" << std::endl;
                    std::vector<std::string> terminalOutputVec = interpreter.getTerminalOutputVec();
                    for (int index = 0; index < terminalOutputVec.size(); index++)
                    {
                        std::cout << "[SERVER] terminalOutputVec[" << index << "]:" << terminalOutputVec[index] << std::endl; 
                        output += terminalOutputVec[index] + "\n";
                    };
                }
                else
                {
                    std::cout << "[SERVER ERROR] HANDLE INTERPRETER ERROR OUTPUT HERE!" << std::endl;
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