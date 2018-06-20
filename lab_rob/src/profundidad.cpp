#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "sensor_msgs/LaserScan.h"
#include <std_msgs/Float32MultiArray.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>


using namespace std;

long long convertDecimalToBinary(int);
int convertBinaryToDecimal(long long);
void poner_matriz(float);

int dimx=640;  //*********COLUMNAS DE LA MATRIZ*********88
int dimy=1;  //*********FILAS DE LA MATRIZ**************

int cont2=0;
int cont=0;
int dato1, n;
int dato2;
unsigned int dato_c2;
unsigned int dato_c1;
std::string aux="";
std::string aux2="";
std::string aux3="";
int cuenta=0;
long dato_concatenado;
int distancia;
float depth;
int col=0, fila=0;
ostringstream dato3;
float profundidad[1][640];
//unsigned int num_readings = 4;
float rango_imagen[1*640];

//double intensities[100*110];

float img_color[3][640];
int cont_color=0;
int col_color=0;

ros::Publisher imagen_profundidad;
//ros::Rate r(1.0);
void imagen(const sensor_msgs::Image &Data)
{

    //std::cout << std::endl;

    for (int i=(2*(640*150));i<(2*(640*151));i++)
    {
        cont=cont+1;
        if (cont==1)
        {
            dato1=Data.data[i];
            dato_c1=convertDecimalToBinary(dato1);
            //std::cout << "1: " << sizeof(dato_c1) << std::endl;
            aux= static_cast<std::ostringstream*>(&(std::ostringstream() << dato_c1))->str();
            if (aux.size()!=8)
            {
                cuenta=8-aux.size();
                switch(cuenta)
                {
                    case 1: aux2="0"+aux;
                    break;
                    case 2: aux2="00"+aux;
                    break;
                    case 3: aux2="000"+aux;
                    break;
                    case 4: aux2="0000"+aux;
                    break;
                    case 5: aux2="00000"+aux;
                    break;
                    case 6: aux2="000000"+aux;
                    break;
                    case 7: aux2="0000000"+aux;
                    break;
                }
            }
            else
            {
                aux2=aux;
            }
            //std::cout << "1: " << aux2 << std::endl;
            //cuenta=0;
            //istringstream buffer(aux2);
            //buffer >> dato_c1;
            //std::cout << "1: " << dato_c1 << std::endl;

        }
        else
        {
            if (cont==2)
            {

                dato2=Data.data[i];
                dato_c2=convertDecimalToBinary(dato2);
                aux3= static_cast<std::ostringstream*>(&(std::ostringstream() << dato_c2))->str();
                //std::cout << "2: " << dato_c2 << std::endl;

                dato3<<aux3+aux2;

                istringstream iss(dato3.str());
                iss >> dato_concatenado;

                dato3.str("");

                distancia=convertBinaryToDecimal(dato_concatenado);
                depth=((float)distancia/(float)1000);

                col=col+1;
//                if (depth>0.5 && depth <2.5)
//                {
                    poner_matriz(depth);
//                }
//                else
//                {
//                    poner_matriz(0);
//                }


                //****************ALGORITMO PARA PUBLICAR LA IMAGEN COMO RANGOS*****************************


                if (col==dimx && fila==dimy-1)  //*****************OJO VERIFICAR**********************88
                {
                    sensor_msgs::LaserScan prof;

                    ros::Time scan_time = ros::Time::now();
                    prof.angle_min=-0.49742;
                    prof.angle_max=0.49742;
                    prof.header.frame_id="/front_laser";
                    prof.header.stamp = scan_time;
                    prof.time_increment=0.000039062;
                    prof.angle_increment=0.0015625;
                    //prof.angle_increment=0.24871;
                    prof.range_min=0.0;
                    prof.range_max=8.0;
                    prof.ranges.resize(dimx*dimy);
                    prof.intensities.resize(dimx*dimy);
                        for (int i=0;i<dimy;i++)
                        {
                            for(int j=dimx-1;j>-1;j--)
                            {
                                if ((profundidad[i][j]==0)  || ((img_color[1][j] == img_color[2][j]) && img_color[2][j]>100 ))
                                {
                                    profundidad[i][j]=INFINITY;

                                }
                                //std::cout << "pixel 1: " << img_color[1][1]  << std::endl;
                                prof.ranges[cont2]=profundidad[i][j];
                                prof.intensities[cont2]=100;
                                cont2=cont2+1;
                                if (cont2==dimx*dimy)
                                {
                                    cont2=0;
                                }
                            }
                        }
                    imagen_profundidad.publish(prof);
                    //r.sleep();
                }

                cont=0;
             }
        }
    }
}

void color(const sensor_msgs::Image &Data2)
{
    for (int i=(3*(640*150));i<(3*(640*151));i++)
    {
        cont_color=cont_color+1;
        if (cont_color==1)
        {
            img_color[0][col_color]=Data2.data[i];
        }
        else
        {
            if (cont_color==2)
            {
                img_color[1][col_color]=Data2.data[i];
            }
            else
            {
                if (cont_color==3)
                {
                    img_color[2][col_color]=Data2.data[i];
                    cont_color=0;
                    col_color=col_color+1;
                    if (col_color==640)
                    {
                        col_color=0;
                    }
                }
            }
        }


    }
    
}

//**************CONVERSION DE DECIMAL A BINARIO***********************************

long long convertDecimalToBinary(int n)
{
    long long binaryNumber = 0;
    int remainder, i = 1, step = 1;

    while (n!=0)
    {
        remainder = n%2;
        //cout << "Step " << step++ << ": " << n << "/2, Remainder = " << remainder << ", Quotient = " << n/2 << endl;
        n /= 2;
        binaryNumber += remainder*i;
        i *= 10;
    }
    return binaryNumber;
}
//**************CONVERSION DE BINARIO A DECIMAL***********************************

int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}


//********************RECONSTRUCCION DE LA MATRIZ********************************

void poner_matriz(float valor)
{

    if(col==dimx+1)
    {
        fila=fila+1;
        col=1;
        if (fila==dimy)
        {
            fila=0;
        }
    }
    profundidad[fila][col-1]=valor;

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_profundidad");
    
    ros::NodeHandle s;
    imagen_profundidad=s.advertise<sensor_msgs::LaserScan>("front/scan",50);

    ros::NodeHandle n;
    ros::Subscriber imagen_depth = n.subscribe("/camera/depth/image",1000, imagen);
    
    ros::NodeHandle m;
    ros::Subscriber imagen_color = m.subscribe("/camera/rgb/image_raw",1000, color);

    ros::spin();
    return 0;
}



