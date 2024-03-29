#include "BaseComm.h"

std::vector<std::queue<Message*>> BaseComm::messageBacklog; 
Message * (*BaseComm::_fcnPtr)(int);
std::mutex BaseComm::messageMutex;

void BaseComm::Init(int systemId)
{
	//ASid = systemId;
	ASid = systemId; 

	std::ifstream inf("../catkin_ws/src/parent_comm/config/nameIDs.txt"); 

	int id;
	std::string friendlyName; 
	int numAs;
	inf >> numAs; 
	int counter =0;
	//std::cout << "Basecomm init " << std::endl; 
	//while((inf>> id).good() && (inf>> friendlyName).good())
	while(counter < numAs)
	{	
		inf >> id; inf >> friendlyName;
		std::cout << id << " " << friendlyName << std::endl << std::flush; 
		nameIDs.insert(std::make_pair(friendlyName,id));
		counter++;
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

	MutexUnlock();  
	return false;
}

Message * BaseComm::GetMessage(int moduleId)
{
	MutexLock();

	Message * newMsg = messageBacklog[moduleId].front()->Clone();
	messageBacklog[moduleId].pop();
	MutexUnlock();

	return newMsg;
}

int BaseComm::GetNextMsgType(int moduleId)
{
	return messageBacklog[moduleId].front()->GetMsgType();
}

void BaseComm::UpdateMessageLog(int  * dataBuffer, int moduleId)
{
	//std::cout << "Pushing back a message to module " << moduleId << " with message id " << dataBuffer[3] << std::endl; 
	Message * newMsg = _fcnPtr(dataBuffer[3]);
	//std::cout << "id " << dataBuffer[0] << " " << dataBuffer[1] << std::endl; 
	newMsg->SetHeaderAttr(std::make_pair(dataBuffer[0], dataBuffer[1]),dataBuffer[2], dataBuffer[3], dataBuffer[4], 
	std::make_pair(dataBuffer[5], dataBuffer[6]), std::make_pair(dataBuffer[7], dataBuffer[8]));
	newMsg->DeSerialize(dataBuffer); 
	MutexLock();
//	std::cout << "locked in update message log" << std::endl; 
	messageBacklog[moduleId].push(newMsg);
	MutexUnlock(); 
//	std::cout << "unlocked in update message log " << std::endl;
//	std::cout << "check for message reuslt " << CheckForMessage(0) << std::endl; 
}

void BaseComm::UpdateMsgLogNum()
{
	
	// Create and add anothe vector to MessageBacklog
	std::queue<Message *> anotherMsgLog; 
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

	std::map<std::string,int>::iterator it = nameIDs.find(name);

	//for(int i = 0 ; i < nameIDs.size(); i++)
	//{
	//	//std::cout << it->first << std::endl; 
	//	if(it->first == name)
	//	{
			return it->second;
	//	}
	//	it++;
	//}

	return -1; 
}

int BaseComm::GetMyId()
{
	return ASid;
}
