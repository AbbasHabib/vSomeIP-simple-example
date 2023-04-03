#ifndef CLIENT_ADAPTER_HPP_
#define CLIENT_ADAPTER_HPP_

#include <vsomeip/vsomeip.hpp>

class ClientAdapter
{
public:
    ClientAdapter(std::shared_ptr<vsomeip::application> app, const ServiceManagerAdapter::serviceIdAndCallBack &serviceIdCallback, bool useTcp = false);
    bool init();
    void requestService();
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void sendToService(const std::vector<vsomeip::byte_t>& msg);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::shared_ptr<vsomeip::message> request_;
    bool use_tcp_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool blocked_;
    bool m_isServiceAvailable;
    std::thread sender_;
    const ServiceManagerAdapter::serviceIdAndCallBack serviceIdCallback;
};

#endif