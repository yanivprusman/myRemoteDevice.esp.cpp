#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
    // Initialize networking
    ix::initNetSystem();
    
    // Create WebSocket instance
    ix::WebSocket webSocket;
    
    // Set the URL with your parameters
    std::string url("wss://myRemoteDevice.ya-niv.com:8443/wss/?type=client&deviceIdentifier=sdfg&passWord=ggg");
    webSocket.setUrl(url);
    
    std::cout << "Connecting to " << url << "..." << std::endl;

    // Set up the callback
    webSocket.setOnMessageCallback([&webSocket](const ix::WebSocketMessagePtr& msg)
    {
        if (msg->type == ix::WebSocketMessageType::Message)
        {
            try {
                // Parse the nested JSON
                auto outerJson = json::parse(msg->str);
                auto innerJson = json::parse(outerJson["msg"].get<std::string>());
                
                std::cout << "Received message: " << innerJson.dump(2) << std::endl;
                
                // Check for respondingToGetPage action
                if (innerJson["msg"]["action"] == "respondingToGetPage") {
                    std::cout << "Got respondingToGetPage" << std::endl;
                    std::cout << "Page content: " << innerJson["msg"]["page"] << std::endl;
                }
            }
            catch (const json::parse_error& e) {
                std::cout << "JSON parse error: " << e.what() << std::endl;
            }
        }
        else if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "Connection established" << std::endl;
            
            // Send getPage action when connection opens
            json message = {
                {"action", "getPage"}
            };
            
            webSocket.send(message.dump());
        }
        else if (msg->type == ix::WebSocketMessageType::Error)
        {
            std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Close)
        {
            std::cout << "Connection closed" << std::endl;
        }
    });

    // Start the connection
    webSocket.start();

    // Keep the program running
    std::string input;
    std::getline(std::cin, input);

    return 0;
}