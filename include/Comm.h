#ifndef COMM_H
#define COMM_H

#include <string>
#include <vector>
#include <map>
#include <condition_variable>
#include <iostream>
#include <map>
#include <fstream>
#include "Message.h"
#include "BlueComm.h"
#include "RosComm.h"
#include "ros/ros.h"

class Comm
{
public:
	static Comm * GetInstance();
	bool checkCommInstance();
	void Init(Message * (*fcnPtr)(int), ros::NodeHandle * nh, int systemId, std::string ASname);														
	int SendPtoP(int source, int sourceMod,Message * msg, std::string dest, int moduleId);																					
	int SendBd(int source, int sourceMod, Message * msg);					
	// instead of above two, could have one function
	//Send(source?,message, destination(could be "all"))
	bool CheckForMessage(int moduleId);
	Message * GetMessage(int moduleId);
	int GetNextMsgType(int moduleId);
	void AddMsgQueue();
	BaseComm * getPtr(char type); 
	int GetId(std::string name);

protected:
	Comm();
	~Comm();
	static Comm * instance;

private:
	bool commInstance = false; 
	std::string systemName;												// AS friendly name 
	const char* commTableFile = 
	"../catkin_ws/src/parent_comm/config/commTable.txt";				// Configuration file for the shared Comm 
																		// table that lists what communication is
																		// used for the Autonomous systems
	const char* nameIDsFile = 											// Configuration file for translating the
	"../catkin_ws/src/parent_comm/config/nameIDs.txt";					// AS friendly names to IDs
	char ** commTable; 
	std::map<std::string,int> nameIdMap; 		
	std::map<char,BaseComm *> BaseCommPtrs;		
};

#endif
