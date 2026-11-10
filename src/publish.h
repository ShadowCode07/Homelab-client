//
// Created by plamen on 11/10/26.
//

#ifndef HOMELABCLIENT_PUBLISH_H
#define HOMELABCLIENT_PUBLISH_H

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <string>

#include "mqtt/async_client.h"

class publish {

};

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

class action_listener : public virtual mqtt::iaction_listener {
protected:
    void on_failure(const mqtt::token& tok) override {
        std::cout << "\tListener failure for token:" << tok.get_message_id() << std::endl;
    }
    void on_success(const mqtt::token& tok) override {
        std::cout << "\tListener success for token:" << tok.get_message_id() << std::endl;
    }
};

class delivery_action_listener: public virtual action_listener {
    std::atomic<bool> done_;

    void on_failure(const mqtt::token& tok) override {
        action_listener::on_failure(tok);
        done_ = true;
    }

    void on_success(const mqtt::token& tok) override {
        action_listener::on_success(tok);
        done_ = true;
    }

public:
    delivery_action_listener() : done_(false) {}
    bool is_done() const { return done_; }
};

#endif //HOMELABCLIENT_PUBLISH_H
