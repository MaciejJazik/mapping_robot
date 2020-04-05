#! /usr/bin/env python

import rospy
from sensor_msgs.msg import LaserScan
 
def callback(msg):
    print 'angle 0'
    print msg.ranges[0]
    print 'angle 29'
    print msg.ranges[320]
    print 'angle 58'
    print msg.ranges[639]
    
 
rospy.init_node('scan_values')
sub = rospy.Subscriber('/scan', LaserScan, callback)
rospy.spin()
