#ifndef BASECOMM_H
#define BASECOMM_H

#include <iostream>
#include <vector>
#include <mutex>
#include <fstream>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <queue>
#include "Message.h"
#include "Message1.h"
#include "Message2.h"

class BaseComm
{
public:
	//virtual ~BaseComm(){ }; 								/// saw on a forum that abstact classes should have a virtual destructor 
															/// so that derived classes can free up resources
	void Init(int systemId);
	virtual void Setup() = 0;  
	virtual int SendPtoP(int * dataBuffer, std::string dest) = 0;
	bool CheckForMessage(int moduleId);
	Message * GetMessage(int moduleId);
	int GetNextMsgType(int moduleId);
	void UpdateMessageLog(int * dataBuffer, int moduleId); 
	void UpdateMsgLogNum();
	void MutexLock();
	void MutexUnlock();
	void setMsgFcnPtr(Message * (*fcnPtr)(int));
	

	Message * getMsgFromId(int id);
	
	int GetMessageLogSize(int moduleId);
	int GetId(std::string name);
	int GetMyId(); 

protected:
	std::map<std::string, int> nameIDs;

private:
	int ASid; 
	static std::vector<std::queue<Message*>> messageBacklog;
	static std::mutex messageMutex; 
	static Message * (*_fcnPtr)(int);
};

#endif