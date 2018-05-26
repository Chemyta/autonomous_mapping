#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "nav_msgs/Odometry.h"
#include "tf/tf.h"

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
    odom_ = nodo_.subscribe("odom",0, &vuelta::calcular_angulo, this);
    start_ = nodo_.subscribe("vuelta",0, &vuelta::comienzo, this);
    
    angulo = 0;
    objetivo = -1; // Si el objetivo es negativo, significa que no necesitamos dar la vuelta
  }


  void calcular_angulo(const nav_msgs::Odometry::ConstPtr& msg){
	  // Se calcular el angulo actual del robot respecto a su posición inicial 
	  tf::Quaternion q(msg->pose.pose.orientation.x,
                       msg->pose.pose.orientation.y,
                       msg->pose.pose.orientation.z,
                       msg->pose.pose.orientation.w);
      tf::Matrix3x3 m(q);

      double roll, pitch, yaw;
      m.getRPY(roll, pitch, yaw);
      angulo = (360*yaw)/(2*3.14159);
      if(angulo<0) angulo=angulo+360;
      
      if (objetivo != -1) girar();
  }
  
  void girar()  {
	  /* Creamos una variable auxiliar para comprobar si hemos cumplimentado el objetivo
	   * La vuelta finalizará cuando el objeticvo se encuentre entre los valores angulo y aux */
	  double aux = angulo -5;
	  if (aux < 0) aux += 360;
	  
	  // Se comprueba la finalización del proceso
	  if (angulo > aux){
		  if (angulo > objetivo && objetivo > aux) objetivo = -1;
	  }
	  else if (objetivo > aux) objetivo = -1;
	  
	  // Se publican los datos en consecuencia
	  if (objetivo == -1) publicar_finalizacion();
	  else publicar_movimiento();
  }
  
  void comienzo(const std_msgs::Bool::ConstPtr& msg){
	  ROS_INFO("Recibida señal de comienzo de giro");
	  objetivo = angulo - 10;
	  if (objetivo < 0) objetivo += 360;
  }
  
  void publicar_movimiento(){
	  geometry_msgs::Twist movimiento;
	  movimiento.angular.z = 0.5;
	  mov_.publish(movimiento);
  }
  
  void publicar_finalizacion(){
	  std_msgs::Bool fin;
	  fin.data = true;
	  end_.publish(fin);
  }
  
private:
  // Parametros de ROS
  ros::NodeHandle nodo_;
  ros::Publisher mov_;
  ros::Publisher end_;
  ros::Subscriber odom_;
  ros::Subscriber start_;

  // Parametros del programa
  double angulo; // Para tener un control de la orientación del robot en todo momento
  double objetivo; // Valor al que se requiere llegar para haber completado la vuelta

};//End of class


int main (int argc, char **argv)
{

  ros::init(argc, argv, "dar_vuelta");

  ROS_INFO("Creando nodo para girar");
  vuelta girar;

  ros::spin();


  return 0;
}
