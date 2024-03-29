#include "CommInt.h"

CommInt::CommInt(Message * (*fcnPtr)(int), ros::NodeHandle * nh)
{
	//TO DO: read module id from file
	_moduleId = 0;
	_ASId = 0; 

	Comm::GetInstance()->Init(fcnPtr, nh ,_ASId, friendlyName); 
	// Create singular Comm instance if it hasn't been 
	// created already
	Comm::GetInstance()->AddMsgQueue();
}

CommInt::~CommInt()
{

}

bool CommInt::CheckForMessage(int moduleId)
{
	return Comm::GetInstance()->CheckForMessage(moduleId);
}

int CommInt::SendMessage(std::string dest, Message * msg)
{
	// Set Framework Message Header parameters
	msg->SetId(_ASId);
	msg->SetSourceId(_ASId, _moduleId);

	if(dest == "All")
	{
		return Comm::GetInstance()->SendBd(_ASId, _moduleId, msg);
		std::cout << "Done sending Bd at CommInt" << std::endl; 
	}
	else
	{
		return Comm::GetInstance()->SendPtoP(_ASId, _moduleId, msg, dest, _moduleId);
		std::cout << "Done Sending PtP at CommInt" << std::endl; 
	}
	return 0;
}

Message * CommInt::GetMessage(int moduleId)
{
	return Comm::GetInstance()->GetMessage(moduleId);
}

int CommInt::GetNextMsgType(int moduleId)
{
	return Comm::GetInstance()->GetNextMsgType(moduleId); 
}
