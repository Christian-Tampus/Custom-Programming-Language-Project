/* UPDATE VERSION [13] */

#include "httplib.h"
#include "json.hpp"
#include <iostream>
using namespace std;
using json = nlohmann::json;

/*
Sample Code Input:
output("[SYSTEM MESSAGE] Program Start!");
integer x = 5;
integer y;
input(y);
output(x);
output(y);
output("[SYSTEM MESSAGE] Program Terminated!");
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
        json requestBodyClientCode = json::parse(request.body);
        json responseJSON;
        responseJSON["status"] = "recieved";
        responseJSON["body"] = requestBodyClientCode;
        response.set_content(responseJSON.dump(), "application/json");
    });
    std::cout << "[SERVER] main.cpp Program Running On Port {" << PORT << "}..." << std::endl;
    server.listen("0.0.0.0", PORT);
    return 0;
};