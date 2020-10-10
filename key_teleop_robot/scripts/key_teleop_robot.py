#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty

msg = """
MOVE:
	w
a		d
	x

q/e - increase, decrease speed by 0.1
z/c - increase, decrease turn by 0.05
s - stop

"""
moveBindings = {
	'w':(1,0,0,0),
	'a':(0,0,0,1),
	'd':(0,0,0,-1),
	'x':(-1,0,0,0),
}

speedBindings = {
	'q':(0.1,0),
	'e':(-0.1,0),
    'z':(0,0.05),
    'c':(0,-0.05),
}


def getKey():
    tty.setraw(sys.stdin.fileno())
    select.select([sys.stdin], [], [], 0)
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def vels(speed,turn):
    return "currently:\tspeed %s\tturn %s " % (speed,turn)


if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    pub = rospy.Publisher('mapping_robot/cmd_vel', Twist, queue_size = 1)
    rospy.init_node('key_teleop_robot')

    speed = rospy.get_param("~speed", 0.5)
    turn = rospy.get_param("~turn", 0.1)
    x = 0
    y = 0
    z = 0
    th = 0

    try:
        print(msg)
        print(vels(speed,turn))
        while(1):
            key = getKey()
            if key in moveBindings.keys():
                x = moveBindings[key][0]
                y = moveBindings[key][1]
                z = moveBindings[key][2]
                th = moveBindings[key][3]
            elif key in speedBindings.keys():
                temp_speed = speed
                temp_turn = turn
                speed = speed + speedBindings[key][0]
                turn = turn + speedBindings[key][1]

                if speed > 3.1 or speed < -3.1 or turn > 2.05 or turn < -2.05:
                    speed = temp_speed
                    turn = temp_turn

                print(vels(speed,turn))
            elif key == 's':
                speed = 0.0
                turn = 0.0
                print(vels(speed,turn))
            else:
                if (key == '\x03'):
                    break
                else:
                    print('wrong key')

            twist = Twist()
            twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
            twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
            pub.publish(twist)

    except Exception as e:
        print(e)

    finally:
        twist = Twist()
        twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
        pub.publish(twist)

        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
