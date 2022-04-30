#include "ros/ros.h"
#include "docking/Init.h"
#include "docking/limit.h"
#include "docking/desired_position.h"

bool X_LIMIT_UP;
bool X_LIMIT_DOWN;
bool Y_LIMIT_LEFT;
bool Y_LIMIT_RIGHT;
ros::ServiceClient* client;

bool Init_callback(docking::Init::Request &req, docking::Init::Response &res)
{
  docking::desired_position desired_xyz;
  res.Init_done = 0;
  if(req.Init_start == 1){
    
    desired_xyz.request.x = 10;
    desired_xyz.request.y = 10;
    desired_xyz.request.z = 0;
    
    
    client->call(desired_xyz); 
    ROS_INFO("Initializing" );
     while(1)
   {
   	if(desired_xyz.response.done ==1)
   	{
   	  desired_xyz.request.x = -10;
          desired_xyz.request.y = -10;
          desired_xyz.request.z = 0;
   	 // ROS_INFO(": %d",(int)res.done );
   	  client->call(desired_xyz); 
   	  break;
   	}
   }
   
    while(1)
    {
        if(desired_xyz.response.done ==1)
        { 
          
          res.Init_done =1;
          ROS_INFO("Init done  %d ",(int)res.Init_done );
          break;
        }
    }
   
   
  }
  else res.Init_done = 0;
  
    return true;
}






int main(int argc, char **argv)
{
  ros::init(argc, argv, "Init");
  ros::NodeHandle nh;
  
 
  ros::ServiceServer service = nh.advertiseService("Init", Init_callback);
  ros::ServiceClient client_tmp = nh.serviceClient<docking::desired_position>("desired_xyz");
  client = &client_tmp;
 
  
  ROS_INFO("Ready to Init ppp for docking.");
  

  
  
  
  ros::spin();

  return 0;
}
