#include "agentConfig.h"

#include <fstream>
#include <regex>
#include <stdexcept>
#include <nlohmann/json.hpp>

static const std::regex guidPattern{
    R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$)"};

AgentConfig loadConfig(const std::string& configFile) {
    std::ifstream ifs(configFile);

    if (!ifs.is_open())
        throw std::runtime_error("Cannot open config file: " + configFile);


    nlohmann::json j = nlohmann::json::parse(ifs);

    AgentConfig cfg;
    cfg.brokerHost = j.value("brokerHost", cfg.brokerHost);
    cfg.brokerPort = j.value("brokerPort", cfg.brokerPort);

    if (!j.contains("deviceId"))
        throw std::runtime_error(configFile + ": 'deviceId' is required");

    cfg.deviceId = j.at("deviceId").get<std::string>();

    if (!std::regex_match(cfg.deviceId, guidPattern))
        throw std::runtime_error(configFile + ": 'deviceId' must be a GUID (copy it from the device page in the UI)");

    return cfg;
}
