#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include "mavros_msgs/State.h"
#include <ros/ros.h>

void droneStatusCB						(const std_msgs::String& msg);

std_msgs::String drone_status;
std_msgs::String drone_feedback;

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
	 
	ros::init(argc, argv, "imageProcessing");
	ros::NodeHandle nh;

	ros::Publisher pub_drone_status 	= nh.advertise<std_msgs::String>("/pkm/drone/status", 16,true);
	ros::Subscriber sub_drone_status 	= nh.subscribe("/pkm/drone/status", 10, droneStatusCB);

	ROS_WARN_STREAM("node : image processing ready");
	
	while (ros::ok() && drone_status.data != "process.start")
	{
		ros::spinOnce();
	}
	sleep(3);
	
 ROS_INFO("process : load image picture.jpg");
 
 Mat Original = imread("../picture.jpg", CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'
 	
 if (Original.empty()) //check whether the image is loaded or not
 {
	cout << "Error : Image cannot be loaded..!!" << endl;
	//system("pause"); //wait for a key press
	return -1;
 }
 
 ROS_INFO("process : load image success");
 
 namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 int redLowH = 170;
 int redHighH = 179;
 
 int blueLowH = 75;
 int blueHighH = 130;
 
 int greenLowH = 38;
 int greenHighH = 75;

 int iLowS = 150; 
 int iHighS = 255;

 int iLowV = 60;
 int iHighV = 255;

 //Create trackbars in "Control" window
 createTrackbar("redLowH", "Control", &redLowH, 255);
 createTrackbar("redHighH", "Control", &redHighH, 255);
 
 createTrackbar("blueLowH", "Control", &blueLowH, 255); 
 createTrackbar("blueHighH", "Control", &blueHighH, 255);
 
 createTrackbar("greenLowH", "Control", &greenLowH, 255); 
 createTrackbar("greenHighH", "Control", &greenHighH, 255);

 createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

 createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);

  Mat imgHSV;
  
  ROS_INFO("process : convert original to HSV");
   
  cvtColor(Original, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
  
  erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //morphological closing (removes small holes from the foreground)
  dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  
  ROS_INFO("process : clear noise HSV");

  Mat red = imgHSV.clone();
  Mat blue = imgHSV.clone();
  Mat green = imgHSV.clone();
  
  Mat redThreshold;
  Mat blueThreshold;
  Mat greenThreshold;
  
  ROS_INFO("process : convert HSV to Threshold");

  inRange(red, Scalar(redLowH, iLowS, iLowV), Scalar(redHighH, iHighS, iHighV), redThreshold); //Threshold the image
  inRange(blue, Scalar(blueLowH, iLowS, iLowV), Scalar(blueHighH, iHighS, iHighV), blueThreshold); //Threshold the image
  inRange(red, Scalar(greenLowH, iLowS, iLowV), Scalar(greenHighH, iHighS, iHighV), greenThreshold); //Threshold the image
  
  ROS_INFO("process : get area RGB");
  
  //Calculate the moments of the thresholded image
  Moments redMoments = moments(redThreshold);
  Moments blueMoments = moments(blueThreshold);
  Moments greenMoments = moments(greenThreshold);

  double redArea = redMoments.m00;  
  double blueArea = blueMoments.m00; 
  double greenArea = greenMoments.m00;
  
  cout<< "red :"<<redArea<<"\t"<<"blue :"<<blueArea<<"\t"<<"green :"<<greenArea<<endl;
  sleep(3);
  
  drone_feedback.data = "process.end";
  pub_drone_status.publish(drone_feedback);
  ros::shutdown();
}

void droneStatusCB(const std_msgs::String& msg)
{
	drone_status.data = msg.data;
}
