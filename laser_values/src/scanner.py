#! /usr/bin/env python

import rospy
from sensor_msgs.msg import LaserScan
 
def callback(msg):
    pub = rospy.Publisher('/scan', LaserScan, queue_size=10)
    msg.header.frame_id = 'camera_depth_frame'
    pub.publish(msg)
    
    
 
rospy.init_node('scan_values')
sub = rospy.Subscriber('/world', LaserScan, callback)
rospy.spin()
