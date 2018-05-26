#include <ros/ros.h>
#include <vector>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/LaserScan.h"

using namespace std;

class pared
{
public:
  pared()
  {
    //Topic you want to publish
    mov_ = nodo_.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/navi",0);
    end_ = nodo_.advertise<std_msgs::Bool>("vuelta",0);
    //Topic you want to subscribe
    scan_ = nodo_.subscribe("scan",0, &pared::seguir_pared, this);
    start_ = nodo_.subscribe("pared",0, &pared::comienzo, this);
  }


  void seguir_pared(const sensor_msgs::LaserScan::ConstPtr& msg){
  }
  
  void comienzo(const std_msgs::Bool::ConstPtr& msg){
  }
  
private:
  ros::NodeHandle nodo_;
  ros::Publisher mov_;
  ros::Publisher end_;
  ros::Subscriber scan_;
  ros::Subscriber start_;


};//End of class


int main (int argc, char **argv)
{

  ros::init(argc, argv, "seguir_pared");

  ROS_INFO("Creando nodo para seguir la pared");
  pared seguimiento;

  ros::spin();


  return 0;
}
