#include <ros/ros.h>
#include <vector>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/LaserScan.h"
#include "nav_msgs/Odometry.h"

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
    scan_ = nodo_.subscribe("scan",0, &pared::datos_laser, this);
    start_ = nodo_.subscribe("pared",0, &pared::comienzo, this);
     odom_ = nodo_.subscribe("odom",0, &pared::calcular_distancia, this);
    
    derecha.resize(5);
    centro.resize(5);
    
    mover = false;
    distancia.x = 0;
    distancia.y = 0;
  }


  void datos_laser(const sensor_msgs::LaserScan::ConstPtr& msg){
	  // guardamos aquí los datos necesarios
	  
	  int size = msg->ranges.size();
	  
	  for (int i = 0; i<10; i++){
		  if (i<5) derecha[i] = msg->ranges[i*(size / 15 - 1)];
		  else centro[i-5] = msg->ranges[i*(size / 15 - 1)];
	  }
  }
  
  void calcular_distancia(const nav_msgs::Odometry::ConstPtr& msg){
	  // Primero calculamos la posicón actual del robot
	  actual.x = msg->pose.pose.position.x;
	  actual.y = msg->pose.pose.position.y;
	  
	  // Ahora, si se está moviendo, se calcula la distancia entre ambos puntos
	  float ecl;
	  if (mover){
		  ecl = sqrt(pow((actual.x - distancia.x),2) + pow((actual.y - distancia.y),2));
		  //cout << "Distancia = " << ecl << endl;
		  if (ecl < 2) publicar_movimiento();
		  else publicar_finalizacion();
	 }
	  
  }
  
  void comienzo(const std_msgs::Bool::ConstPtr& msg){
	  ROS_INFO("Petición de seguimiento de pared recibida");
	  mover = true;
	  distancia.x = actual.x;
	  distancia.y = actual.y;
  }
  
  void publicar_finalizacion(){
	  mover = false;
	  std_msgs::Bool turn;
	  turn.data = true;
	  end_.publish(turn);
	  ROS_INFO("Fin del seguimiento de pared");
  }
  
  void publicar_movimiento(){
	  geometry_msgs::Twist movimiento;
	  movimiento.linear.x = 0.1;
	  // Calculamos el movimiento linear necesario
	  for (int i = 0; i<5; i++){
		  if (centro[i] < 1.2){
			  movimiento.linear.x = 0;
			  break;
		  }
	  }
	  
	  // Calculamos el movimiento angular necesario
	  movimiento.angular.z = 0;
	  for (int i = 0; i<5; i++){
		  if (derecha[i] < 1) movimiento.angular.z += 0.02;
		  else movimiento.angular.z -= 0.02;
	  }
	  
	  // Por último, se publica
	  mov_.publish(movimiento);
  }
  
  
  
private:
  // Parametros ROS
  ros::NodeHandle nodo_;
  ros::Publisher mov_;
  ros::Publisher end_;
  
  ros::Subscriber scan_;
  ros::Subscriber start_;
  ros::Subscriber odom_;
  
  //Parametros del programa
  vector <float> derecha; // Guardamos lo datos de un cierto numero de valores de scan
  vector <float> centro;

  // Creamos una variable que nos servirá para decir si el robot está lo suficientemente lejos como para empezar a girar
  geometry_msgs::Point distancia;
  geometry_msgs::Point actual;
  bool mover;

};//End of class


int main (int argc, char **argv)
{

  ros::init(argc, argv, "seguir_pared");

  ROS_INFO("Creando nodo para seguir la pared");
  pared seguimiento;

  ros::spin();


  return 0;
}
