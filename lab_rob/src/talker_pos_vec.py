#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import MultiArrayDimension


def talker(args):
    pub = rospy.Publisher('id_color', Float32MultiArray, queue_size=1)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(100)
    count = 0
    while (count < 5):

        mat = [4,3,-2.252,14.566]
        pub_array = Float32MultiArray(data = mat)
                #hello_str = "340,420 %s" %rospy.get_time()
                #rospy.loginfo(hello_str)
        pub.publish(pub_array)
        count = count + 1
        rate.sleep()




if __name__ == '__main__':
    try:
        import sys, getopt
        talker(sys.argv)
    except rospy.ROSInterruptException:
        pass
