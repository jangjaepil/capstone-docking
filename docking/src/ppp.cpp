#include "ros/ros.h"
#include "docking/desired_position.h"
#include "docking/ppp.h"
#include "robot_msgs/mrkrPos.h"
ros::ServiceClient* client;
ros::ServiceClient* client2;

bool PPP_callback(docking::ppp::Request &req, docking::ppp::Response &res)
{
  docking::desired_position desired_xyz;
  robot_msgs::mrkrPos mark_pose;
  res.ppp_done = 0;
  
  
  
  if(req.ppp_start == 1)
  {
     while(1) //for xy
     {
        ROS_INFO("For xy");
        client ->call(mark_pose);
        desired_xyz.request.x = mark_pose.response.trn_x;
        desired_xyz.request.y = mark_pose.response.trn_y;
        desired_xyz.request.z = 0;
        
        
        if(abs(mark_pose.response.trn_x)<10 && abs(mark_pose.response.trn_y)<10)
        {
            break;
        }        
        if(abs(mark_pose.response.trn_x)<10 && abs(mark_pose.response.trn_y)>10)
        {
          desired_xyz.request.x = 0;
          client2->call(desired_xyz);
        }
        if(abs(mark_pose.response.trn_x)>10 && abs(mark_pose.response.trn_y)<10)
        {
          desired_xyz.request.y = 0;
          client2->call(desired_xyz);
        }
        if(abs(mark_pose.response.trn_x)>10 && abs(mark_pose.response.trn_y)>10)
        {
          
          client2->call(desired_xyz);
        }
        
        while(1)
        {
          if(desired_xyz.response.done ==1) break;
        }
        
        
        
     }
     while(1) // for z
     {   
          ROS_INFO("For z");
         client ->call(mark_pose);
         desired_xyz.request.x = 0;
         desired_xyz.request.y = 0;
         desired_xyz.request.z = mark_pose.response.trn_z;
         
         if(mark_pose.response.trn_z<10)
         {
           break;
         }
         else 
         {
            client2->call(desired_xyz);
         }
         
          while(1)
        {
          if(desired_xyz.response.done ==1) break;
        }
     }
  res.ppp_done = 1;  
  }
  else res.ppp_done = 1;
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "PPP");
  ros::NodeHandle nh;
  
 
  ros::ServiceServer service = nh.advertiseService("PPP", PPP_callback);
  ros::ServiceClient client_tmp = nh.serviceClient<robot_msgs::mrkrPos>("mark_Pose");
  client = &client_tmp;
  
  ros::ServiceClient client_tmp2 = nh.serviceClient<docking::desired_position>("desired_xyz");
  client2 = &client_tmp2;
 
  
  ROS_INFO("Ready to docking! just hit it!!!");
  

  
  
  
  ros::spin();

  return 0;
  }


