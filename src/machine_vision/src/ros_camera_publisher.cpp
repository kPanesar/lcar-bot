#include <ros/ros.h>
#include <ros/console.h>
#include <cv_bridge/cv_bridge.h>

#include <image_transport/image_transport.h>
#include "opencv2/highgui.hpp"
#include <sstream> // for converting the command line parameter to integer#
#include <stdlib.h>

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

    ros::init(argc, argv, "camera_publisher" /*, ros::init_options::AnonymousName*/ );
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);

    // get node specfic parameters from parameter server

    std::string topic;
    bool paramSet = ros::param::get("~topic", topic);

    if(!paramSet)
    {
        ROS_ERROR_STREAM("a topic must be specified!\n\t ex: rosrun machine_vision camera_publisher _topic:=/cam1 or rosrun machine_vision camera_publisher _topic:=/cam1 _video_id:=1");
        return 1;
    }
    image_transport::Publisher pub = it.advertise(topic, 1);
    ROS_INFO_STREAM("node: " << ros::this_node::getName()  
                      << " publishing on topic: " 
                      << pub.getTopic());

    int video_source;
    paramSet = ros::param::get("~video_id", video_source);
    if(!paramSet)
        video_source = -1;

    ROS_INFO_STREAM("video source: " << video_source);
    cv::VideoCapture cap(video_source);
    // Check if video device can be opened with the given index
    if(!cap.isOpened())
    {
        ROS_ERROR_STREAM("Error opening camera");
        return 1;
    }
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;

    int numFrames = 0;
    //don't change my color space! (bgr8)
    std::string enc = "bgr8"; // encoding
    std::cout << "broadcasting in " << enc << "\n";
    while (nh.ok()) 
    {
      try{
          cap >> frame;
          if(!frame.empty())
          {
              msg = cv_bridge::CvImage(std_msgs::Header(), enc, frame).toImageMsg();
              pub.publish(msg);
              numFrames++;
          }
          else 
              ROS_ERROR_STREAM("frame skipped: " << numFrames);
      }
      catch(cv::Exception e)
      {
          e.formatMessage();
          ROS_ERROR("%s", e.what());
      }
      ros::spinOnce();
    }
}
