#include "BaseComm.h"

Message * BaseComm::getMsgFromId(int id)
{
	Message1 * msg1; 
	Message2 * msg2;
	switch(id)
	{
		case 0:
			msg1 = new Message1(); 
			return msg1;
			break;
		case 1:
			msg2 = new Message2(); 
			return msg2;
			break;
	}
}

std::vector<std::vector<Message*>> BaseComm::messageBacklog; 
Message * (*BaseComm::_fcnPtr)(int);
std::mutex BaseComm::messageMutex;

void BaseComm::Init()
{
	std::ifstream inf("../catkin_ws/src/parent_comm/config/nameIDs.txt"); 

	int id;
	std::string friendlyName; 
	int numAs;
	inf >> numAs; 
	//std::cout << "Basecomm init " << std::endl; 
	while((inf>> id).good() && (inf>> friendlyName).good())
	{
	//	inf >> id; inf >> friendlyName;
		std::cout << id << " " << friendlyName << std::endl << std::flush; 
		nameIDs.insert(std::make_pair(friendlyName,id));
	}
	inf.close(); 
}
void BaseComm::setMsgFcnPtr(Message * (*fcnPtr)(int))
{
	_fcnPtr = fcnPtr; 
}
bool BaseComm::CheckForMessage(int moduleId)
{
	MutexLock();
	if(messageBacklog[moduleId].size() > 0)
	{
		MutexUnlock();
		return true;
	}
	else
	{
		MutexUnlock();  
		return false;
	}
}

Message * BaseComm::GetMessage(int moduleId)
{
	MutexLock();
	Message * newMsg = getMsgFromId(messageBacklog[moduleId].at(0)->GetType());


	newMsg = (Message *) malloc(sizeof(messageBacklog[moduleId].at(0)));
	std::memcpy(newMsg, messageBacklog[0].at(0), sizeof(messageBacklog[moduleId].at(0)));
	free(messageBacklog[moduleId].at(0));
	messageBacklog[moduleId].erase(messageBacklog[moduleId].begin());
	MutexUnlock();

	return newMsg;
}

void BaseComm::UpdateMessageLog(int  * dataBuffer, int moduleId)
{
	std::cout << "Pushing back a message to module " << moduleId << std::endl; 
	Message * newMsg = getMsgFromId(dataBuffer[3]);
	
	newMsg->SetHeaderAttr(std::make_pair(dataBuffer[0], dataBuffer[1]),dataBuffer[2], dataBuffer[3], dataBuffer[4], 
	std::make_pair(dataBuffer[5], dataBuffer[6]), std::make_pair(dataBuffer[7], dataBuffer[8]));
	newMsg->DeSerialize(dataBuffer); 
	messageBacklog[moduleId].push_back(newMsg);
}

void BaseComm::UpdateMsgLogNum()
{
	
	// Create and add anothe vector to MessageBacklog
	std::vector<Message *> anotherMsgLog; 
	messageBacklog.push_back(anotherMsgLog);
}

void BaseComm::MutexLock()
{
	messageMutex.lock();
}

void BaseComm::MutexUnlock()
{
	messageMutex.unlock();
}

int BaseComm::GetMessageLogSize(int moduleId)
{
	return messageBacklog[moduleId].size(); 
}

int BaseComm::GetId(std::string name)
{
	//std::cout << "Name " << name << std::endl; 

	std::map<std::string,int>::iterator it = nameIDs.begin();

	for(int i = 0 ; i < nameIDs.size(); i++)
	{
		//std::cout << it->first << std::endl; 
		if(it->first == name)
		{
			return it->second;
		}
		it++;
	}

	return -1; 
}

int BaseComm::GetMyId()
{
	return ASid;
}
