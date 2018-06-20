#!/usr/bin/env python

#INFORMACION IMPORTANTE!!!
#
#colores 3:rojo 2:verde 1:azul
#id: id del objeto
#Suscriptores:
#posicion/print_roi     (arreglo de 7 valores)[x1,y1,x2,y2,id,cp1,cp2]
#posicion/objeto        (x,y,co1, co2)
#Publicadores:
#id_color               (arreglo de 2 valores) [id,color,x,y]



import rospy, cv2, cv_bridge
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import Byte
from std_msgs.msg import String
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Float64MultiArray
from std_msgs.msg import Int32MultiArray
from std_msgs.msg import MultiArrayDimension



roi_vec = [0,0,0,0,0,0,0]
xy_vec = [0,0,0,0]

#Variables globales

red_area = 0
green_area = 0
blue_area = 0
red_area_val = 0
green_area_val = 0
blue_area_val = 0

class Color:

        def __init__(self):

                self.bridge = cv_bridge.CvBridge()
                #cv2.namedWindow("window", 1)

                self.image_sub = rospy.Subscriber('camera/rgb/image_raw',
                        Image, self.image_callback)

                self.roi_sub = rospy.Subscriber('posicion/print_roi', Int32MultiArray, self.data_callback)
                self.avent_sub = rospy.Subscriber('posicion/objeto', Float64MultiArray, self.obj_callback)
                #self.id_roi_sub = rospy.Subscriber('id_roi', Byte, self.id_callback)
                #self.color_pub = rospy.Publisher('id_color', Byte, queue_size=1)
                self.color_pub = rospy.Publisher('id_color', Float64MultiArray, queue_size=1)

        def data_callback(self, data):

                global roi_vec
                roi_vec = (data.data)

        def obj_callback(self, data):

                global xy_vec
                xy_vec = (data.data)



        def image_callback(self, msg):

                #print (roi_vec)
                #print (xy_vec)

                if (roi_vec != [0,0,0,0,0,0,0] and xy_vec !=[0,0,0,0]):
                        x1 = int(abs(roi_vec[1]))
                        y1 = int(abs(roi_vec[0]))
                        x2 = int(abs(roi_vec[3]))
                        y2 = int(abs(roi_vec[2]))
                        id = roi_vec[4]
                        cp1 = int(roi_vec[5])
                        cp2 = int(roi_vec[6])


                        x = float((xy_vec[0]))
                        y = float((xy_vec[1]))
                        co1 = int(xy_vec[2])
                        co2 = int(xy_vec[3])

                        #print (cp1,co1)

                        if (x == 0 and y == 0 and cp1 != co1 and cp2 != co2):

                                pass

                        else:
                                image = self.bridge.imgmsg_to_cv2(msg,desired_encoding='bgr8')
                                hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

                                #imagen hsv de ROI
                                img_roi_hsv = hsv[x1:x2, y1:y2]
                                img = image[x1:x2, y1:y2]


                                #Filtros
                                #rojo
                                l_range_r = np.array([0,100,76])
                                h_range_r =np.array([0,255,255])

                                red_mask = cv2.inRange(img_roi_hsv,l_range_r,h_range_r)
                                kernel = np.ones((1,1),np.uint8)
                                red_mask = cv2.morphologyEx(red_mask, cv2.MORPH_OPEN, kernel)
                                #kernel = np.ones((5,5),np.uint8)
                                #red_mask = cv2.dilate(red_mask,kernel,iterations = 1)

                                #verde
                                l_range_g = np.array([60,90,80])
                                h_range_g =np.array([110,255,185])

                                green_mask = cv2.inRange(img_roi_hsv,l_range_g,h_range_g)
                                kernel = np.ones((1,1),np.uint8)
                                green_mask = cv2.morphologyEx(green_mask, cv2.MORPH_OPEN, kernel)
                                #kernel = np.ones((5,5),np.uint8)
                                #green_mask = cv2.dilate(green_mask,kernel,iterations = 1)


                                #azul
                                l_range_b = np.array([110,50,50])
                                h_range_b =np.array([130,255,255])

                                blue_mask = cv2.inRange(img_roi_hsv,l_range_b,h_range_b)
                                kernel = np.ones((1,1),np.uint8)
                                blue_mask = cv2.morphologyEx(blue_mask, cv2.MORPH_OPEN, kernel)
                                #kernel = np.ones((5,5),np.uint8)
                                #blue_mask = cv2.dilate(blue_mask,kernel,iterations = 1)

                                # # FILTRO DE COLORES
                                # l_range_col = np.array([0,160,50])
                                # h_range_col =np.array([179,255,255])
                                #
                                # color_mask = cv2.inRange(img_roi_hsv,l_range_col,h_range_col)
                                # kernel = np.ones((1,1),np.uint8)
                                # color_mask = cv2.morphologyEx(color_mask, cv2.MORPH_OPEN, kernel)
                                # #kernel = np.ones((5,5),np.uint8)
                                # #color_mask = cv2.dilate(color_mask,kernel,iterations = 1)
                                #
                                #
                                # mascara = cv2.bitwise_and(image, image, mask=color_mask)

                                #######################################################################
                                # Con mascara de cada color

                                # nrojo = np.nonzero(red_mask) [0]
                                # nverde = np.nonzero(green_mask) [0]
                                # nazul = np.nonzero(blue_mask) [0]




                                im2,contours_r,hierarchy = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
                                if len(contours_r) != 0:

                                    red_area = max(contours_r, key = cv2.contourArea)
                                    global red_area_val
                                    red_area_val = cv2.contourArea(red_area)
                                    pass
                                else:
                                    global red_area_val
                                    red_area_val = 0
                                    pass




                                im2,contours_g,hierarchy = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

                                if len(contours_g) != 0:

                                    green_area = max(contours_g, key = cv2.contourArea)
                                    global green_area_val
                                    green_area_val = cv2.contourArea(green_area)
                                    pass
                                else:
                                    global green_area_val
                                    green_area_val = 0
                                    pass




                                im2,contours_b,hierarchy = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

                                if len(contours_b) != 0:

                                    blue_area = max(contours_b, key = cv2.contourArea)
                                    global blue_area_val
                                    blue_area_val = cv2.contourArea(blue_area)
                                    pass
                                else:
                                    global blue_area_val
                                    blue_area_val = 0
                                    pass


                                max_array = [red_area_val,green_area_val,blue_area_val]
                                index =max_array.index(max(max_array))

                                if index == 0:
                                    rate = rospy.Rate(100)
                                    rojo = 3
                                    mensaje = [id,rojo,x,y]
                                    if (mensaje[2] != 0 and mensaje[3] != 0 and cp1 == co1 and cp2 == co2):
                                        pub_array= Float64MultiArray(data = mensaje)
                                        self.color_pub.publish(pub_array)
                                        rate.sleep()
                                        pass
                                    pass

                                elif index == 1:
                                    rate = rospy.Rate(100)
                                    verde = 2
                                    mensaje = [id,verde,x,y]
                                    if (mensaje[2] != 0 and mensaje[3] != 0 and cp1 == co1 and cp2 == co2):
                                        pub_array= Float64MultiArray(data = mensaje)
                                        self.color_pub.publish(pub_array)
                                        rate.sleep()
                                        pass
                                    pass

                                elif index == 2:
                                    rate = rospy.Rate(100)
                                    azul = 1
                                    mensaje = [id,azul,x,y]
                                    if (mensaje[2] != 0 and mensaje[3] != 0 and cp1 == co1 and cp2 == co2):
                                        pub_array= Float64MultiArray(data = mensaje)
                                        self.color_pub.publish(pub_array)
                                        rate.sleep()
                                        pass
                                    pass



                                else:
                                        #     rate = rospy.Rate(100)
                                        #     mensaje = [0,0,0,0]
                                        #     pub_array= Float32MultiArray(data = mensaje)
                                        #     self.color_pub.publish(pub_array)
                                        #     rate.sleep()
                                        pass

                else:

                #     rate = rospy.Rate(100)
                #     mensaje = [0,0,0,0]
                #     pub_array= Float32MultiArray(data = mensaje)
                #     self.color_pub.publish(pub_array)
                #   rate.sleep()
                      pass



rospy.init_node('color_position_id')
color = Color()
rospy.spin()
