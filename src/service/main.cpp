#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "ServiceManagerAdapter.hpp"
#include "../sample-ids.hpp"

void on_message(const std::shared_ptr<vsomeip::message> &_request, const std::shared_ptr<vsomeip::application> &app)
{
    std::cout << "on msg\n";
    std::cout << "Received a message with Client/Session [" << std::setw(4)
              << std::setfill('0') << std::hex << _request->get_client() << "/"
              << std::setw(4) << std::setfill('0') << std::hex
              << _request->get_session() << "]"
              << std::endl;
    std::string receivedMsg((char*)_request->get_payload()->get_data(), _request->get_payload()->get_length());
    std::cout << receivedMsg << '\n';


    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (std::size_t i = 0; i < 120; ++i)
        its_payload_data.push_back(static_cast<vsomeip::byte_t>(i % 256));
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);

    app->send(its_response);
}


void on_message2(const std::shared_ptr<vsomeip::message> &_request, const std::shared_ptr<vsomeip::application> &app)
{
    std::cout << "on msg2222222222222\n";
    std::string receivedMsg((char*)_request->get_payload()->get_data(), _request->get_payload()->get_length());
    std::cout << receivedMsg << '\n';


    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (std::size_t i = 0; i < 120; ++i)
        its_payload_data.push_back(static_cast<vsomeip::byte_t>(i % 256));
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);

    app->send(its_response);
}

int main()
{

    using service_id_call = ServiceManagerAdapter::serviceIdAndCallBack;

    std::shared_ptr<vsomeip::application> app{vsomeip::runtime::get()->create_application()};

    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return -1;
    }

    std::vector<service_id_call> offeredServices{
        {SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, std::bind(&on_message, std::placeholders::_1, app)},
        {SAMPLE_SERVICE_ID+1, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID+1, std::bind(&on_message2, std::placeholders::_1, app)}};

    ServiceManagerAdapter vsomeService{app, offeredServices};

    if (!vsomeService.init())
    {
        std::cerr << "Couldn't initialize vsomeip services" << std::endl;
        return -1;
    }
    vsomeService.start();
}