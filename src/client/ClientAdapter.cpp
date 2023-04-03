#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include <vsomeip/vsomeip.hpp>


ClientAdapter::ClientAdapter(std::shared_ptr<vsomeip::application> app, const ServiceManagerAdapter::serviceIdAndCallBack &serviceIdCallback, bool useTcp = false)
    : app_{app},
      request_{vsomeip::runtime::get()->create_request(useTcp)},
      use_tcp_{useTcp},
      blocked_{false},
      m_isServiceAvailable{false},
      serviceIdCallback{serviceIdCallback}
{
}

bool ClientAdapter::init()
{

    std::cout << "Client settings [protocol="
              << (use_tcp_ ? "TCP" : "UDP")
              << ":quiet="
              << ":cycle="
              << "]"
              << std::endl;
    
    // TODO
    // app_->register_state_handler(
    //     std::bind(
    //         &client_sample::on_state,
    //         this,
    //         std::placeholders::_1));

    // while(!m_appAvailable);

    requestService();

    app_->register_message_handler(
        serviceIdCallback.service_id, serviceIdCallback.instance_id, serviceIdCallback.method_id,
        serviceIdCallback.callback);

    request_->set_service(serviceIdCallback.service_id);
    request_->set_instance(serviceIdCallback.instance_id);
    request_->set_method(serviceIdCallback.method_id);

    app_->register_availability_handler(serviceIdCallback.service_id, serviceIdCallback.instance_id,
                                        std::bind(&ClientAdapter::on_availability,
                                                  this,
                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    return true;
}

void ClientAdapter::requestService()
{
    app_->request_service(serviceIdCallback.service_id, serviceIdCallback.instance_id);
}

void ClientAdapter::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available)
{
    std::cout << "Service ["
              << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
              << "] is "
              << (_is_available ? "available." : "NOT available.")
              << std::endl;

    if (serviceIdCallback.service_id == _service && serviceIdCallback.instance_id == _instance)
    {
        if (m_isServiceAvailable && !_is_available)
        {
            m_isServiceAvailable = false;
        }
        else if (_is_available && !m_isServiceAvailable)
        {
            m_isServiceAvailable = true;
        }
    }
}

void ClientAdapter::sendToService(const std::vector<vsomeip::byte_t> &msg)
{
    std::lock_guard<std::mutex> its_lock(mutex_);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    its_payload->set_data(msg);
    request_->set_payload(its_payload);
    if (m_isServiceAvailable)
    {
        app_->send(request_);
    }
}