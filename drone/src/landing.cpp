#include <iostream>
#include <stdio.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include "mavros_msgs/State.h"
#include <ros/ros.h>


void droneStatusCB						(const std_msgs::String& msg);

using namespace std;


std_msgs::String drone_status;
std_msgs::String drone_feedback;

int main( int argc, char** argv )
{
	ros::init(argc, argv, "landing");
	ros::NodeHandle nh;

	ros::Publisher pub_drone_status 	= nh.advertise<std_msgs::String>("/pkm/drone/status", 16,true);
	ros::Subscriber sub_drone_status 	= nh.subscribe("/pkm/drone/status", 10, droneStatusCB);

	ROS_WARN_STREAM("node : landing ready");
	while (ros::ok() && drone_status.data != "landing.start")
	{
		ros::spinOnce();
	}
	sleep(3);
	//system("rosrun mavros mavsys mode -c LOITER");
	
	ROS_INFO("landing : Drone landing");
	
	//system("rosrun mavros mavwp clear"); //clear wp

	ROS_INFO("landing : Waypoint clear");
	
	ROS_INFO("landing : change mode to RTL");
	
	//system("rosrun mavros mavsys mode -c RTL"); //take off and auto
	
	ROS_INFO("landing : drone landing succesfully");
	sleep(3);
	drone_feedback.data = "landing.end";
	pub_drone_status.publish(drone_feedback);
	ros::shutdown();
}

void droneStatusCB(const std_msgs::String& msg)
{
	drone_status.data = msg.data;
}
