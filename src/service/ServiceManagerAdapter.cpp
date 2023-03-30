
#include "ServiceManagerAdapter.hpp"


ServiceManagerAdapter::ServiceManagerAdapter(std::shared_ptr<vsomeip::application> app, const std::vector<serviceIdAndCallBack> &servicesIdsCallbacks)
    : m_app_{app},
        m_is_registered_{false},
        m_blocked_{false},
        m_running_{true},
        m_serviceIdCallbacks{servicesIdsCallbacks},
        m_offer_thread_{std::bind(&ServiceManagerAdapter::run, this)}
{
}

void ServiceManagerAdapter::start()
{
    m_app_->start();
}

bool ServiceManagerAdapter::init()
{
    std::lock_guard<std::mutex> lock(m_mutex_);

    m_app_->register_state_handler(
        std::bind(&ServiceManagerAdapter::onState, this,
                    std::placeholders::_1));

    for (auto service : m_serviceIdCallbacks)
    {
        m_app_->register_message_handler(
            service.service_id,
            service.instance_id,
            service.method_id,
            service.callback);
    }

    return true;
}

void ServiceManagerAdapter::run()
{
    // wait until service is registered state
    std::unique_lock<std::mutex> its_lock(m_mutex_);
    while (!m_blocked_)
        m_condition_.wait(its_lock);

    bool is_offer(true);

    offerServices();
}

void ServiceManagerAdapter::offerServices()
{
    for (auto service : m_serviceIdCallbacks)
    {
        m_app_->offer_service(service.service_id, service.instance_id);
    }
}

void ServiceManagerAdapter::onState(vsomeip::state_type_e state)
{
    std::cout << "Application " << m_app_->get_name() << " is "
                << (state == vsomeip::state_type_e::ST_REGISTERED ? "registered." : "deregistered.")
                << std::endl;

    if (state == vsomeip::state_type_e::ST_REGISTERED)
    {
        if (!m_is_registered_)
        {
            m_is_registered_ = true;
            m_blocked_ = true;
            m_condition_.notify_one();
        }
    }
    else
    {
        m_is_registered_ = false;
    }
}

