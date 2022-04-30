#include "ros/ros.h"
#include "docking/xyInput.h"
#include "docking/desired_position.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"


ros::ServiceClient* client1;
ros::ServiceClient* client2;

bool desired_positionCallback(docking::desired_position::Request &req, docking::desired_position::Response &res) //desired_position's type is mm and desired_position means relative postion
{
   
  float x = req.x; //relative position mm
  float y = req.y; //relative position mm
  float z = req.z; //absolute position mm
  res.done = 0;
  
  dynamixel_workbench_msgs::DynamixelCommand zInput;
  zInput.request.value = -(4096/360)*(360/100)*z; //resolutoin 4096  deg/mm = 360/100(wil be experienced)     
  zInput.request.id = 3;
  zInput.request.addr_name = "Goal_Position";
  zInput.response.comm_result = 0;
  
  docking::xyInput xyInput;
  xyInput.request.x = x;
  xyInput.request.y = y;
  xyInput.response.done = 0;
  ROS_INFO("done: %d", (int)xyInput.response.done);
   
   client1->call(zInput);
   client2->call(xyInput);
  //ROS_INFO("DONE: %d",(int)xyInput.response.done);
   while(1)
   {
   	if(xyInput.response.done ==1 && zInput.response.comm_result ==1)
   	{
   	  res.done = 1;
   	  ROS_INFO("done: %d",(int)res.done );
   	  break;
   	}
   }
   return 1;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "motor_controller");
  ros::NodeHandle nh;
  ros::ServiceServer service = nh.advertiseService("desired_xyz", desired_positionCallback);

  ros::ServiceClient client_tmp1 = nh.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("/dynamixel_workbench/dynamixel_command");
  ros::ServiceClient client_tmp2 = nh.serviceClient<docking::xyInput>("xy_srv");
 
  client1 = &client_tmp1;
  client2 = &client_tmp2;

  ROS_INFO("motor_control mode");
  ros::spin();

  return 0;
}














