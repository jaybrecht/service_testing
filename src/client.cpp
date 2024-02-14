#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/trigger.hpp"


class TestClient : public rclcpp::Node
{
public:
  TestClient()
    : Node("test_client")
  {
    srv_client_ = create_client<example_interfaces::srv::Trigger>("test_service");
  }

  void call_service()
  {
    while (!srv_client_->wait_for_service()) {}

    auto request = std::make_shared<example_interfaces::srv::Trigger::Request>();

    RCLCPP_INFO(get_logger(), "calling service");

    srv_client_->async_send_request(request, std::bind(&TestClient::response_received_callback, this, std::placeholders::_1));
  }

  void response_received_callback(rclcpp::Client<example_interfaces::srv::Trigger>::SharedFuture future)
  {
    auto result = future.get();

    if (result->success) {
      RCLCPP_INFO(get_logger(), result->message.c_str());
    } else {
      RCLCPP_INFO(get_logger(), "service failed");
    }

    response_flag_ = true; 
  }

  bool was_response_recieved()
  {
    return response_flag_;
  }

private:
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr srv_client_;

  bool response_flag_ = false;
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto client = std::make_shared<TestClient>();

  rclcpp::executors::SingleThreadedExecutor executor;

  executor.add_node(client);
  std::thread([&executor]() { executor.spin(); }).detach();

  client->call_service();

  while (!client->was_response_recieved()){}

  rclcpp::shutdown();
  return 0;
}