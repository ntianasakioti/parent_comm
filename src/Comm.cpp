#include "Comm.h"
#include <fstream>

Comm* Comm::instance = NULL;

Comm::Comm()
{
	//terminate = false; 
}

Comm::~Comm()
{
	std::cout << "Terminating " << std::endl << std::flush; 
	delete commTable;
	delete commTableFile; 
	delete nameIDsFile; 

	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();

	while(it != BaseCommPtrs.end())
	{
		delete it->second;
	}
}

Comm * Comm::GetInstance()
{
	// Initialization on first use
	if( instance == NULL)
	{
		instance = new Comm();
	}
	return instance; 
}

bool Comm::checkCommInstance()
{
	return commInstance; 
}

void Comm::Init(Message * (*fcnPtr)(int), ros::NodeHandle * nh, int systemId, std::string ASname)
{

	commInstance = true; 
	std::ifstream inf("../catkin_ws/src/parent_comm/config/nameIDs.txt"); 

	// if can't open file
	if(!inf.is_open())
	{
		std::cerr << "Uh oh, file could not be opened for reading! " << std::endl;
		exit(1);
	}

	systemName = ASname; 
//	std::cout << "Opened nameIDs correctly" << std::endl;

	// read number of autonomous systems to create comm table 
	int numAS = 0;
	inf >> numAS; 
	//std::cout << "number of systems " << numAS << std::endl; 
	// populate container that maps friendly names to ID used by the framework
	int id;
	std::string friendlyName; 
	int counter = 0; 
	//while((inf>> id).good() && (inf>> friendlyName).good())
	while(counter < numAS)
	{
		std::cout << "Am I stuck here " << std::endl;
		inf >> id; inf >> friendlyName;
		//std::cout << id << " " << friendlyName << std::endl << std::flush;
		nameIdMap.insert(std::make_pair(friendlyName,id));
		counter++;
	}

//	std::cout << "Read and made pairs for ID maps" << std::endl; 
	// create 2D Communications Table that will show the type of communication
	// needed for two AS systems to communicate.
	commTable = new char *[numAS];

	for(int i = 0; i < numAS; i++)
	{
		commTable[i] = new char [numAS];
	}

	inf.close();

//	std::cout << "Created my commtable " << std::endl;

	// read comunication table file (commTable) to populate the commTable container

	std::ifstream inf2("../catkin_ws/src/parent_comm/config/commTable.txt");
	char temp;

	// if can't open file
	if(!inf2.is_open())
	{
		std::cerr << "Uh oh, file could not be opened for reading! " << std::endl;
		exit(1);
	}

	//std::cout << "Am i advancing? " << std::endl; 
	// populate communication table 
	// maybe we should be grabbing just the row that has to do with this AS system 
	for(int i = 0; i < numAS; i++)
	{
		//std::cout << "Am I in the first loop " << std::endl;
		for(int j = 0; j < numAS; j++)
		{
			//std::cout << "Am I in the second loop" << std::endl; 
			inf2 >> temp; 
			//std::cout << temp << std::endl;
			while(temp < 57)
			{
				inf2 >> temp; 
			}
			commTable[i][j] = temp; 
		}
		//std::cout << "Am I stuck" << std::endl; 
	}

	inf2.close(); 

//	std::cout << "Populated commTable correctly " << std::endl; 

	std::map<std::string, int>::iterator it = nameIdMap.begin();
	id = (nameIdMap.find(systemName))->second;
/* 	while(it != nameIdMap.end())
	{
		if(it->first == systemName)
		{
			id = it->second;
			break; 
		}
		it++;
	}
*/
//	std::cout << "Found system name id: " << id << std::endl; 

	bool Rflag = false;
	bool Bflag = false;  



	//AddMsgQueue();
	
	// Hard-coded testing code.
 
//	BaseCommPtrs.insert(std::make_pair('B', new BlueComm()));

//	std::map<char,BaseComm *>::iterator BaseCommIt = BaseCommPtrs.begin(); 
//	BaseCommIt->second->Init();
//	BaseCommIt->second->setMsgFcnPtr(fcnPtr);

///Code below will be used in actual implementation ///
	for(int i = 0; i < numAS; i++)
	{
		if(commTable[id][i] == 'R' && Rflag == false)
		{
		//	std::cout << "Creating RosComm" << std::endl; 
			BaseCommPtrs.insert(std::make_pair('R', new RosComm(commTable[id], nh,fcnPtr, systemId)));
		//	BaseCommPtrs.push_back(new RosComm());
			Rflag = true; 
		}
		else if(commTable[id][i] == 'B' && Bflag == false)
		{
		//	std::cout << "Creating BlueComm" << std::endl; 
			BaseCommPtrs.insert(std::make_pair('B', new BlueComm(fcnPtr,systemId)));
			//BaseCommPtrs.push_back(new BlueComm());
			Bflag = true; 
		}
	}

	std::cout << "DONE INITIALIZING" << std::endl; 

/*
	std::cout << "CHECKING IF THINGS WERE DONE CORRECTLY" << std::endl; 
	std::cout << "Number of autonomous systems " << numAS << std::endl;
	std::cout<< "Checking NameIdMap" << std::endl;
	it = nameIdMap.begin();
	while(it != nameIdMap.end())
	{
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}

	std::cout << "Checking Communication Table " << std::endl; 
	for(int i = 0; i < numAS; i++)
	{
		//std::cout << "Am I in the first loop " << std::endl;
		for(int j = 0; j < numAS; j++)
		{
			std::cout << i << " " << j << " " << commTable[i][j] << std::endl;
		}

	}*/
}

int Comm::SendPtoP(int source, int sourceMod, Message  * msg, std::string dest, int moduleId)
{	
	//std::cout << "In send p to p " << std::endl << std::flush; 
//	int sourceId = GetId(systemName);
	int destId = GetId(dest);

	// Create message data buffer
	int bufferSize = msg->GetSize() + msg->GetHeaderSize(); 
	int * dataBuffer = new int[bufferSize]; 

	// Set remainder header attributes
	msg->SetMsgDataSize(bufferSize - msg->GetHeaderSize());
	msg->SetDestId(GetId(dest),sourceMod);
	msg->SetCommType(commTable[source][destId]);
	msg->SetBufHeader(dataBuffer);
	msg->Serialize(dataBuffer);

	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
	//std::cout << sourceId << " " << destId << std::endl; 
	int success = 0; 
	int counter = 0; 

	// Iterate through communication pointers to get correct one
	// Then send the data buffer
	for(int i = 0; i < BaseCommPtrs.size(); i++)
	{
		while(success == 0 && counter < 3)
		{
			success = (getPtr(commTable[source][destId])->SendPtoP(dataBuffer,dest)); 
			counter++;
			//sleep(5);
		}
	}

	//std::cout << "Done with Comm Send P to P " << std::endl; 
	if (success == 1)
		return 1;

	return 0; 
}

int Comm::SendBd(int source, int sourceMod, Message * msg)
{
	//std::cout << "In send Bd " << std::endl; 
	bool success = false; 

	// Create message data buffer
	int bufferSize = msg->GetSize() + msg->GetHeaderSize(); 
	int * dataBuffer = new int[bufferSize]; 

	// Set remainder header attributes
	msg->SetMsgDataSize(bufferSize - msg->GetHeaderSize());
	msg->SetDestId(-1,-1);
	msg->SetCommType('n');
	msg->SetBufHeader(dataBuffer);
	msg->Serialize(dataBuffer);

	//int id = 
	//getPtr('B')->GetId(systemName); 
	//std::cout << "id " << id << std::endl; 
	std::map<std::string,int>::iterator it = nameIdMap.begin();


    for(int i = 0; i < nameIdMap.size(); i++)
	{
		//std::cout << "it->second " << it->second << " id " << source << std::endl; 
		if(it->second == source)
		{
			it++;
			continue;
		}	
	//	std::cout << "id " << id  << " " << "i " << i << std::endl << std::flush; 	
	//	std::cout << "dest name " << it->first << std::endl << std::flush;
		// could keep editing the buffer for it to have correct destination, and type
		dataBuffer[2] = (int) commTable[source][it->second];
		dataBuffer[7] = it->second;
		dataBuffer[8] = dataBuffer[1];

		int counter = 0; 
		
		while(success == 0 && counter < 3)
		{
			success = (getPtr(commTable[source][it->second])->SendPtoP(dataBuffer,it->first)); 
			counter++;
		//	sleep(5);
		}
		success = 0; 
		it++; 
	}
	return success; 
}

bool Comm::CheckForMessage(int moduleId)
{
	//if( messageBacklog.size() != 0)
	//	return true; 
	//else 
	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
	return it->second->CheckForMessage(moduleId); 
	
}

Message * Comm::GetMessage( int moduleId)
{
	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
	return it->second->GetMessage(moduleId);
}

void Comm::AddMsgQueue()
{
	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
	it->second->UpdateMsgLogNum(); 

}

BaseComm * Comm::getPtr(char type)
{
	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
//	std::cout << "type " << type << std::endl; 
//	std::cout << BaseCommPtrs.size() << std::endl;
	for(unsigned int i = 0; i < BaseCommPtrs.size(); i++)
	{	
		//std::cout << it->first << std::endl; 
		if(it->first == type)
		{
			BaseComm * ptr = it->second;
			return ptr;
		}
		it++;
	}
	std::cout << "Initialization Problem. Check configuration files. " << std::endl; 

	exit(EXIT_FAILURE);
	BaseComm * ptr = nullptr; 
	return ptr; 
}

int Comm::GetId(std::string name)
{
	//std::map<std::string,int>::iterator it = nameIdMap.begin();
	return (nameIdMap.find(name))->second;
/* 	for(int i = 0 ; i < nameIdMap.size(); i++)
	{
		if(it->first == name) 
		{
			return it->second;
		}
		it++;
	}*/

	//return -1; 
}

int Comm::GetNextMsgType(int moduleId)
{
	std::map<char, BaseComm *>::iterator it = BaseCommPtrs.begin();
	return it->second->GetNextMsgType(moduleId);
}
