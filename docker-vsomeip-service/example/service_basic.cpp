#include <iomanip>
#include <iostream>
#include <sstream>

#include <vsomeip/vsomeip.hpp>


#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_METHOD_ID 0x0421

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &_request)
{

    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();

    std::string receivedMsg((char*)its_payload->get_data(), its_payload->get_length());

    std::cout << receivedMsg;

    std::cout << "SERVICE: Received message with Client/Session ["
              << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
              << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
              << "\n\n\n\n\n\n\n\n";
    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    std::string msg = "HELLOOO\n";
    for (uint8_t c : msg)
        its_payload_data.push_back(c);

    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
    app->send(its_response);
}

int main()
{
    app = vsomeip::runtime::get()->create_application("World");
    app->init();
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->start();
}