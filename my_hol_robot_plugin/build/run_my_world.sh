locate libDepthCameraPlugin.so /usr/lib/x86_64-linux-gnu/gazebo-9/plugins/libDepthCameraPlugin.so
locate libgazebo_ros_imu_sensor.so /usr/lib/x86_64-linux-gnu/gazebo-9/plugin/libgazebo_ros_imu_sensor.so
. /usr/share/gazebo-9/setup.sh && gazebo -s libgazebo_ros_api_plugin.so --verbose ../my_hol_robot.world

