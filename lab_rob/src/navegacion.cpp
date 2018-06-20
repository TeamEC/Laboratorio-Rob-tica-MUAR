#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/LaserScan.h"
#include <move_base/move_base.h>
#include <stdlib.h>
#include <tf/transform_datatypes.h>
#include <sstream>
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Byte.h"
#include "nav_msgs/Path.h"
#include "vector"
#include <iostream>
#include <string>
#include <unistd.h>


double cont=0;
int cont_restringido=0;
int bloqueo=0;
int cont2=0;
int flag_inicio=0;
int flag_objetivo=0;
int flag_posible=0;
int paso_pto=0;
int posex;
int posey;
float girow;
int i=0;
int bandera=1;

short contador=0;
float p_0x=8.0;                 //*************PONER AQUI POSICION INICIAL DEL ROBOT*****************************
float p_0y=-8.0;                 //*************PONER AQUI POSICION INICIAL DEL ROBOT*****************************
double p_xaruco;
double p_yaruco;

double timer=0;

int paso_aruco=0;

double radians = -90 * (M_PI/180);

float targets[14][2]={{p_xaruco-p_0x,p_yaruco-p_0y},{8.5-p_0x,8.5-p_0y},{-8.5-p_0x,8.5-p_0y},{-8.5-p_0x,-8.5-p_0y},{8.5-p_0x,-8.5-p_0y},{8.5-p_0x,8.5-p_0y},{-8.5-p_0x,-8.5-p_0y},{8.5-p_0x,0.0-p_0y},{0.0-p_0x,8.5-p_0y},{-8.5-p_0x,0.0-p_0y},{0.0-p_0x,-8.5-p_0y},{8.5-p_0x,-8.5-p_0y},{-8.5-p_0x,8.5-p_0y},{0.0,0.0}};
float targets2[14][2]={{0.0-p_0x,-8.0-p_0y},{0.0-p_0x,8-p_0y},{-4.5-p_0x,0.0-p_0y},{8.5-p_0x,0.0-p_0y},{4.5-p_0x,-4.5-p_0y},{8.5-p_0x,8.5-p_0y},{8.5-p_0x,-8.5-p_0y},{-4.5-p_0x,-4.5-p_0y},{-4.5-p_0x,4.5-p_0y},{4.5-p_0x,4.5-p_0y},{-4.5-p_0x,-4.5-p_0y},{0.0-p_0x,-0.0-p_0y},{0.0-p_0x,-8.5-p_0y},{0.0,0.0}};


int salir=0;

float target_x;
float target_y;
int contador_target=0;

double retardo;
int paso=0;

ros::Publisher lab_goal;

ros::Publisher velocidad;



void posicion(const nav_msgs::Odometry &Data)
{

    timer++;
    posex=Data.pose.pose.position.x;
    posey=Data.pose.pose.position.y;


}

void target(const std_msgs::Float64MultiArray &pose_target)
{


    target_x=pose_target.data[0];
    target_y=pose_target.data[1];

    if (target_x != 100 && target_y != 100)
    {
        contador_target++;
        if(contador_target==1)
        {

            switch (flag_posible)
            {
                case 0:
                    targets[12][0]=(target_x-p_0x);
                    targets[12][1]=(target_y-p_0y);
                break;
                case 1:
                    targets[12][0]=(target_x-p_0x)-0.35;
                    targets[12][1]=(target_y-p_0y)-0.35;
                break;
                case 2:
                    targets[12][0]=(target_x-p_0x)-0.35;
                    targets[12][1]=(target_y-p_0y)+0.35;
                break;
                case 3:
                    targets[12][0]=(target_x-p_0x)+0.35;
                    targets[12][1]=(target_y-p_0y)-0.35;
                break;
                case 4:
                    targets[12][0]=(target_x-p_0x)+0.35;
                    targets[12][1]=(target_y-p_0y)+0.35;
                break;
            }
            i=12;
            cont=0;
            paso_pto=1;
            timer=0;
        }
        else
        {
            contador_target=1;
        }

    }

    geometry_msgs::PoseStamped goal;
    goal.header.frame_id="odom";
    goal.header.stamp=ros::Time::now();

    tf::Quaternion quaternion;
    quaternion = tf::createQuaternionFromYaw(radians);

    geometry_msgs::Quaternion qMsg;
    tf::quaternionTFToMsg(quaternion, qMsg);

    goal.pose.orientation = qMsg;

    cont++;
    if (cont==1)
    {

        goal.pose.position.x=targets[i][0];
        goal.pose.position.y=targets[i][1];
        lab_goal.publish(goal);

    }

    if(((((int)posex==(int)targets[i][0]-1 || (int)posex==(int)targets[i][0]+1 || (int)posex==(int)targets[i][0] ) && ((int)posey==(int)targets[i][1]-1 || (int)posey==(int)targets[i][1]+1 || (int)posey==(int)targets[i][1]))  || timer>=3000 || (i==0 && paso_aruco==1) ) && paso_pto==0)
    {
        timer=0;
        if(i<13)
        {
            i++;
        }
        cont=0;
        if (i==13 && paso_pto==0 && salir==0)
        {
            for(int kk=0;kk<=13;kk++)
            {
                targets[kk][0]=targets2[kk][0];
                targets[kk][1]=targets2[kk][1];
            }
           i=0;
           cont=0;
           salir=1;
        }

    }
    else
    {
		
        if((((((int)posex==(int)(targets[i][0]-1) || (int)posex==(int)(targets[i][0]+1) || (int)posex==(int)targets[i][0] ) && ((int)posey==(int)(targets[i][1]-1) || (int)posey==(int)(targets[i][1]+1) || (int)posey==(int)targets[i][1]))) || timer>=3000) && paso_pto==1)
        {
            if (i==12)
            {
                std::cout << "ENTREGANDO AYUDA" <<std::endl;
                usleep(10000000);
            }
            if(i<13)
            {
                i++;
            }
            cont=0;

        }
    }

std::cout << "TIMER  " << timer <<std::endl;
std::cout << "PUNTO DE VISITA  " << i <<std::endl;
if (i==13)
{
    timer=0;
}


}

void posible(const nav_msgs::Path &Data_path)
{
    if (Data_path.header.frame_id.empty())
    {

        cont_restringido++;
        if (cont_restringido==5)
        {
            std::cout << "CAMBIANDO PUNTO" <<std::endl;
            if (paso_pto==1)
            {
                contador_target=0;
                flag_posible++;
                cont_restringido=0;
            }
            else
            {
                targets[i][0]=targets[i][0]+1;
                targets[i][1]=targets[i][1]+1;
                cont=0;
                cont_restringido=0;
            }
        }
    }
}

void aruco(const std_msgs::Byte &aruco)
{
    if(paso_aruco==0)
    {
        if (aruco.data != 100)
        {
            paso_aruco=1;
        }
    }

}

int main(int argc, char **argv)
{
 
    
    
    ros::init(argc, argv, "err_eval");

    ros::NodeHandle pnh("~");
    double param1;
    double param2;


    pnh.getParam("p_xaruco",param1);
    pnh.getParam("p_yaruco",param2);

    p_xaruco=param1;
    p_yaruco=param2;

    targets[0][0]=p_xaruco-p_0x;
    targets[0][1]=p_yaruco-p_0y;

    ros::NodeHandle n;
    ros::Subscriber position=n.subscribe("/odometry/filtered",1000,posicion);

    ros::NodeHandle m;
    ros::Subscriber target_mov=m.subscribe("/target/obj",1000,target);

    ros::NodeHandle p;
    ros::Subscriber target_posible=p.subscribe("/move_base/NavfnROS/plan",1000,posible);

    ros::NodeHandle r;
    lab_goal=r.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1000);

    ros::NodeHandle t;
    ros::Subscriber rec_aruco=t.subscribe("/id/aruco",1000,aruco);

    ros::spin();
    return 0;
}
