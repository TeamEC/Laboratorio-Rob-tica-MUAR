#include "ros/ros.h"
#include <stdlib.h>
#include <sstream>
#include "aruco_msgs/Marker.h"
#include "aruco_msgs/MarkerArray.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/UInt32MultiArray.h"
#include "vector"
#include "std_msgs/Byte.h"


int identificador;
int contador_1=0;
int contador_2=0;
float pos;

ros::Publisher numero_aruco;


void detectar(const std_msgs::UInt32MultiArray &Data)
{
    std_msgs::Byte iden;

    if (Data.data.size() == 0)
    {
        contador_1++;
        identificador=100;
        iden.data=identificador;
        numero_aruco.publish(iden);
        if(contador_1==1)
        {
          std::cout<<"Aruco No Encontrado"<<std::endl;
        }
    }

    else
    {
        contador_2++;
        identificador=Data.data[0];
        iden.data=identificador;
        numero_aruco.publish(iden);
        if(contador_2==1)
        {
          std::cout<<"Aruco Encontrado - ID:  "<<identificador<<std::endl;
        }

    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "aruco_detector");

    ros::NodeHandle det;
    ros::Subscriber detector =det.subscribe("aruco_marker_publisher/markers_list",1000,detectar);

    ros::NodeHandle aru;
    numero_aruco=aru.advertise<std_msgs::Byte>("id/aruco",1000);

    ros::spin();
    return 0;

}

