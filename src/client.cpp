#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/trigger.hpp"


class TestServer : public rclcpp::Node
{
public:
  TestServer()
    : Node("test_client")
  {
    srv_client_ = create_client<example_interfaces::srv::Trigger>("test_service");

    call_service();
  }

  void call_service()
  {
    while (!srv_client_->wait_for_service()) {}

    auto request = std::make_shared<example_interfaces::srv::Trigger::Request>();

    RCLCPP_INFO(get_logger(), "calling service");

    auto future = srv_client_->async_send_request(request, std::bind(&TestServer::service_cb, this, std::placeholders::_1));
  }

  void service_cb(rclcpp::Client<example_interfaces::srv::Trigger>::SharedFuture future)
  {
    auto result = future.get();

    if (result->success) {
      RCLCPP_INFO(get_logger(), result->message.c_str());
    } else {
      RCLCPP_INFO(get_logger(), "service failed");
    }
    
  }

private:
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr srv_client_;
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto client = std::make_shared<TestServer>();

  rclcpp::spin(client);

  rclcpp::shutdown();
  return 0;
}