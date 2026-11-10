#include <chrono>
#include <iostream>
#include <string>

#include "mqttConnection.h"

const std::string DFLT_SERVER_URI{"mqtt://localhost:1883"};
const std::string PAYLOAD{"online"};


int main (int argc, char* argv[]) {
    std::string serverURI = (argc > 1) ? std::string{argv[1]} : DFLT_SERVER_URI;

    std::cout << "Initializing connection to " << serverURI << std::endl;

    try {
        mqttConnection MqttConnection(serverURI, DEVICE_ID);
        MqttConnection.connect();

        std::cout << "\nSending next message..." << std::endl;

        if (!MqttConnection.publish(TOPIC, PAYLOAD, true)) {
            std::cerr << "Not confirmed@" << std::endl;
        }

        std::cin.get();


        if (!MqttConnection.publish(TOPIC, LTW_PAYLOAD, true)) {
            std::cerr << "offline not confirmed" << std::endl;
        }

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        MqttConnection.disconnect();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}