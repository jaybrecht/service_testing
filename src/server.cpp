#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/trigger.hpp"


class TestServer : public rclcpp::Node
{
public:
  TestServer()
    : Node("test_client")
  {
    srv_server_ = create_service<example_interfaces::srv::Trigger>("test_service", 
        std::bind(&TestServer::service_callback, this, std::placeholders::_1,  std::placeholders::_2));
  }

  void service_callback(
    example_interfaces::srv::Trigger::Request::SharedPtr req,
    example_interfaces::srv::Trigger::Response::SharedPtr res)
  {
    (void)req;

    RCLCPP_INFO(get_logger(), "service reqested");

    sleep(3);

    res->success = true;
    res->message = "service completed";
  }

private:
  rclcpp::Service<example_interfaces::srv::Trigger>::SharedPtr srv_server_;
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto server = std::make_shared<TestServer>();

  RCLCPP_INFO(server->get_logger(), "server ready");

  rclcpp::spin(server);

  rclcpp::shutdown();
  return 0;
}