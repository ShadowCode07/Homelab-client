#include <chrono>
#include <iostream>
#include <string>
#include "publish.h"

const std::string DFLT_SERVER_URI{"mqtt://localhost:1883"};

const std::string DEVICE_ID{"deviceId"};
const std::string TOPIC{"homelab/" + DEVICE_ID + "/status"};

const std::string PAYLOAD{"online"};
const std::string LTW_PAYLOAD{"offline"};

const int QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);

int main (int argc, char* argv[]) {
    std::string serverURI = (argc > 1) ? std::string{argv[1]} : DFLT_SERVER_URI;

    std::cout << "Initializing connection to " << serverURI << std::endl;
    mqtt::async_client client{serverURI, DEVICE_ID};

    callback cb;
    client.set_callback(cb);

    auto connOpts = mqtt::connect_options_builder()
                    .connect_timeout(TIMEOUT)
                    .clean_session()
                    .keep_alive_interval(std::chrono::seconds(10))
                    .will(mqtt::message(TOPIC, LTW_PAYLOAD, QOS, true))
                    .finalize();

    std::cout << "...Ok" << std::endl;

    try {
        std::cout << "\nConnecting..." << std::endl;
        mqtt::token_ptr conntok = client.connect(connOpts);
        std::cout << "Waiting for connection..." << std::endl;
        conntok->wait();
        std::cout << "Connected" << std::endl;

        std::cout << "\nSending next message..." << std::endl;
        mqtt::delivery_token_ptr pubtok;
        pubtok = client.publish(TOPIC, PAYLOAD, QOS, false);
        std::cout << "  ...with token: " << pubtok->get_message_id() << std::endl;
        std::cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
             << " bytes" << std::endl;

        if (pubtok->wait_for(TIMEOUT)) {
            std::cout << "...Ok" << std::endl;
        }else {
            std::cout << "...timed out, not confirmed" << std::endl;
        }

        std::cin.get();

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        client.disconnect()->wait();
        std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
