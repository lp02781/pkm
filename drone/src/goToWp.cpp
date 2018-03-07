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
	ros::init(argc, argv, "goToWp");
	ros::NodeHandle nh;

	ros::Publisher pub_drone_status 	= nh.advertise<std_msgs::String>("/pkm/drone/status", 16,true);
	ros::Subscriber sub_drone_status = nh.subscribe("/pkm/drone/status", 10, droneStatusCB);

	ROS_WARN_STREAM("node : waypoint ready");
	while (ros::ok() && drone_status.data != "goToWp.start")
	{
		ros::spinOnce();
	}
	sleep(3);
	
	//system("rosrun mavros mavsys mode -c LOITER"); //hold
	
	//system("rosrun mavros mavwp clear"); //clear wp

	ROS_INFO("goToWp : Waypoint clear");

	//system("rosrun mavros mavwp load ~/takeoff.waypoints");
	ROS_INFO("goToWp : Waypoint loaded");
	
	ROS_INFO("goToWp : drone mode GUIDED");
	
	//system("rosrun mavros mavsys mode -c GUIDED"); //take off and auto
	
	//system("rosrun mavros mavsys mode -c LOITER"); //hold
	
	ROS_INFO("goToWp : drone mode LOITER");
	
	sleep(3);
	
	drone_feedback.data = "goToWp.end";
	pub_drone_status.publish(drone_feedback);
	ros::shutdown();
}

void droneStatusCB(const std_msgs::String& msg)
{
	drone_status.data = msg.data;
}
