#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Byte.h"
#include "vector"
#include "nav_msgs/Odometry.h"
#include "math.h"

double matriz[12][5];
int obj;
int obj_ret;
int color_ret;
int aruco_ret=0;
int obj_prof;
double pose_x;
double pose_y;
double pose_odom_x;
double pose_odom_y;
double orientacion;
float yaw;
double p_x_o;
double p_y_o;


ros::Publisher target_mov;

void forma(const std_msgs::Float32MultiArray &Data)
{
    if (Data.data.size()!=0)
    {
        obj=Data.data[0];
        obj=obj/10;
        switch (obj)
        {
            case 1:
                std::cout << "OBJETO: AVION"<< std::endl;
            break;
            case 2:
                std::cout << "OBJETO: BICI"<< std::endl;
            break;
            case 3:
                std::cout << "OBJETO: BARCO"<< std::endl;
            break;
            case 4:
                std::cout << "OBJETO: BOTELLA"<< std::endl;
            break;
        }
        //std::cout << "numero : "<< obj <<std::endl;
    }

}

void color(const std_msgs::Float64MultiArray &Color)
{
    obj_ret=(int)Color.data[0];
    color_ret=(int)Color.data[1];


    pose_x=Color.data[2]+p_x_o;
    pose_y=Color.data[3]+p_y_o;


    switch (obj_ret)
    {
        case 1:
            switch (color_ret)
            {
                case 1:
                    matriz[0][1]=1;
                    matriz[0][2]=1;
                    matriz[0][3]=pose_x;
                    matriz[0][4]=pose_y;
                break;
                case 2:
                    matriz[1][1]=1;
                    matriz[1][2]=2;
                    matriz[1][3]=pose_x;
                    matriz[1][4]=pose_y;
                break;
                case 3:
                    matriz[2][1]=1;
                    matriz[2][2]=3;
                    matriz[2][3]=pose_x;
                    matriz[2][4]=pose_y;
                break;
            }
        break;
        case 2:
            switch (color_ret)
            {
                case 1:
                    matriz[3][1]=2;
                    matriz[3][2]=1;
                    matriz[3][3]=pose_x;
                    matriz[3][4]=pose_y;
                break;
                case 2:
                    matriz[4][1]=2;
                    matriz[4][2]=2;
                    matriz[4][3]=pose_x;
                    matriz[4][4]=pose_y;
                break;
                case 3:
                    matriz[5][1]=2;
                    matriz[5][2]=3;
                    matriz[5][3]=pose_x;
                    matriz[5][4]=pose_y;
                break;
            }
        break;
        case 3:
            switch (color_ret)
            {
                case 1:
                    matriz[6][1]=3;
                    matriz[6][2]=1;
                    matriz[6][3]=pose_x;
                    matriz[6][4]=pose_y;
                break;
                case 2:
                    matriz[7][1]=3;
                    matriz[7][2]=2;
                    matriz[7][3]=pose_x;
                    matriz[7][4]=pose_y;
                break;
                case 3:
                    matriz[8][1]=3;
                    matriz[8][2]=3;
                    matriz[8][3]=pose_x;
                    matriz[8][4]=pose_y;
                break;
            }
        break;
        case 4:
            switch (color_ret)
            {
                case 1:
                    matriz[9][1]=4;
                    matriz[9][2]=1;
                    matriz[9][3]=pose_x;
                    matriz[9][4]=pose_y;
                break;
                case 2:
                    matriz[10][1]=4;
                    matriz[10][2]=2;
                    matriz[10][3]=pose_x;
                    matriz[10][4]=pose_y;
                break;
                case 3:
                    matriz[11][1]=4;
                    matriz[11][2]=3;
                    matriz[11][3]=pose_x;
                    matriz[11][4]=pose_y;
                break;
            }
        break;
    }

    //IMPRIMIR MATRIZ

}
void aruco(const std_msgs::Byte &aruco)
{
    std_msgs::Float64MultiArray target;
    target.data.resize(2);
    //std::cout << "aviso" << std::endl;
   if (aruco_ret==0)
   {
        if (aruco.data!=100)
        {
            aruco_ret=aruco.data;
            if (matriz[aruco_ret-1][3]!=0 && matriz[aruco_ret-1][4]!=0)
            {
                //PUBLICAR QUE DEBE IRSE A ESA DIRECCION
                target.data[0]=matriz[aruco_ret-1][3];
                target.data[1]=matriz[aruco_ret-1][4];
            }
            else
            {
                //PUBLICAR UNA SEÑAL DE CONTINUAR EN LA NAVEGACIÓN
                target.data[0]=100.0;
                target.data[1]=100.0;
            }
            //std::cout << "ARUCO "<< aruco_ret << std::endl;
        }
        else
        {
            //std::cout << "NO VEO NADA" << std::endl;
            target.data[0]=100.0;
            target.data[1]=100.0;

        }
    }
   else
   {
       if (matriz[aruco_ret-1][3]!=0 && matriz[aruco_ret-1][4]!=0)
       {
           //PUBLICAR QUE DEBE IRSE A ESA DIRECCION
           target.data[0]=matriz[aruco_ret-1][3];
           target.data[1]=matriz[aruco_ret-1][4];
       }
       else
       {
           //PUBLICAR UNA SEÑAL DE CONTINUAR EN LA NAVEGACIÓN
           target.data[0]=100.0;
           target.data[1]=100.0;
       }
   }
    target_mov.publish(target);

}

void pose_real(const nav_msgs::Odometry &odom_real)
{
    pose_odom_x=odom_real.pose.pose.position.x;
    pose_odom_y=odom_real.pose.pose.position.y;

    std::cout << "INICIO TABLA : " << std::endl;

    for (int i=0;i<12;i++)
    {
        for (int j=0;j<5;j++)
        {
            std::cout << " || "<< matriz[i][j] << "||";
        }
        std::cout<<std::endl;
    }
    std::cout << "ID DEL ARUCO : "<< aruco_ret << std::endl;
}



int main(int argc, char **argv)
{

    for (int i=0;i<12;i++)
    {
        matriz[i][0]=i+1;
    }


    ros::init(argc, argv, "err_eval");

    ros::NodeHandle pnh("~");
    double param1;
    double param2;

    pnh.getParam("p_x_o",param1);
    pnh.getParam("p_y_o",param2);

    p_x_o=param1;
    p_y_o=param2;

    ros::NodeHandle n;
    ros::Subscriber rec_imagen=n.subscribe("/objects",1000,forma);

    ros::NodeHandle s;
    ros::Subscriber rec_color=s.subscribe("/id_color",1000,color);
    
    ros::NodeHandle t;
    ros::Subscriber rec_aruco=t.subscribe("/id/aruco",1000,aruco);

    ros::NodeHandle u;
    ros::Subscriber posicion=u.subscribe("/odometry/filtered",1000,pose_real);

    ros::NodeHandle v;
    target_mov=v.advertise<std_msgs::Float64MultiArray>("target/obj",50);

    ros::spin();
    return 0;
}
