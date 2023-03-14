#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>
#include <atomic>
#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_METHOD_ID 0x0421

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;
std::atomic<bool> g_service_available{false};

void run() {
  while(true)
  {
    if(!g_service_available.load())
    {
      std::unique_lock<std::mutex> its_lock(mutex);
      condition.wait(its_lock);
    }
    std::cout << "ANAA BA3ATTT \n\n\n\n\n\n\n";

    std::shared_ptr< vsomeip::message > request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SAMPLE_SERVICE_ID);
    request->set_instance(SAMPLE_INSTANCE_ID);
    request->set_method(SAMPLE_METHOD_ID);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
   
    for (uint8_t c : std::string("client is calling lol\n")) {
        its_payload_data.push_back(c);
    }
   
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

void on_message(const std::shared_ptr<vsomeip::message> &_response) {

  std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();

  std::string receivedMsg((char*)its_payload->get_data(), its_payload->get_length());

  std::cout << receivedMsg;
  std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
      <<"\n\n\n\n\n\n";
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;

    g_service_available = _is_available;

    if(_is_available)
    {
        std::cout << "notify one\n\n\n\n";
        condition.notify_one();
        g_service_available = true;
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    std::thread sender(run);
    app->start();
}