#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

class CustomPublisherNode : public rclcpp::Node {
public:
  CustomPublisherNode() : Node("height_publisher") {
    publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("command/pose", 10);

    timer_ = this->create_wall_timer(0.01s, std::bind(&CustomPublisherNode::publishPosition, this));

    std::cout << "Insert new coordinates x (front), y (right), z (downword), yaw (clowise)" << std::endl;
    scanf("%lf %lf %lf %lf", &x, &y, &z, &yaw);
    std::cout << "Request new plan for: [" << x << ", " << y << ", " << z << " - " << yaw << "]" << std::endl;

  }

private:
  void publishPosition() {

    geometry_msgs::msg::PoseStamped pose_stamped;
    pose_stamped.header.stamp = this->now();
    pose_stamped.header.frame_id = "base_link"; // Change this to your desired frame ID

    pose_stamped.pose.position.x = x;
    pose_stamped.pose.position.y = y;
    pose_stamped.pose.position.z = z;

    double cos_yaw = cos(yaw);
    double sin_yaw = sin(yaw);
    pose_stamped.pose.orientation.x = 0.0;
    pose_stamped.pose.orientation.y = 0.0;
    pose_stamped.pose.orientation.z = sin_yaw;
    pose_stamped.pose.orientation.w = cos_yaw;


    publisher_->publish(pose_stamped);

  }

  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  double yaw = 0.0;

  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CustomPublisherNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}