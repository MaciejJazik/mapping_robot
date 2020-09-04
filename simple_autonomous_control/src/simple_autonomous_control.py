#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from darknet_ros_msgs.msg import ObjectCoordinates
from std_msgs.msg import Float64
 

speed = 0
turn = 0
x = 0
y = 0
z = 0
th = 0
power=0
pub = rospy.Publisher('mapping_robot/cmd_vel', Twist, queue_size = 10)


def detection_callback(data):
    global speed, turn, x, y, z, th
    if data.Z >= 1000 and  -5 < data.alpha < 5:
        x=1
        th=0
        if speed < 3:
            speed+=0.05
        print('forward')
    elif data.Z < 500:
        if speed > 0:
            speed-=0.05
        th=0
        print('stop')
    elif data.Z >= 1000 and data.alpha > 5:
        if speed > 0:
            speed-=0.05
        else:
            x=0
            th=1
            turn=-1
        print('turn right')
    elif data.Z >= 1000 and data.alpha < -5:
        if speed > 0:
            speed-=0.05
        else:
            x=0
            th=1
            turn=1 
        print('turn left')
    twist = Twist()
    twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
    twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn 
    pub.publish(twist)

def control():
    rospy.init_node('simple_autonomous_control', anonymous = True)
    rospy.Subscriber('/darknet_ros/object_coordinates', ObjectCoordinates, detection_callback)
    twist = Twist()
    twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
    twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
    pub.publish(twist)
    rospy.spin()


if __name__ == '__main__':
       try:
          control()
       except rospy.ROSInterruptException:
          pass
