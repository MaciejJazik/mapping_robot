#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;
  

  while(n.ok())
  {
    /*broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"base_footprint", "base_link"));*/

    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(1.57, 0.0, 1.57)), tf::Vector3(-0.2, 0.0, 0.1)),
        ros::Time::now(),"base_link", "wheel_left"));

    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(1.57, 0.0, 1.57)), tf::Vector3(0.2, 0.0, 0.1)),
        ros::Time::now(),"base_link", "wheel_right"));
    
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(0.0, 0.0, -1.57)), tf::Vector3(0.0, -0.1827, 1.216)),
        ros::Time::now(),"base_link", "kinect"));
        
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(0.0, 0.0, 0.0)), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"kinect", "camera_link"));

    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(0.0, 0.0, 0.0)), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"camera_link", "camera_depth_frame"));

        
	/*broadcaster.sendTransform(
	  tf::StampedTransform(
		tf::Transform(tf::Quaternion(tf::createQuaternionFromRPY(0.0,0.0,0.0)), tf::Vector3(0.0,0.0,0.0)),
		ros::Time::now(),"base_link","sensor"));*/
        
    r.sleep();
  }
}