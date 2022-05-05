#include "ros/ros.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"
#include "docking/door.h"

ros::ServiceClient* client1;


bool door_Callback(docking::door::Request &req, docking::door::Response &res)
{

  res.door_done = 0;
  dynamixel_workbench_msgs::DynamixelCommand door_Input;
    
  door_Input.request.id = 12;
  door_Input.request.addr_name = "Goal_Velocity";
  door_Input.response.comm_result = 0;
  door_Input.request.value = 0; 
  
  if(req.door_start ==1)
  {
     door_Input.request.value = -15; //resolutoin 4096 open the door     
  }
  else if(req.door_start ==0)
  {
      door_Input.request.value = 15; // close the door
  }
   
  client1->call(door_Input);
  double begin =ros::Time::now().toSec();
  
   while(1)
   {
        double done =ros::Time::now().toSec();
   	if(done-begin > 5.0)
   	{
   	  door_Input.request.id = 12;
          door_Input.request.addr_name = "Goal_Velocity";
          door_Input.request.value = 0; 
   	  client1->call(door_Input);
   	  res.door_done = 1;
   	  
   	  
   	  break;
   	}
   }
   return 1;
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "door");
  ros::NodeHandle nh;
  ros::ServiceServer service = nh.advertiseService("door", door_Callback);

  ros::ServiceClient client_tmp1 = nh.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("/dynamixel_workbench/dynamixel_command");
  
 
  client1 = &client_tmp1;
 

  ROS_INFO("door ready!");
  ros::spin();

  return 0;
}

