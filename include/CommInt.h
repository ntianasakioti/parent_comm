#ifndef COMMINT_H
#define COMMINT_H

#include "Comm.h"

class CommInt
{
public:
	CommInt(Message * (*fcnPtr)(int));
	~CommInt();
	bool CheckForMessage(int moduleId);
	int SendMessage(std::string dest, Message * msg);
	Message * GetMessage(int moduleId);
protected:

private:
	int _moduleId; 
	int _ASId; 
	//static int counter = 0;
}; 

#endif