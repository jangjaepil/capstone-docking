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
int limit = 80;

bool PPP_callback(docking::ppp::Request &req, docking::ppp::Response &res)
{
  docking::desired_position desired_xyz;
  robot_msgs::mrkrPos mark_pose;
  res.ppp_done = 0;


  if(req.ppp_start == 1)
  {
     while(ros::ok()) //for xy
     {
        ROS_INFO("For xy");
        while(1)
        {
          ROS_INFO("Detecting marker");
         if(client ->call(mark_pose)&& mark_pose.response.is_pos_return)
          {
           desired_xyz.request.x = mark_pose.response.trn_x*1000+marker_x_d; //m -> mm
           desired_xyz.request.y = mark_pose.response.trn_y*1000+marker_y_d;
           desired_xyz.request.z = 0;
           ROS_INFO("g");
           
           break;
          }        
        }
        
        if(abs(desired_xyz.request.x)<=tol_x && abs(desired_xyz.request.y)<=tol_y)
        {
            ROS_INFO("good");
            break;
        }        
        if(abs(desired_xyz.request.x)<=tol_x && abs(desired_xyz.request.y)>tol_y)
        {
          ROS_INFO("moving y");
          desired_xyz.request.x = 0;
          client2->call(desired_xyz);
        }
        if(abs(desired_xyz.request.x)>tol_x && abs(desired_xyz.request.y)<=tol_y)
        {
          ROS_INFO("moving x");
          desired_xyz.request.y = 0;
          client2->call(desired_xyz);
        }
        if(abs(desired_xyz.request.x)>tol_x && abs(desired_xyz.request.y)>tol_y)
        {
          ROS_INFO("moving x and y");
          client2->call(desired_xyz);
        }
        
        while(1)
        {
          if(desired_xyz.response.done ==1) break;
        }
        
        
     } 
     float current_length = 0;
     while(1) // for z
     
     {   
          ROS_INFO("For z");
         client ->call(mark_pose);
         desired_xyz.request.x = 0;
         desired_xyz.request.y = 0;
         desired_xyz.request.z = mark_pose.response.trn_z*1000+marker_z_d;
         
         if(abs(desired_xyz.request.z)<tol_z)
         {
           ROS_INFO("good");
           break;
         }

         if(abs(desired_xyz.request.z)>tol_z && current_length+desired_xyz.request.z<limit)
         {
                  
                 current_length = current_length + desired_xyz.request.z;
                 desired_xyz.request.z = current_length;
                 client2->call(desired_xyz);
         }
         
         
         if(abs(desired_xyz.request.z)>tol_z && current_length+desired_xyz.request.z>limit)

         { 
                  desired_xyz.request.z = limit;
                  current_length=limit;
                  client2->call(desired_xyz);
                  break;

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
  inFile.open("/home/volta/catkin_ws/src/capstone-docking/docking/config/docking_params.txt");
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
  ros::ServiceClient client_tmp = nh.serviceClient<robot_msgs::mrkrPos>("marker_pose_srv");
  client = &client_tmp;
  
  ros::ServiceClient client_tmp2 = nh.serviceClient<docking::desired_position>("desired_xyz");
  client2 = &client_tmp2;
 
  
  ROS_INFO("Ready to docking! just hit it!!!");
  
   Text_Input();
  
  
  
  
    
    ros::spin();
  
  return 0;
  }


