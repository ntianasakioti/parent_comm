#include "CommInt.h"
#include <boost/date_time.hpp>
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
	ros::init(argc, argv, "talker");

	ros::NodeHandle * nh = new ros::NodeHandle; 


//	int (*fcnPtr)(int) = foo; 
	Message * (*fcnPtr)(int);
	fcnPtr = getMessage;


	Message1 * msg = new Message1();
	std::cout << "Before initialize " << std::endl; 
	msg->Initialize( 1 , 'd', 32.657);
 	 
	CommInt * commint = new CommInt(fcnPtr, nh);

  
	// Getting time that broadcast message will be sent
	boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
//	std::string time_string;
	//strftime(time_string, 50, "Time %T", timeLocal.time_of_day());
	//std::cout << "Time " << timeLocal.time_of_day() << std::endl; 
	sleep(15);

	//msg  = "Broadcast from Leathrum at " + to_simple_string(timeLocal.time_of_day()); 

 
	// Sending Broadcast message
	std::cout << "Before send bd " << std::endl << std::flush; 
	commint->SendMessage("All", msg);

	sleep(7);
	std::cout << "After Send Broadcast call " << std::endl << std::flush; 

	int counter = 0; 
	// Send 5 Point-to-Point Messages 
	while(counter < 5)	
	{
		timeLocal = boost::posix_time::second_clock::local_time();
		msg->Initialize( 1, 'd', 32.657);
		commint->SendMessage(rand() < RAND_MAX /2 ? "Ntiana" : "OldLeathrum", msg);
		//commint->SendMessage("OldLeathrum", msg);
		std::cout << "After this PtoP Send at Main " << std::endl; 
		sleep(7);
		counter++;
	}

	std::cout << "After all Send PtoP in main " << std::endl; 
	//msg  = "Broadcast from Leathrum at " + to_simple_string(timeLocal.time_of_day()); 
	delete msg; 														/// when perror is done this is what brings the error free(): corrupted unsorted chunks: 0x00000000018b4c20 ***
	// could it be that we serialize every time we go into BlueComm's sendPtoP
	// let's try serializing only once. before the commintSend function is called. and change all sends to sending an int 8 buffer 
	// 
	// when should the message be deleted 

	std::cout << "After deleting the message" << std::endl; 

	while( commint->CheckForMessage(0) == true)
	{
		std::cout << "There is a message, I am taking it out " << std::endl; 
		Message * newMsg =  commint->GetMessage(0);
		std::tuple<int,int> sourceId = newMsg->GetSourceId();  
		std::cout << "it came from " << std::get<0>(sourceId) << " " << std::get<1>(sourceId) << std::endl; 
		delete newMsg; 
	}

	sleep(100);

	return 0;
}