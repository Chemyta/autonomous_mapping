Repositorio para guardar un trabajo simple para una asignatura de master.

La idea es crear un algoritmo para que el robot turtlebot pueda mapear una zona.
Se va a utilizar Turtlebor, Gazebo y ROS.

El algoritmo recorrerá el mapa siguiendo la pared y cada cierto tiempo dará una vuelta de reconocimiento

Si se requiere probar el algoritmo:
  1) Abrir el mapa en Gazebo 
  2) Lanzar el algoritmo de movimiento
	roslaunch autonomous_mapping decisiones.launch 
  3) Lanzar el algoritmo SLAM
	roslaunch autonomous_mapping gmapping_pared.launch
  4) Comenzar el movimiento publicando el topic /vuelta que el robot comience dando una vuelta:
	rostopic pub /vuelta std_msgs/Bool "data: true" -1
  5) Abrir Rviz para ver el proceso

