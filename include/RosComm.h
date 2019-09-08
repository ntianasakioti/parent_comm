#ifndef ROSCOMM_H
#define ROSCOMM_H

#include "BaseComm.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32MultiArray.h"


class RosComm: public BaseComm
{
public:
	RosComm(char * myCommRow, ros::NodeHandle * nh);
	~RosComm(); 
	void Setup();
	//void InitComm(int argc, char ** argv);
	void ListeningThread(); 
	int SendPtoP(int *  dataBuffer, std::string dest);
	//int SendBd(int * dataBuffer);
	void messageCallback(const std_msgs::Int32MultiArray::ConstPtr& array);
	

protected:

private:
	ros::NodeHandle * _nh; 
	std::thread ltThread;												// listening thread
	char * _myCommRow; 
	std::map<int, ros::Publisher *> _publishers;
	ros::Subscriber subPtoP;
};

#endif
