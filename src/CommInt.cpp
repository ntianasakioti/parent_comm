#include "CommInt.h"

CommInt::CommInt(Message * (*fcnPtr)(int))
{
	//TO DO: read module id from file
	_moduleId = 0;
	_ASId = 1; 

	Comm::GetInstance()->Init(fcnPtr); 
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
	msg->SetId(_ASId, _ASId, -1);
	msg->SetSourceId(_ASId, _moduleId);

	if(dest == "All")
	{
		return Comm::GetInstance()->SendBd(msg);
		std::cout << "Done sending Bd at CommInt" << std::endl; 
	}
	else
	{
		return Comm::GetInstance()->SendPtoP(msg, dest);
		std::cout << "Done Sending PtP at CommInt" << std::endl; 
	}
	return 0;
}

Message * CommInt::GetMessage(int moduleId)
{
	return Comm::GetInstance()->GetMessage(moduleId);
}

