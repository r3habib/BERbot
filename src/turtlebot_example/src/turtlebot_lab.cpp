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

double pi = 3.141592654;
double X;
double Y;
double Yaw;
double Yaw_orig;

//Callback function for the Position topic 
void pose_callback(const geometry_msgs::PoseWithCovarianceStamped& msg)
{
	//This function is called when a new pose message is received

	 X = msg.pose.pose.position.x; // Robot X psotition
	 Y = msg.pose.pose.position.y; // Robot Y psotition
	 Yaw_orig = tf::getYaw(msg.pose.pose.orientation); // Robot Yaw
	 if (Yaw_orig <0) 
		Yaw = 2*pi + Yaw_orig;
	else Yaw = Yaw_orig;
			
					

	//ROS_INFO("pose_callback X: %f Y: %f Yaw: %f", X, Y, Yaw);
}



int main(int argc, char **argv)
{
	//Initialize the ROS framework
	ros::init(argc,argv,"main_control");
	ros::NodeHandle n;

	//Subscribe to the desired topics and assign callbacks)
	ros::Subscriber pose_sub = n.subscribe("/amcl_pose", 1, pose_callback);

	//Setup topics to Publish from this node
	ros::Publisher velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1);
    
	//Velocity control variable
	geometry_msgs::Twist vel;

	//Set the loop rate
	ros::Rate loop_rate(5);    //20Hz update rate

	//set the variables
	double x_init = 0;
	double y_init = 0;
	double initial_theta;
	double distance = 0.0;
	int counter = 0;
	bool go_straight = 1;
	bool init = 1;
	while (ros::ok())
	{
		
		loop_rate.sleep(); //Maintain the loop rate
		ros::spinOnce();   //Check for new messages
		
		//Main loop code goes here:

		if (counter < 4){
			
			//ros::spinOnce();   //Check for new messages
			//Main loop code goes here:
			
			if (init == 1)
			{
				x_init = X;
				y_init = Y;
				initial_theta = Yaw;
				//if (initial_theta <0 ) 
				//	initial_theta = 2*pi + initial_theta;
				
				init = 0;
//ROS_INFO("Main - Velocity commands: v - %f, w - %f, dist - %f, x_init - %f, y_init - %f, X - %f, Y - %f, yaw - %f", vel.linear.x, vel.angular.z, distance, x_init, y_init, X, Y, Yaw);

			}
			else
			{
				if (go_straight == 1){
		
				
					
					if ( distance < 0.3)
					{
						vel.linear.x = 0.8; // set linear speed	
						vel.linear.y = 0; // set linear speed	
						vel.angular.z = 0; // set angular speed
						distance = sqrt( (x_init - X) * (x_init - X) + (y_init - Y) *(y_init - Y) );				 
					}
					else
					{			
						distance = 0;
						vel.linear.x = 0; // set linear speed
						vel.linear.y = 0; // set linear speed
						vel.angular.z = 0; // set angular speed

						go_straight = 0;	
					}
				}

				else {
					
					if ((initial_theta>= (3*pi/2) ) && (Yaw >= 0 && Yaw < pi))
					{
						Yaw = Yaw + 2*pi;
							
					}	
					
					if (fabs(Yaw -initial_theta) >= (pi/2) )
					{
						vel.angular.z = 0;
						go_straight = 1;
						counter ++;
						init = 1;
						//ROS_INFO("Initial Theta = %f", initial_theta*180/pi);
					//ROS_INFO("Yaw Theta = %f", Yaw*180/pi);
					//ROS_INFO("diff Theta = %f", fabs(Yaw - initial_theta) *180/pi);

					//ROS_INFO("break");

					}
					else vel.angular.z = 0.4;
					
				}
			}
			//ros::spinOnce();   //Check for new messages
			
		/*	if (go_straight == 1){ 
				vel.angular.z = 0.2;
		/*	if (go_straight == 1){ 
				vel.angular.z = 0.2;
				if (theta_target > 0 && theta_target <= pi/2) //first Q
				{

					if ( abs(Yaw - theta_target) > pi/2){
						vel.angular.z = 0;
						go_straight = 0;
						init_pos_flag = 0;
						counter ++;
					}
				}
				else if (theta_target > pi/2 && theta_target <= pi) //Second Q
				{

					if (2*pi - abs(Yaw - theta_target) > pi/2){
						vel.angular.z = 0;
						go_straight = 0;
						init_pos_flag = 0;
						counter ++;
					}
				}
				else if (theta_target >= -pi && theta_target < -pi/2) //Third Q
				{

					if (abs(Yaw - theta_target) > pi/2){
						vel.angular.z = 0;
						go_straight = 0;
						init_pos_flag = 0;
						counter ++;
					}
				}

				else if (theta_target >= -pi/2 && theta_target <= 0)//Fourth Q
				{

					if ( abs(Yaw - theta_target) > pi/2){
						vel.angular.z = 0;
						go_straight = 0;
						init_pos_flag = 0;
						counter ++;
					}
				}
				
			}*/

		velocity_publisher.publish(vel); // Publish the command velocity
		


		}
		
		
 
	}

	return 0;
}
