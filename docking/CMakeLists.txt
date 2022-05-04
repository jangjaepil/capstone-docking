cmake_minimum_required(VERSION 3.0.2)
project(docking)

find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation robot_msgs )

add_service_files(FILES  desired_position.srv Init.srv ppp.srv door.srv xyInput.srv)
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

add_executable(Init src/Init.cpp)
add_dependencies(Init docking_generate_messages_cpp)
target_link_libraries(Init ${catkin_LIBRARIES})

add_executable(ppp src/ppp.cpp)
add_dependencies(ppp docking_generate_messages_cpp)
target_link_libraries(ppp ${catkin_LIBRARIES})

add_executable(door src/door.cpp)
add_dependencies(door docking_generate_messages_cpp)
target_link_libraries(door ${catkin_LIBRARIES})

add_dependencies(door ${catkin_EXPORTED_TARGETS}) ##FOR USING robot_msgs


