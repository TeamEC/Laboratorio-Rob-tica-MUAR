#!/usr/bin/env python

#cambiar xml <build_depend>python-numpy</build_depend>
PKG = 'numpy_tutorial'
import roslib; roslib.load_manifest(PKG)

import rospy
from rospy_tutorials.msg import Floats
from rospy.numpy_msg import numpy_msg #comentar

def callback(data):
    #print rospy.get_name(), "Vector %s"%str(data.data)
    print "Vector %s"%str(data.data)
def listener():
    rospy.init_node('listener')
    #rospy.Subscriber("floats", Floats, callback)
    rospy.Subscriber("floats", numpy_msg(Floats), callback)
    rospy.spin()

if __name__ == '__main__':
    listener()
