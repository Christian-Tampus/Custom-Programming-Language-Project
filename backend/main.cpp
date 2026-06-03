/* UPDATE VERSION [FINAL] */

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
#include <cstdlib>
using namespace std;
using json = nlohmann::json;

/*
==================================================
Main() Function
==================================================
*/
int main()
{
    const char* port_env = getenv("PORT");
    int PORT = port_env ? std::stoi(port_env) : 8080;
    std::cout << "[SERVER] main.cpp Program Start!" << std::endl;
    httplib::Server server;
    server.set_mount_point("/","./frontend");
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
                interpreter.setFunctionsVec(parser.getFunctionsVec());
                interpreter.setVariableMemoryAddressCounter(parser.getVariableMemoryAddressCounter());
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