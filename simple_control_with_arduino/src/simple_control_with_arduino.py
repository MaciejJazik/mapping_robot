#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from darknet_ros_msgs.msg import ObjectCoordinates
from std_msgs.msg import Float64
 
mode = '0'
pub = rospy.Publisher('arduino_msg', String, queue_size = 10)


def detection_callback(data):
    global mode
    if data.Z >= 1000 and  -5 < data.alpha < 5:
        mode = '1'
        print('forward')
    elif data.Z < 1000:
        mode = '0'
        print('stop')
    elif data.Z >= 1000 and data.alpha > 5:
        mode = '2'
        print('turn right')
    elif data.Z >= 1000 and data.alpha < -5:
        mode = '3'
        print('turn left')
    pub.publish(mode)

def control():
    rospy.init_node('simple_autonomous_control', anonymous = True)
    rospy.Subscriber('/darknet_ros/object_coordinates', ObjectCoordinates, detection_callback)
    pub.publish(mode)
    rospy.spin()


if __name__ == '__main__':
       try:
          control()
       except rospy.ROSInterruptException:
          pass
