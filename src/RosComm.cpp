#include "RosComm.h"

RosComm::RosComm(char * myCommRow, ros::NodeHandle * nh)
{
	_myCommRow = myCommRow;
	_nh = nh; 
	Init();
	Setup();
}

RosComm::~RosComm()
{
	
}

void RosComm::Setup()
{
	// Get my Point-to-Point topic name (my AS topic name)
	std::map<std::string, int>::iterator it; 
	it = nameIDs.begin();
	std::string topicPtoP; 

	for(int i = 0; i < nameIDs.size(); i++)
	{
		if(GetMyId() == it->second)
		{
			topicPtoP = it->first; 
			break;
		}
		it++;
	}

	// Create Listeners 
	ros::Subscriber subPtoP = _nh->subscribe(topicPtoP, 1000, &RosComm::messageCallback, this);
	ros::Subscriber subBroadcast = _nh->subscribe("Broadcast", 1000, &RosComm::messageCallback, this);

	// Create Publishers
	it = nameIDs.begin(); 
	for(int i = 0; i < nameIDs.size(); i++)
	{
		std::cout << "i : " << i << "comm " << _myCommRow[i] << std::endl; 
		if(_myCommRow[i] != GetMyId() && _myCommRow[i] == 'R')
		{
			std::cout << "Creating publisher for topic " << it->first << std::endl;
			ros::Publisher * newPub = new ros::Publisher(_nh->advertise<std_msgs::Int32MultiArray>(it->first, 100));
			std::cout << "Inserting publisher pair "<< i << " " << it->first << std::endl; 
			_publishers.insert(std::make_pair(it->second, newPub));
		}
		it++;
	}
		// Create Broadcast publisher
		//ros::Publisher newPub = new ros::Publisher(_nh->advertise<std_msgs::Int32MultiArray("Broadcast", 100));
		//_publishers.insert(std::make_pair(-1,newPub));
}

void ListeningThread()
{
	
}

int RosComm::SendPtoP(int * dataBuffer, std::string dest)
{
	std::cout << "In roscomm's sent p to p" << std::endl << std::flush; 

	std_msgs::Int32MultiArray dataArray;
	dataArray.data.clear(); 

	for(int i = 0; i < dataBuffer[4] + 9; i++)
	{
		dataArray.data.push_back(dataBuffer[i]);
	}

	int id; 
	std::map<std::string, int>::iterator it; 
	it = nameIDs.begin(); 
	for(int i = 0; i < nameIDs.size(); i++)
	{
		if(it->first == dest)
		{
			id = it->second; 
			break;
		}
		it++;
	}

	std::map<int, ros::Publisher *>::iterator it2;
	it2 = _publishers.begin();
	for(int i = 0; i < _publishers.size(); i++)
	{
		if(id == it2->first)
		{
			(it2->second)->publish(dataArray);
			ros::spinOnce(); 
			return 1; 
		}
	}

	ros::spinOnce();
	return 0;
}

/* int RosComm::SendBd(int * dataBuffer)
{
	ros::spinOnce();
	return 0; 
}
*/
void RosComm::messageCallback(const std_msgs::Int32MultiArray::ConstPtr& array)
{
	// check that message is not a broadcast from "me"
	std::vector<int>::const_iterator it = array->data.begin(); 
	if (*it != GetMyId())
	{
		int * buf = new int[array->data.size()];
		// copy serialized content to buffer
		for(int i = 0; i < array->data.size(); i++)
		{
			//std::cout << "temp buf " << array[i] << std::endl; 
			buf[i] =*it;
			it++;
		}

		// call function to update message log
		MutexLock();
		UpdateMessageLog(buf, buf[6]);
		MutexUnlock();
		delete buf; 
	}
}