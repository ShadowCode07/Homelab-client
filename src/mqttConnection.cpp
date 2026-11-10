#include "mqttConnection.h"

mqttConnection::mqttConnection(const std::string &serverUri, const std::string &clientId)
    : mqttClient{serverUri, clientId} {
    mqttClient.set_callback(cb);
    connOpts = mqtt::connect_options_builder()
            .connect_timeout(TIMEOUT)
            .clean_session()
            .keep_alive_interval(std::chrono::seconds(10))
            .will(mqtt::message(TOPIC, LTW_PAYLOAD, QOS, true))
            .finalize();
}

void mqttConnection::connect() {
    mqtt::token_ptr conntok = mqttClient.connect(connOpts);
    conntok->wait();

    std::cout << "Connected" << std::endl;
}

void mqttConnection::disconnect() {
    mqttClient.disconnect()->wait();
}

bool mqttConnection::publish(const std::string& topic, const std::string &payload, bool retain) {
    mqtt::delivery_token_ptr pubtok = mqttClient.publish(topic, payload, QOS, retain);

    std::cout << "  ...with token: " << pubtok->get_message_id() << std::endl;

    std::cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
            << " bytes" << std::endl;

    return pubtok->wait_for(TIMEOUT);
}
