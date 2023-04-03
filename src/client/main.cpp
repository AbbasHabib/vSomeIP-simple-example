#include "ClientAdapter.hpp"
#include "../ServiceManagerAdapter.hpp"
#include "../../sample-ids.hpp"

void on_message(const std::shared_ptr<vsomeip::message> &_response)
{
    std::cout << "Received a response from Service ["
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_service()
                << "."
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_instance()
                << "] to Client/Session ["
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_client()
                << "/"
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_session()
                << "]"
                << std::endl;
}

void on_message2(const std::shared_ptr<vsomeip::message> &_response)
{
    std::cout << "Received a response from Service ["
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_service()
                << "."
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_instance()
                << "] to Client/Session ["
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_client()
                << "/"
                << std::setw(4) << std::setfill('0') << std::hex << _response->get_session()
                << "]"
                << std::endl;
}



int main(int argc, char **argv)
{
    using service_id_call = ServiceManagerAdapter::serviceIdAndCallBack;

    std::shared_ptr<vsomeip::application> app{vsomeip::runtime::get()->create_application()};
   
    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return -1;
    }

    service_id_call service = {SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, &on_message};
    service_id_call service2 = {SAMPLE_SERVICE_ID + 1, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID+1, &on_message2};
    client_sample its_sample2(app, service2);
    client_sample its_sample(app, service);
    if (!its_sample.init()) 
    {   
        return -1;
    }

    if (!its_sample2.init()) 
    {   
        return -1;
    }
    
    std::future<void> vsome = std::async(std::launch::async, [&](){
        app->start();
    });
    
    while(1)
    {
        its_sample.sendToService({'H', 'E', 'L', 'L', 'O', '\n', 0});
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        its_sample2.sendToService({'2', 'J', 'L', 'L', 'O', '\n', 0});
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    vsome.get();
}