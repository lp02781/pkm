#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "mavros_msgs/State.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>

using namespace cv;
using namespace std;

void droneStatusCB						(const std_msgs::String& msg);

std_msgs::String drone_status;
std_msgs::String drone_feedback;

int main(int argc, char **argv)
{	
	ros::init(argc, argv, "imageCapture");
	ros::NodeHandle nh;

	ros::Publisher pub_drone_status 	= nh.advertise<std_msgs::String>("/pkm/drone/status", 16,true);
	ros::Subscriber sub_drone_status 	= nh.subscribe("/pkm/drone/status", 10, droneStatusCB);

	ROS_WARN_STREAM("node : image capture ready");
	while (ros::ok() && drone_status.data != "image.start")
	{
		ros::spinOnce();
	}
	sleep(3);
	//system("rosrun mavros mavsys mode -c LOITER"); //hold
	
	ROS_INFO("image : create Mat Original");
	
	Mat Original;
	VideoCapture cap(0);
	if ( !cap.isOpened() ) 
    {
         cout << "webcam tidak terbaca" << endl;
         return -1;
    }
    
    ROS_INFO("image : webcam active");
	
	cap.read(Original);
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    
    vector<int> compression_params; //vector that stores the compression parameters of the image
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); //specify the compression technique
    compression_params.push_back(98); //specify the compression quality
    bool cSuccess = imwrite("../picture.jpg", Original, compression_params); //write the image to file
    
    ROS_INFO("image : get picture");
    
    //system("rosrun mavros mavsys mode -c LOITER"); //hold
    sleep(3);
    drone_feedback.data = "image.end";
	pub_drone_status.publish(drone_feedback);
	ros::shutdown();
}

void droneStatusCB(const std_msgs::String& msg)
{
	drone_status.data = msg.data;
}
