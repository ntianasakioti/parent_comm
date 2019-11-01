#include "CommInt.h"
//#include <boost/date_time.hpp>
#include "Comm.h"
#include "Message1.h"
#include <cstdio>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include "ros/ros.h"

Message * getMessage( int id)
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

int main()
{  
	// ROS initializations
	int argc = 0;
	char ** argv; 
	ros::init(argc, argv, "Leathrum");

	ros::NodeHandle * nh = new ros::NodeHandle; 

	Message * (*fcnPtr)(int);
	fcnPtr = getMessage;
	srand(time(0)); 


	Message1 * msg = new Message1();
	std::cout << std::endl << "RESTART" << std::endl; 
	msg->Initialize( 1 , 'd', 32.657);
 	 
	CommInt * commint = new CommInt(fcnPtr, nh);

  
	// Getting time that broadcast message will be sent
	//boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
//	std::string time_string;
	//strftime(time_string, 50, "Time %T", timeLocal.time_of_day());
	//std::cout << "Time " << timeLocal.time_of_day() << std::endl; 
	sleep(15);

	//msg  = "Broadcast from Leathrum at " + to_simple_string(timeLocal.time_of_day()); 

	ros::spinOnce();
	// Sending Broadcast message
//	std::cout << "Before send bd " << std::endl << std::flush; 
	commint->SendMessage("All", msg);
	sleep(7);
//	std::cout << "After Send Broadcast call " << std::endl << std::flush; 

	int counter = 0; 
	ros::spinOnce();
	Message1 * msg2 = new Message1();
	msg2->Initialize( 'f', 45.6, 29.5);
	// Send 5 Point-to-Point Messages 
	while(counter < 5)	
	{
		ros::spinOnce();
		//timeLocal = boost::posix_time::second_clock::local_time();
		commint->SendMessage(rand() < RAND_MAX /2 ? "Ntiana" : "OldLeathrum", msg2);
		//commint->SendMessage("OldLeathrum", msg);
		//std::cout << "After this PtoP Send at Main " << std::endl; 
		sleep(7);
		counter++;
	}
	ros::spinOnce();
	//std::cout << "After all Send PtoP in main " << std::endl; 
	//msg  = "Broadcast from Leathrum at " + to_simple_string(timeLocal.time_of_day()); 
	delete msg; 														/// when perror is done this is what brings the error free(): corrupted unsorted chunks: 0x00000000018b4c20 ***
	// could it be that we serialize every time we go into BlueComm's sendPtoP
	// let's try serializing only once. before the commintSend function is called. and change all sends to sending an int 8 buffer 
	// 
	// when should the message be deleted 
	//std::cout << "After deleting the message" << std::endl; 
	ros::spinOnce();
	bool messageCheck = commint->CheckForMessage(0);
	while( messageCheck == true)
	{
		ros::spinOnce(); 
		std::cout << "There is a message, I am taking it out " << std::endl; 
		Message1 * newMsg;
		Message2 * newMsg2; 
		if(commint->GetNextMsgType(0) == 0)
		{
			newMsg = dynamic_cast<Message1 *>((commint->GetMessage(0))->Clone()); 
			std::cout << newMsg->getRandomNum();
			delete newMsg; 
		}
		else if(commint->GetNextMsgType(0) == 1)
		{
			newMsg2 = dynamic_cast<Message2 *>(commint->GetMessage(0)->Clone()); 
			std::cout << newMsg2->getCharacter(); 

			delete newMsg2; 
		}
		//std::tuple<int,int> sourceId = newMsg->GetSourceId();  
		//std::cout << "it came from " << std::get<0>(sourceId) << " " << std::get<1>(sourceId) << std::endl; 
		//std::cout << "printing data " ;
		//newMsg->printData();
		std::cout << std::endl; 
		ros::spinOnce();
		messageCheck = commint->CheckForMessage(0);
		//std::cout << "check for message result in main " << commint->CheckForMessage(0) << std::endl; 
		sleep(5);
	}

	std::cout << "after while loop at end of main" << std::endl; 
	sleep(100);

	return 0;
}
