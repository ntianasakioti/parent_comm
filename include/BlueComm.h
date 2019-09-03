#ifndef BLUECOMM_H
#define BLUECOMM_H

#include <string>
#include <vector>
#include <map>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <iostream>
#include <thread>
//#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include "BaseComm.h"

// Class that handles autonomous system communication via bluetooth //
class BlueComm : public BaseComm 
{
public:
	BlueComm();
	/*
		BlueComm
			Parameters: 
				none
			Return value:
				BlueComm
			Behavior:
				Constructor for BlueComm object. Sets the terminating variable to false
	*/

	~BlueComm(); 
	/*
		~BlueComm
			Parameters:
				none
			Return value:
				none
			Behavior:
				Destructor for BlueComm object. Closes sockets
	*/

	void Setup(); 
	void InitComm();
	/*
		InitComm
			Parameters:
				none
			Return value:
				none
			Behavior:
				Initialization for communication. Starts listening thread
	*/


	int SendPtoP(int *  dataBuffer, std::string dest); 
	/*
		SendPtoP
			Parameters:
				string type (will be changed to Message type) message to be sent
				(destination??) 
			Return value:
				none currently (possibly boolean for check of sending success)
			Behavior:
				Sends message for point-to-point communication
	*/

	void Terminate();
	/*
		Terminate
			Parameters:
				none
			Return value:
				none
			Behavior:
				Destroys all necessary variables and calls destructor? 
	*/

	void ListeningThread();
	/*
		ListeningThread
			Parameters:
				none
			Return value:
				none
			Behavior:
				Listens for communication from other AS and saves messages to messageBacklog
	*/
	// message returning accessing function (checkBuffer) 

protected:

private:
	std::string name;													// AS name as used in communication (i.e. "rover 1", "red", "blue")
	int id; 															// AS ID
	//bool terminate;													// termination signal 
	//pthread_t ltId; 													// listening thread ID
	std::thread ltThread;												// listening thread
	std::vector<std::string> addresses;									// friendly names of rovers								
//	std::vector<int> * sockets;  	
	std::map<int,std::string> BdAddresses; 								
};

#endif
