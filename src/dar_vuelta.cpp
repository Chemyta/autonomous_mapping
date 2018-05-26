#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "nav_msgs/Odometry.h"

using namespace std;

class vuelta
{
public:
  vuelta()
  {
    //Topic you want to publish
    mov_ = nodo_.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/navi",0);
    end_ = nodo_.advertise<std_msgs::Bool>("pared",0);
    //Topic you want to subscribe
    odom_ = nodo_.subscribe("odom",0, &vuelta::girar, this);
    start_ = nodo_.subscribe("vuelta",0, &vuelta::comienzo, this);
  }


  void girar(const nav_msgs::Odometry::ConstPtr& msg){
  }
  
  void comienzo(const std_msgs::Bool::ConstPtr& msg){
  }
  
private:
  ros::NodeHandle nodo_;
  ros::Publisher mov_;
  ros::Publisher end_;
  ros::Subscriber odom_;
  ros::Subscriber start_;


};//End of class


int main (int argc, char **argv)
{

  ros::init(argc, argv, "dar_vuelta");

  ROS_INFO("Creando nodo para girar");
  vuelta girar;

  ros::spin();


  return 0;
}
