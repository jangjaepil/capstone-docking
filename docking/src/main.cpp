#include "ros/ros.h"
#include "docking/ppp.h"
#include "docking/Init.h"
#include "docking/door.h"
#include "docking/docking.h"
#include "docking/desired_xyz.h"

ros::ServiceClient * client_Door;
ros::ServiceClient * client_Iniit;
/*ros::ServiceClient * client_;*/
ros::ServiceClient * client_Ppp;
ros::ServiceClient * client_Xyz;

bool docking_callback(docking::docking::Request &req, docking::docking::Response &res)
{
   
   docking::door door;
   docking::Init Init;
   //
   docking::ppp  ppp;
   docking::desired_xyz xyz;
   xyz.x=0;
   xyz.y=0;
   xyz.z=0;
   
   if(req.docking_start == 1)
   {
     door.door_start = 1;
     client_Door ->call(door);
     
     Init.Init_start ==1;
     client_Init ->call(Init);
     
     //
     
     
     ppp.ppp_start == 1;
     client_Init ->call(ppp);
     
     
     //wait a seconds
     
     client_Xyz ->call(xyz);
     
     door.door_start = 0;
     client_Door ->call(door);
     
     res.done = 1;
      
   }
   
   
   res.done = 1;
   return 1
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "PPP");
  ros::NodeHandle nh;
  
 
  ros::ServiceServer service = nh.advertiseService("docking", docking_callback);
  
  ros::ServiceClient client_door = nh.serviceClient<docking::door>("door");
  client_Door = &client_door;
  
  ros::ServiceClient client_init = nh.serviceClient<docking::Init>("Init");
  client_Init = &client_Init;
  
  /*ros::ServiceClient client_tmp2 = nh.serviceClient<docking::ppp>("ppp");
  client2 = &client_tmp2; */
  
  ros::ServiceClient client_ppp = nh.serviceClient<docking::docking>("ppp");
  client_Ppp = &client_ppp;
   
  ros::ServiceClient client_xyz = nh.serviceClient<docking::desired_xyz>("xyz");
  client_Xyz = &client_xyz;
 
  
  ROS_INFO("Ready to docking! just hit it!!!");
  
  ros::spin();

  return 0;
  }





