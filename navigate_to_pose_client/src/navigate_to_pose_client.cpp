#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/string.hpp>

using NavigateToPose = nav2_msgs::action::NavigateToPose;
using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

class NavigateToPoseClient : public rclcpp::Node
{
public:
  NavigateToPoseClient() : Node("navigate_to_pose_client")
  {
    this->client_ptr_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");
    this->timer_ = this->create_wall_timer(
      std::chrono::seconds(1), std::bind(&NavigateToPoseClient::send_goal, this));

    this->declare_parameter<int>("HP",100);
    this->get_parameter("HP",HP);     //?
    param_subscriber_ = std::make_shared<rclcpp::ParameterEventHandler>(this);
        auto HP_cb = [this](const rclcpp::Parameter & p) {
            this->HP = p.as_int();
            RCLCPP_INFO(get_logger(),"HP:%d",HP);
            send_goal();  
        };
        cb_handle_ = param_subscriber_->add_parameter_callback("HP",HP_cb);
    }

private:
  int HP;
  std::shared_ptr<rclcpp::ParameterEventHandler> param_subscriber_;
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle_;

  void send_goal()
  {
    
    auto goal_msg = NavigateToPose::Goal();
    goal_msg.pose.header.frame_id = "map";
    if(HP >= 0 && HP<=50){
      goal_msg.pose.pose.position.x = -2.0;  
      goal_msg.pose.pose.position.y = 3.5;  
      goal_msg.pose.pose.position.z = 0.0;  
    }
    else if(HP > 50 && HP<=100){
      goal_msg.pose.pose.position.x = 7.0;  
      goal_msg.pose.pose.position.y = -2.6;  
      goal_msg.pose.pose.position.z = 0.0;  
    }
    goal_msg.pose.pose.orientation.x = 0.0;
    goal_msg.pose.pose.orientation.y = 0.0;
    goal_msg.pose.pose.orientation.z = 0.0;
    goal_msg.pose.pose.orientation.w = 1.0;  
    this->client_ptr_->async_send_goal(goal_msg);
  }

  rclcpp_action::Client<NavigateToPose>::SharedPtr client_ptr_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<NavigateToPoseClient>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}