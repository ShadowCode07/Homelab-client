#include <atomic>
#include <chrono>
#include <thread>
#include <csignal>
#include <iostream>
#include <string>

#include "telemetry.h"
#include "mqttConnection.h"
#include "agentConfig.h"

std::atomic<bool> running {true};
void onSignal(int) {
    running = false;
}

int main (int argc, char* argv[]) {
    std::signal(SIGINT, onSignal);
    std::signal(SIGTERM, onSignal);

    std::string configPath = (argc > 1) ? argv[1] : "agent.json";

    /*std::cout << "Idle Time " << readCpuTimes().value().idle << std::endl;
    std::cout << "Total Time " << readCpuTimes().value().total / 100 / 16 << std::endl;*/
    /*auto mounts = listDiskMounts();
    for (const auto& paths : *mounts) {
        std::cout << paths << '\n';
    }*/

    std::cout << "Initializing connection to server" << std::endl;

    try {
        AgentConfig cfg = loadConfig(configPath);
        std::string serverURI = "mqtt://" + cfg.brokerHost + ":" + std::to_string(cfg.brokerPort);

        std::vector<std::string> drivesPaths = cfg.drivesPaths;

        if (drivesPaths.empty()) {
            auto automaticPathSearch = listDiskMounts();
            if (automaticPathSearch && !automaticPathSearch->empty()) {
                drivesPaths = automaticPathSearch.value();
            }
            else
                std::cerr << "No drivesPaths found" << std::endl;
        }

        for (auto drivePath : drivesPaths) {
            std::cout << drivePath << std::endl;
        }

        mqttConnection MqttConnection(serverURI, cfg.deviceId);
        MqttConnection.connect();

        std::cout << "Running. Press Ctrl+C to stop." << std::endl;
        while (running) {
            // TODO: telemetry (separate topic, non-retained) goes here
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }


        if (!MqttConnection.publish(MqttConnection.topic(), LTW_PAYLOAD, true)) {
            std::cerr << "offline not confirmed" << std::endl;
        }

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        MqttConnection.disconnect();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const std::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}