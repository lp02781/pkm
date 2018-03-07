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
	ros::init(argc, argv, "takeOff");
	ros::NodeHandle nh;

	ros::Publisher pub_drone_status 	= nh.advertise<std_msgs::String>("/pkm/drone/status", 16,true);
	ros::Subscriber sub_drone_status = nh.subscribe("/pkm/drone/status", 10, droneStatusCB);

	ROS_WARN_STREAM("node : take-off ready ");
	while (ros::ok() && drone_status.data != "takeoff.start")
	{
		ros::spinOnce();
	}
	sleep(3);
	
	ROS_INFO("take off : waiting load waypoints");
	
	//system("rosrun mavros mavwp clear"); //clear wp

	ROS_INFO("take off : Waypoint clear");

	//system("rosrun mavros mavwp load ~/takeoff.waypoints");
	
	ROS_INFO("take off : Waypoint loaded");
	
	ROS_INFO("take off : drone take-off");
	
	//system("rosrun mavros mavsys mode -c GUIDED"); //take off and auto
	
	//system("rosrun mavros mavsys mode -c LOITER"); //hold
	
	ROS_INFO("take off : drone mode LOITER");
	sleep(3);
	
	drone_feedback.data = "takeoff.end";
	pub_drone_status.publish(drone_feedback);
	ros::shutdown();
}

void droneStatusCB(const std_msgs::String& msg)
{
	drone_status.data = msg.data;
}
