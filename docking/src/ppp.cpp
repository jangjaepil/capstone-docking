#include "ros/ros.h"
#include "docking/desired_position.h"
#include "docking/ppp.h"
#include "robot_msgs/mrkrPos.h"
#include <fstream>
ros::ServiceClient* client;
ros::ServiceClient* client2;

float marker_x_d;
float marker_y_d;
float marker_z_d;
float tol_x;
float tol_y;
float tol_z;

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
        while(1)
        {
          ROS_INFO("Detecting marker");
         if(client ->call(mark_pose))
          {
           desired_xyz.request.x = mark_pose.response.trn_x*0.001; //m -> mm
           desired_xyz.request.y = mark_pose.response.trn_y*0.001;
           desired_xyz.request.z = 0;
           break;
          }        
        }
        
        if(abs(mark_pose.response.trn_x - marker_x_d)<tol_x && abs(mark_pose.response.trn_y - marker_y_d)<tol_y)
        {
            break;
        }        
        if(abs(mark_pose.response.trn_x - marker_x_d)<tol_x && abs(mark_pose.response.trn_y - marker_y_d)>tol_y)
        {
          desired_xyz.request.x = 0;
          client2->call(desired_xyz);
        }
        if(abs(mark_pose.response.trn_x - marker_x_d)>tol_x && abs(mark_pose.response.trn_y - marker_y_d)<tol_y)
        {
          desired_xyz.request.y = 0;
          client2->call(desired_xyz);
        }
        if(abs(mark_pose.response.trn_x - marker_x_d)>tol_x && abs(mark_pose.response.trn_y - marker_y_d)>tol_y)
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
         
         if(abs(mark_pose.response.trn_z-marker_z_d)<tol_z)
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


void Text_Input(void)
{
  int i = 0;
  std::size_t found;
  std::ifstream inFile;
  inFile.open("/home/jjp/catkin_ws/src/docking/config/docking_params.txt");
  if(inFile.is_open())   ROS_INFO("file open!");
  else ROS_INFO("NO");
  
  for(std::string line; std::getline(inFile,line);)
  {
      found=line.find("=");
      
      switch(i)
      {
      case 0: marker_x_d = atof(line.substr(found+2).c_str()); break;
      case 1: marker_y_d = atof(line.substr(found+2).c_str()); break;
      case 2: marker_z_d = atof(line.substr(found+2).c_str()); break;
      case 3: tol_x = atof(line.substr(found+2).c_str()); break;
      case 4: tol_y = atof(line.substr(found+2).c_str()); break;
      case 5: tol_z = atof(line.substr(found+2).c_str()); break;
      }
      i +=1;
  }
  inFile.close();
  ROS_INFO("marker_x_d:%f",marker_x_d);
  ROS_INFO("marker_y_d:%f",marker_x_d);
  ROS_INFO("marker_z_d:%f",marker_x_d);
  ROS_INFO("tol_x:%f",tol_x);
  ROS_INFO("tol_y:%f",tol_y);
  ROS_INFO("tol_z:%f",tol_z);
 
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
  
   Text_Input();
  
  
  
  
    
    ros::spin();
  
  return 0;
  }


