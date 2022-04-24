cmake_minimum_required(VERSION 3.0.2)
project(docking)

find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)

add_service_files(FILES xyInput.srv desired_position.srv Init.srv)
add_message_files(FILES limit.msg)

generate_messages( DEPENDENCIES std_msgs)

catkin_package(
 INCLUDE_DIRS include 
 LIBRARIES docking
 CATKIN_DEPENDS roscpp std_msgs message_runtime
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(motor_controller src/motor_controller.cpp)
add_dependencies(motor_controller docking_generate_messages_cpp)
target_link_libraries(motor_controller ${catkin_LIBRARIES})



