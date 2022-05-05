#include "ros/ros.h"
#include "docking/xyInput.h"
#include "docking/desired_position.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"
#define limit 40
int limit_sign = 0;
ros::ServiceClient* client1;
ros::ServiceClient* client2;

bool desired_positionCallback(docking::desired_position::Request &req, docking::desired_position::Response &res) //desired_position's type is mm and desired_position means relative postion
{
   
  float x = req.x; //relative position mm
  float y = req.y; //relative position mm
  float z = req.z; //absolute position mm
  res.done = 0;
  
  int x_limit = int(x)/limit;
  float x_remain = x-x_limit*limit;
  ROS_INFO("x_limit: %d",x_limit );
  ROS_INFO("x_remain: %f",x_remain );
  
  int y_limit = int(y)/limit;
  float y_remain = y-y_limit*limit;
  ROS_INFO("y_limit: %d",y_limit );
  ROS_INFO("y_remain: %f",y_remain );
  
  dynamixel_workbench_msgs::DynamixelCommand zInput;
  zInput.request.value = -(4096/360)*(360/100)*z; //resolutoin 4096  deg/mm = 360/100(wil be experienced)     
  zInput.request.id = 3;
  zInput.request.addr_name = "Goal_Position";
  zInput.response.comm_result = 0;
  client1->call(zInput);
  
  
  docking::xyInput xyInput;
  xyInput.request.y = 0;
  xyInput.response.done = 0;

 if(x_limit>0) limit_sign =1;
 else limit_sign  = -1;
 
 for(int i=0;i<abs(x_limit);i++)
  { 
    ROS_INFO("x_iteration: %d",i );
    ROS_INFO("x_sign: %d",limit_sign );
    xyInput.request.x = limit*limit_sign;
    client2->call(xyInput);
    
  }
 if(x_remain != 0)
  {
    xyInput.request.x = x_remain;
    client2->call(xyInput);
    ROS_INFO("x_remain " );
  }
    xyInput.request.x = 0;
    
 if(y_limit>0) limit_sign =1;
 else limit_sign  = -1;   
 
 for(int i=0;i<abs(y_limit);i++)
  {  
    ROS_INFO("y_iteration: %d",i );
    ROS_INFO("y_sign: %d",limit_sign );
    xyInput.request.y = limit*limit_sign;
    client2->call(xyInput);
    
  }
  if(y_remain != 0) 
  { 
     xyInput.request.y = y_remain;
     client2->call(xyInput);
     ROS_INFO("y_remain " );
  }
  
  
   
   	if(xyInput.response.done ==1 /*&& zInput.response.comm_result ==1*/)
   	{
   	  res.done = 1;
   	  ROS_INFO("done: %d",(int)res.done );
   	  
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

















