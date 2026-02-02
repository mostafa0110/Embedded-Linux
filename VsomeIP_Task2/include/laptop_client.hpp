#include <vsomeip/vsomeip.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <atomic>
#include "common_defs.hpp"

class client {
public:
    client() :
        rtm_(vsomeip::runtime::get()),
        app_(rtm_->create_application("MyClient")),
        is_available_(false)
    {
        if (!app_->init()) {
            std::cerr << "[Client] Couldn't initialize application" << std::endl;
            return;
        }

        // 1. Register Availability Handler (Service Discovery)
        app_->register_availability_handler(service_id, service_instance_id,
            std::bind(&client::on_availability_cbk, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 2. Request
        app_->request_service(service_id, service_instance_id);

        // 3. Register Message Handler 
        app_->register_message_handler(service_id, service_instance_id, service_method_id,
            std::bind(&client::on_message_cbk, this, std::placeholders::_1));

        app_thread_ = std::thread([this]() {
            app_->start();
        });
    }

    ~client() {
        std::cout << "[Client] Destructor called. Shutting down..." << std::endl;

        app_->release_service(service_id, service_instance_id);
        app_->unregister_availability_handler(service_id, service_instance_id);
        app_->unregister_message_handler(service_id, service_instance_id, service_method_id);
        app_->stop();

        if (app_thread_.joinable()) {
            app_thread_.join();
        }
        std::cout << "[Client] Shutdown complete." << std::endl;
    }

    void set_remote_led(bool turn_on) {
        if (!is_available_) {
            std::cerr << "[Client] Cannot send: Service NOT available." << std::endl;
            return;
        }

        // 1. Create Request
        std::shared_ptr<vsomeip::message> request = rtm_->create_request();
        request->set_service(service_id);
        request->set_instance(service_instance_id);
        request->set_method(service_method_id);

        // 2. Create Payload (1 Byte: 0x01 or 0x00)
        std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
        std::vector<vsomeip::byte_t> pl_data;
        pl_data.push_back(turn_on ? 0x01 : 0x00);
        
        pl->set_data(pl_data);
        request->set_payload(pl);

        // 3. Send
        app_->send(request);
        std::cout << "[Client] Sent Request: " << (turn_on ? "ON" : "OFF") << std::endl;
    }

private:
    void on_availability_cbk(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
        if (service_id == _service && service_instance_id == _instance) {
            is_available_ = _is_available;
            if (is_available_) {
                std::cout << "[Client] Service FOUND!" << std::endl;
            } else {
                std::cout << "[Client] Service LOST." << std::endl;
            }
        }
    }

    void on_message_cbk(const std::shared_ptr<vsomeip::message> &_response) {
        std::shared_ptr<vsomeip::payload> pl = _response->get_payload();
        std::string resp_str((char*)pl->get_data(), pl->get_length());
        std::cout << "[Client] Server Responded: " << resp_str << std::endl;
    }

    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
    std::thread app_thread_;
    std::atomic<bool> is_available_; 
};