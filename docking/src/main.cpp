#include "ros/ros.h"
#include "docking/ppp.h"
#include "docking/Init.h"
#include "docking/door.h"
#include "docking/docking.h"
#include "docking/desired_position.h"
#include "robot_msgs/omoapproach.h"
#include "robot_msgs/omoalign.h"

ros::ServiceClient * client_Door;
ros::ServiceClient * client_Init;
ros::ServiceClient * client_App;
ros::ServiceClient * client_Align;
ros::ServiceClient * client_Ppp;
ros::ServiceClient * client_Xyz;

bool docking_callback(docking::docking::Request &req, docking::docking::Response &res)
{
   
   docking::door door;
   docking::Init Init;
   robot_msgs::omoalign omoalign;
   robot_msgs::omoapproach omoapproach;
   omoapproach.request.z_goal = 0.25; 
   docking::ppp  ppp;
   docking::desired_position xyz;
   xyz.request.x=0;
   xyz.request.y=0;
   xyz.request.z=5;
   
   if(req.docking_start == 1)
   {
     door.request.door_start = 1;
     client_Door ->call(door);
    
    
     
    /* client_Align ->call(omoalign);
     
     while(ros::ok())
     {
       if(omoalign.response.is_aligned ==1) break;
     }*/
     client_App ->call(omoapproach);
      
     
     ppp.request.ppp_start == 1;
     client_Ppp ->call(ppp);
     ROS_INFO("ha!");
     
     double begin =ros::Time::now().toSec();
  
     while(ros::ok())
     {
        double done =ros::Time::now().toSec();
        ROS_INFO("charging,,, ");
   	if(done-begin > 15.0)
   	{
   	  
   	  client_Xyz ->call(xyz);
   	} 
   	break;
     }
      
     Init.request.Init_start =1;
     client_Init ->call(Init); 
   
     
    ROS_INFO("done");
     
    
     res.done = 1;
      
   }
   
   
   res.done = 1;
   return 1;
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "PPP");
  ros::NodeHandle nh;
  
 
  ros::ServiceServer service = nh.advertiseService("docking", docking_callback);
  
  ros::ServiceClient client_door = nh.serviceClient<docking::door>("door");
  client_Door = &client_door;
  
  ros::ServiceClient client_init = nh.serviceClient<docking::Init>("Init");
  client_Init = &client_init;
  
  ros::ServiceClient client_align = nh.serviceClient<robot_msgs::omoalign>("omo_con_srv");
  client_Align = &client_align;
  
  ros::ServiceClient client_app = nh.serviceClient<robot_msgs::omoapproach>("omo_approach");
  client_App = &client_app;
  
  ros::ServiceClient client_ppp = nh.serviceClient<docking::ppp>("ppp");
  client_Ppp = &client_ppp;
   
  ros::ServiceClient client_xyz = nh.serviceClient<docking::desired_position>("xyz");
  client_Xyz = &client_xyz;
 
  
  ROS_INFO("Ready to docking! just hit it!!!");
  
  ros::spin();

  return 0;
  }





