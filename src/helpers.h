//
// Created by plamen on 11/10/26.
//

#ifndef HOMELABCLIENT_HELPERS_H
#define HOMELABCLIENT_HELPERS_H

#include <string>
#include <iostream>

#include "mqtt/async_client.h"

class callback : public virtual mqtt::callback {
public:
    void connection_lost(const std::string& cause) override {
        std::cout << "Connection lost" << std::endl;
        if (!cause.empty()) {
            std::cout << "\tcause: " << cause << std::endl;
        }
    }
    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        std::cout << "\tDeliver successful for token:" << (tok ? tok->get_message_id() : -1) << std::endl;
    }
};

#endif //HOMELABCLIENT_HELPERS_H
