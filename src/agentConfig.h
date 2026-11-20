//
// Created by plamen on 11/20/26.
//

#ifndef HOMELABCLIENT_AGENTCONFIG_H
#define HOMELABCLIENT_AGENTCONFIG_H

#include<string>

struct AgentConfig {
    std::string brokerHost = "localhost";
    int brokerPort = 1883;
    std::string deviceId;
};

AgentConfig loadConfig(const std::string& configFile);

#endif //HOMELABCLIENT_AGENTCONFIG_H
