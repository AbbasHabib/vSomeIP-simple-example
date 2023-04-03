#ifndef SERVICE_MANAGER_ADAPTER_HPP_
#define SERVICE_MANAGER_ADAPTER_HPP_

#include <vsomeip/vsomeip.hpp>

class ServiceManagerAdapter
{

public:

    struct serviceIdAndCallBack
    {
        const uint32_t service_id;
        const uint32_t instance_id;
        const uint32_t method_id;
        const std::function<void(const std::shared_ptr<vsomeip::message> &)> callback;
    };

    ServiceManagerAdapter(std::shared_ptr<vsomeip::application> app, const std::vector<serviceIdAndCallBack> &servicesIdsCallbacks);

    void start();
    bool init();

private:

    void run();
    void offerServices();
    void onState(vsomeip::state_type_e state);
    std::shared_ptr<vsomeip::application> m_app_;
    bool m_is_registered_;
    std::mutex m_mutex_;
    std::condition_variable m_condition_;
    std::vector<serviceIdAndCallBack> m_serviceIdCallbacks;
    bool m_blocked_;
    bool m_running_;
    std::thread m_offer_thread_;
};


#endif