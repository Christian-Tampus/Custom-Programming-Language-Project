/* UPDATE VERSION [14] */

#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std;
using json = nlohmann::json;

/*
Sample Code Input:
output("[SYSTEM MESSAGE] Program Start!");
integer x;
input(x);
output(x);
output("[SYSTEM MESSAGE] Program Terminated!");
*/
bool allowTerminalInput = false;
std::vector<int> inputBufferVec;
std::vector<std::string> codeTokensVec;

/*
|----|-----|-----|-----|-----|-----|-----|-----|-----|
|----|-----|-----|-----|-----|-----|-----|-----|-----|
|----|-----|-----|-----|-----|-----|-----|-----|-----|
IMPORTANT, WE WILL USE INPUT BUFFERS TO SIMPIFY THE PROJECT
UPDATE THE CLIENT (HTML, CSS, JS) ACCORDINGLY
|----|-----|-----|-----|-----|-----|-----|-----|-----|
|----|-----|-----|-----|-----|-----|-----|-----|-----|
|----|-----|-----|-----|-----|-----|-----|-----|-----|
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



        std::string clientCode = requestBody["clientCode"];
        std::stringstream ss(clientCode);
        std::string codeLine;
        while (std::getline(ss, codeLine, '\n'))
        {
            codeTokensVec.push_back(codeLine);
        };
        if (codeTokensVec[0] == "output(\"[SYSTEM MESSAGE] Program Start!\");")
        {
            std::cout << "[SYSTEM MESSAGE] Program Start!" << std::endl;
        };
        int x;
        if (codeTokensVec[2] == "input(x);")
        {
            allowTerminalInput = true;
        };
        while (allowTerminalInput)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };
        x = inputBufferVec[0];
        inputBufferVec.clear();
        if (codeTokensVec[3] == "output(x);")
        {
            std::cout << x << std::endl;
        };
        if (codeTokensVec[4] == "output(\"[SYSTEM MESSAGE] Program Terminated!\");")
        {
            std::cout << "[SYSTEM MESSAGE] Program Terminated!" << std::endl;
        };

        response.set_content(responseJSON.dump(), "application/json");
        std::cout << "[SERVER] /execute POST Completed Successfully!" << std::endl;
    });
    server.Options("/terminalInput", [](const httplib::Request&, httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        response.set_header("Access-Control-Allow-Headers", "Content-Type");
    });
    server.Post("/terminalInput", [](const httplib::Request& request, httplib::Response& response) {
        std::cout << "[SERVER] Recieved /terminalInput POST Request!" << std::endl;
        std::cout << "[SERVER] Request.body:" << std::endl;
        std::cout << request.body << std::endl;
        response.set_header("Access-Control-Allow-Origin", "*");
        json requestBody = json::parse(request.body);
        json responseJSON;
        responseJSON["status"] = "recieved";
        responseJSON["body"] = requestBody;



        if (allowTerminalInput)
        {
            std::string terminalInput = requestBody["terminalInput"];
            int terminalInputToInt = std::stoi(terminalInput);
            inputBufferVec.push_back(terminalInputToInt);
            std::cout << "terminalInputToInt: " << terminalInputToInt << std::endl;
            std::cout << "inputBufferVec Size: " << inputBufferVec.size() << std::endl;
            allowTerminalInput = false;
        }
        else
        {
            std::cout << "[SERVER] Terminal Input Not Allowed!" << std::endl;
        };



        response.set_content(responseJSON.dump(), "application/json");
        std::cout << "[SERVER] /terminalInput POST Completed Successfully!" << std::endl;
    });
    std::cout << "[SERVER] main.cpp Program Running On Port {" << PORT << "}..." << std::endl;
    server.listen("0.0.0.0", PORT);
    return 0;
};