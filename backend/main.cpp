/* UPDATE VERSION [12] */
#include "httplib.h"
#include <iostream>
using namespace std;
int main()
{
    const int PORT = 8080;
    std::cout << "[SERVER] main.cpp Program Start!" << std::endl;
    httplib::Server server;
    server.Get("/api/hello", [](const httplib::Request&, httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_content(R"({
            "message": "[SERVER] C++ Backend Connection Success!",
            "status": "ok"
        })", "application/json");
        std::cout << "[SERVER] Recieved Request From Client!" << std::endl;
    });
    std::cout << "[SERVER] main.cpp Program Running On Port {" << PORT << "}..." << std::endl;
    server.listen("0.0.0.0", PORT);
    return 0;
};