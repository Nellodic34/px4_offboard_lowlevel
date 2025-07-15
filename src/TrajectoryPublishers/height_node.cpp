#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

class HeightPublisherNode : public rclcpp::Node {
public:
  HeightPublisherNode() : Node("height_publisher") {
    publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("command/pose", 10);

    timer_ = this->create_wall_timer(0.01s, std::bind(&HeightPublisherNode::publishHeight, this));
  }

private:
  void publishHeight() {

    geometry_msgs::msg::PoseStamped pose_stamped;
    pose_stamped.header.stamp = this->now();
    pose_stamped.header.frame_id = "base_link"; // Change this to your desired frame ID

    pose_stamped.pose.position.x = 0.0;
    pose_stamped.pose.position.y = 0.0;
    pose_stamped.pose.position.z = 2.0;
    pose_stamped.pose.orientation.w = 1.0;

    publisher_->publish(pose_stamped);

  }

  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<HeightPublisherNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}