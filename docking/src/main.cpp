#include "ros/ros.h"
#include "docking/ppp.h"
#include "docking/Init.h"
#include "docking/door.h"
#include "docking/docking.h"
#include "docking/desired_position.h"

ros::ServiceClient * client_Door;
ros::ServiceClient * client_Init;
/*ros::ServiceClient * client_;*/
ros::ServiceClient * client_Ppp;
ros::ServiceClient * client_Xyz;

bool docking_callback(docking::docking::Request &req, docking::docking::Response &res)
{
   
   docking::door door;
   docking::Init Init;
   //
   docking::ppp  ppp;
   docking::desired_position xyz;
   xyz.request.x=0;
   xyz.request.y=0;
   xyz.request.z=0;
   
   if(req.docking_start == 1)
   {
     door.request.door_start = 1;
     client_Door ->call(door);
     
     Init.request.Init_start ==1;
     client_Init ->call(Init);
     
     //
     
     
     ppp.request.ppp_start == 1;
     client_Init ->call(ppp);
     
     
     //wait a seconds
     
     client_Xyz ->call(xyz);
     
     door.request.door_start = 0;
     client_Door ->call(door);
     
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
  
  /*ros::ServiceClient client_tmp2 = nh.serviceClient<docking::ppp>("ppp");
  client2 = &client_tmp2; */
  
  ros::ServiceClient client_ppp = nh.serviceClient<docking::ppp>("ppp");
  client_Ppp = &client_ppp;
   
  ros::ServiceClient client_xyz = nh.serviceClient<docking::desired_position>("xyz");
  client_Xyz = &client_xyz;
 
  
  ROS_INFO("Ready to docking! just hit it!!!");
  
  ros::spin();

  return 0;
  }





