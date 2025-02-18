#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

using json = nlohmann::json;
class WebSocketClient {
private:
    std::string deviceIdentifier;
    std::string passWord;
    int port;
    ix::WebSocket webSocket;
    bool connected;

    void setupEventListeners() {
        webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Message) {
                try {
                    auto parsedMsg = json::parse(msg->str);
                    if (parsedMsg.empty()) {
                        return;
                    }

                    json data = {
                        {"msg", json::parse(parsedMsg["msg"].get<std::string>())},
                        {"resourceId", parsedMsg["resourceId"]}
                    };

                    if (data["msg"]["action"] == "getPage") {
                        json response = {
                            {"msg", {
                                {"resourceId", data["resourceId"]},
                                {"action", "respondingToGetPage"},
                                {"page", "dddd"}
                            }}
                        };
                        sendMessage(response);
                    }
                }
                catch (const json::parse_error& e) {
                    std::cout << "JSON parse error: " << e.what() << std::endl;
                }
            }
            else if (msg->type == ix::WebSocketMessageType::Open) {
                connected = true;
                std::cout << "Connection established" << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Close) {
                connected = false;
                std::cout << "Connection closed" << std::endl;
                // Reconnect after 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
                connect();
            }
            else if (msg->type == ix::WebSocketMessageType::Error) {
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
            }
        });
    }

public:
    WebSocketClient() : 
        deviceIdentifier("aaaa"),
        passWord("ssss"),
        port(8443),
        connected(false) {
        ix::initNetSystem();
    }

    void connect() {
        std::string wsUrl = "wss://myRemoteDevice.ya-niv.com:8443/?type=device&deviceIdentifier=" + 
                           deviceIdentifier + "&passWord=" + passWord;
        
        try {
            webSocket.setUrl(wsUrl);
            setupEventListeners();
            webSocket.start();
        }
        catch (const std::exception& e) {
            updateStatus("Connection error: " + std::string(e.what()));
        }
    }

    void sendMessage(const json& data) {
        if (connected) {
            try {
                webSocket.send(data.dump());
            }
            catch (const std::exception& e) {
                std::cout << "Send error: " << e.what() << std::endl;
            }
        }
    }

    void updateStatus(const std::string& status) {
        std::cout << status << std::endl;
    }
};

int main() {
    printf("asdf\n");
    WebSocketClient client;
    client.connect();

    // Keep the program running
    std::string input;
    std::getline(std::cin, input);

    return 0;
}