//  ///////////////////////////////////////////////////////////
//
// turtlebot_example_node.cpp
// This file contains example code for use with ME 597 lab 1
// It outlines the basic setup of a ros node and the various 
// inputs and outputs.
// 
// Author: James Servos. 2012 
//
// //////////////////////////////////////////////////////////

#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_datatypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

double X;
double Y;
double Yaw;


//Callback function for the Position topic 
void pose_callback(const geometry_msgs::PoseWithCovarianceStamped& msg)
{
	//This function is called when a new pose message is received

	 X = msg.pose.pose.position.x; // Robot X psotition
	 Y = msg.pose.pose.position.y; // Robot Y psotition
	 Yaw = tf::getYaw(msg.pose.pose.orientation); // Robot Yaw
	ROS_INFO("pose_callback X: %f Y: %f Yaw: %f", X, Y, Yaw);
}



int main(int argc, char **argv)
{
	//Initialize the ROS framework
	ros::init(argc,argv,"main_control");
	ros::NodeHandle n;

	//Subscribe to the desired topics and assign callbacks
	ros::Subscriber pose_sub = n.subscribe("/amcl_pose", 1, pose_callback);

	//Setup topics to Publish from this node
	ros::Publisher velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1);
    
	//Velocity control variable
	geometry_msgs::Twist vel;

	//Set the loop rate
	ros::Rate loop_rate(20);    //20Hz update rate

	//set the variables
	double x_init = 0;
	double y_init = 0;
	double theta_target;
	double distance = 0;
	int counter = 0;
	bool init_pos_flag = 0;
	while (ros::ok())
	{
		loop_rate.sleep(); //Maintain the loop rate
		ros::spinOnce();   //Check for new messages
		//Main loop code goes here:
		if (counter == 0){
			vel.linear.x = 0.2; // set linear speed	
			vel.linear.y = 0; // set linear speed	
			vel.angular.z = 0; // set angular speed

		}else if(counter == 1){
			vel.linear.x = 0; // set linear speed	
			vel.linear.y = 0; // set linear speed	
			vel.angular.z = 0; // set angular speed
		}else if(counter == 2){
			vel.linear.x = 0; // set linear speed	
			vel.linear.y = 0; // set linear speed	
			vel.angular.z = 0; // set angular speed
			
		}else if(counter == 3){
			vel.linear.x = 0; // set linear speed	
			vel.linear.y = 0; // set linear speed	
			vel.angular.z = 0; // set angular speed
		}else{
			vel.linear.x = 0; // set linear speed	
			vel.linear.y = 0; // set linear speed	
			vel.angular.z = 0; // set angular speed
		}

		if (counter < 1){
	
			
			if (!init_pos_flag == 1 ){http://stackoverflow.com/questions/13213917/turtlebot-ros-moving-using-twist
				x_init = X;
				y_init = Y;
				init_pos_flag = 1;
			}
				
			
			
	
			if ( distance < 0.2)
			{
				//distance = sqrt(pow((x_init - X),2) + pow((y_init - Y),2));
				distance = sqrt( (x_init - X) * (x_init - X) + (y_init - Y) *(y_init - Y) );				 
			}else{

				counter ++;
				distance = 0;
				vel.linear.x = 0; // set linear speed
				vel.linear.y = 0; // set linear speed
				vel.angular.z = 0; // set angular speed
				init_pos_flag = 0;	
			}

			

			

			

		}

		velocity_publisher.publish(vel); // Publish the command velocity
		ROS_INFO("Main - Velocity commands: v - %f, w - %f, dist - %f, x_init - %f, y_init - %f, X - %f, Y - %f", vel.linear.x, vel.angular.z, distance, x_init, y_init, X, Y);
 
	}

	return 0;
}