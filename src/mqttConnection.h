//
// Created by plamen on 11/10/26.
//

#ifndef HOMELABCLIENT_CLIENT_H
#define HOMELABCLIENT_CLIENT_H

#include <chrono>
#include <string>

#include "helpers.h"
#include "mqtt/async_client.h"

const std::string DEVICE_ID{"deviceId"};
const std::string TOPIC{"homelab/" + DEVICE_ID + "/status"};

const std::string LTW_PAYLOAD{"offline"};

const int QOS = 1;

const std::chrono::seconds TIMEOUT = std::chrono::seconds(10);

class mqttConnection{
public:
    mqttConnection(const std::string& serverUri, const std::string& clientId);
    void connect();
    void disconnect();

    bool publish(const std::string& topic, const std::string& payload, bool retain = true);
private:
    callback cb;
    mqtt::async_client mqttClient;
    mqtt::connect_options connOpts;
};


#endif //HOMELABCLIENT_CLIENT_H
