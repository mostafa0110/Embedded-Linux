#include <vsomeip/vsomeip.hpp>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include "common_defs.hpp"
#include "gpio.hpp"

class service {
public:
    service() :
                rtm_(vsomeip::runtime::get()),  // vsomeip runtime
                app_(rtm_->create_application("MyServer")), // vsomeip application
                pin_(Gpio(529))
    {
        if (!app_->init()) {
            std::cerr << "[Server] Couldn't initialize application" << std::endl;
            return;
        }


        pin_.setDirection("out");
        pin_.setValue(0);

        app_->register_message_handler(service_id, service_instance_id,
                service_method_id,
                std::bind(&service::on_message_cbk, this,
                        std::placeholders::_1));

        app_->register_state_handler(
                std::bind(&service::on_state_cbk, this,
                        std::placeholders::_1));

        app_thread_ = std::thread([this]() {
            app_->start(); 
        });
    }

    ~service() {
        std::cout << "[Server] Destructor called. Shutting down..." << std::endl;

        // 1. Stop offering
        app_->stop_offer_service(service_id, service_instance_id);
        
        // 2. Unregister handlers 
        app_->unregister_state_handler();
        app_->unregister_message_handler(service_id, service_instance_id, service_method_id);

        // 3. Stop the event loop
        app_->stop();

        // 4. Join the thread 
        if (app_thread_.joinable()) {
            app_thread_.join();
        }
        std::cout << "[Server] Shutdown complete." << std::endl;
    }

private:

    void on_state_cbk(vsomeip::state_type_e _state)
    {
        if(_state == vsomeip::state_type_e::ST_REGISTERED)
        {
            app_->offer_service(service_id, service_instance_id);
        }
    }

    void set_led(int turn_on) {
        pin_.setValue(turn_on);
    }

    void on_message_cbk(const std::shared_ptr<vsomeip::message> &_request) {
        // Get payload
        std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
        vsomeip::byte_t *data = its_payload->get_data();
        if (its_payload->get_length() < 1) { 
            std::cerr << "[Server] Error: Received empty payload!" << std::endl;
            return; 
        }
        int turn_on = (data[0] == 0x01);
        std::cout << "[Server] Request received: " << (turn_on ? "ON" : "OFF") << std::endl;

        set_led(turn_on);

        // Send Response
        std::shared_ptr<vsomeip::message> response = rtm_->create_response(_request);
        std::string str = "Done";
        std::shared_ptr<vsomeip::payload> resp_pl = rtm_->create_payload();
        std::vector<vsomeip::byte_t> pl_data(str.begin(), str.end());
        resp_pl->set_data(pl_data);
        response->set_payload(resp_pl);
        app_->send(response);
    }
    
    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
    std::thread app_thread_;
    Gpio pin_;
};
