#include <ros/ros.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mavros/mavros.h>
#include <sensor_msgs/NavSatFix.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "gps_publisher");
  ros::NodeHandle nh;
  int num_uav = 10;
  int cur_uav = 0;
  float lat =30, lon = -118;
  ros::Rate loop_rate(10*num_uav); //10Hz per topic
  std::string uav_ns  = "UAV";
  ros::Publisher pub_pos_global[num_uav];
  sensor_msgs::NavSatFix position[num_uav];

  for(int i = 0; i < num_uav; i++){
    position[i].latitude  = lat;
    position[i].longitude = lon;

    pub_pos_global[i] = nh.advertise<sensor_msgs::NavSatFix>(uav_ns + std::to_string(i+1) + "/mavros/global_position/global",10);

    lat += .0005;
    lon += .0005;
  }

  while(ros::ok())
  {
    position[cur_uav].longitude += 0.00001;
    position[cur_uav].latitude += 0.00001;

    pub_pos_global[cur_uav].publish(position[cur_uav]);

    if(cur_uav == num_uav-1) cur_uav = 0;
    else cur_uav++;

    ros::spinOnce();
    loop_rate.sleep();
  }

}
