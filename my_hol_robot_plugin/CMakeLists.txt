cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(roscpp REQUIRED)
find_package(std_msgs REQUIRED)
include_directories(${roscpp_INCLUDE_DIRS})
include_directories(${std_msgs_INCLUDE_DIRS})

# Find Gazebo
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Build our plugin
add_library(my_hol_robot_plugin SHARED /home/maciej/my_hol_robot_plugin/my_hol_robot_plugin.cc)
target_link_libraries(my_hol_robot_plugin ${GAZEBO_LIBRARIES} ${roscpp_LIBRARIES})

#add_library(gazebo_ros_imu SHARED gazebo_ros_imu.cpp)
#target_link_libraries(gazebo_ros_imu ${GAZEBO_LIBRARIES} ${roscpp_LIBRARIES})
