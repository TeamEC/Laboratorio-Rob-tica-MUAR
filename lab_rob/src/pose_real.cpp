#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

double p_x_o;
double p_y_o;
float p_x_rect, p_y_rect;
float p_x_act, p_y_act;
char destination[2];


ros::Publisher odom_rectificada;

void odom_rect(const nav_msgs::Odometry &Data)
{
    p_x_act=Data.pose.pose.position.x;
    p_y_act=Data.pose.pose.position.y;
    
    p_x_rect=p_x_act+p_x_o;
    p_y_rect=p_y_act+p_y_o;

    nav_msgs::Odometry pose;


    pose.pose.pose.position.x=p_x_rect;
    pose.pose.pose.position.y=p_y_rect;
    pose.pose.pose.position.z=Data.pose.pose.position.z;


    pose.pose.pose.orientation.x=Data.pose.pose.orientation.x;
    pose.pose.pose.orientation.y=Data.pose.pose.orientation.y;
    pose.pose.pose.orientation.w=Data.pose.pose.orientation.w;
    pose.pose.pose.orientation.z=Data.pose.pose.orientation.z;


    odom_rectificada.publish(pose);


}




int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_real");

    ros::NodeHandle pnh("~");
    double param1;
    double param2;


    pnh.getParam("p_x_o",param1);
    pnh.getParam("p_y_o",param2);

    p_x_o=param1;
    p_y_o=param2;


    ros::NodeHandle n;
    ros::Subscriber posicion_rect = n.subscribe("odometry/filtered",1000, odom_rect);



    ros::NodeHandle s;
    odom_rectificada=s.advertise<nav_msgs::Odometry>("odometry/rectificada",50);




    ros::spin();
    return 0;
}
