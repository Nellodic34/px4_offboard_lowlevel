#include <chrono>
#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

class LemniscatePublisherNode : public rclcpp::Node {
public:
  LemniscatePublisherNode() : Node("lemniscate_publisher") {
    publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("command/pose", 10);

    timer_ = this->create_wall_timer(0.01s, std::bind(&LemniscatePublisherNode::publishLemniscatePose, this));
  }

private:
  void publishLemniscatePose() {
    static double t = 0.0;
    static double h = 0.0;
    double a = 4.0; // Scale factor for the lemniscate size

    geometry_msgs::msg::PoseStamped pose_stamped;
    pose_stamped.header.stamp = this->now();
    pose_stamped.header.frame_id = "base_link"; // Change this to your desired frame ID


    
    if(h<=2)  
    {
      pose_stamped.pose.position.x = 0.0;
      pose_stamped.pose.position.y = 0.0;
      pose_stamped.pose.position.z = h;
      pose_stamped.pose.orientation.w = 1.0;

      h+= 0.01;
    }
    else{

    
    if(t < 4 * M_PI)
    {
      // Lemniscate (figure-8) parametric equations
      double sin_t = sin(t);
      double cos_t = cos(t);
      double denominator = 1.0 + sin_t * sin_t;

      pose_stamped.pose.position.x = a * cos_t / denominator - a;
      pose_stamped.pose.position.y = a * sin_t * cos_t / denominator;

      t += 0.001; // Change this value to control the speed along the lemniscate path
    
    }  

      pose_stamped.pose.position.z = 2.0;
      pose_stamped.pose.orientation.w = 1.0;
    }

    publisher_->publish(pose_stamped);

    
    
  }

  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LemniscatePublisherNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}