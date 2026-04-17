/* UPDATE VERSION [19] */

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

output("   abcdefghijklmnopqrstuvwxyz   ABCDEFGHIJKLMNOPQRSTUVWXYZ   1234567890   !@#$%^&*()-=_+\|]}[{;'/.,:"?><"}]   ");

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



        /*
        ==================================================
        CONTINUE HERE!
        ==================================================
        */
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
            std::string output = "[SYSTEM MESSAGE] No Code To Execute!";
            responseJSON["output"] = output;
        }
        else
        {
            Parser parser(tokensVec);
            parser.parse();
        };
        /*
        ==================================================
        CONTINUE HERE!
        ==================================================
        */



        response.set_content(responseJSON.dump(), "application/json");
        std::cout << "[SERVER] /execute POST Completed Successfully!" << std::endl;
    });
    std::cout << "[SERVER] main.cpp Program Running On Port {" << PORT << "}..." << std::endl;
    server.listen("0.0.0.0", PORT);
    return 0;
};