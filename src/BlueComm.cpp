#include "BlueComm.h"

#include <boost/date_time.hpp>
BlueComm::BlueComm(Message * (*fcnPtr)(int), int id)
{
//	std::cout << "Object created" << std::endl << std::flush; 
	Init(id);
	InitComm(); 
	setMsgFcnPtr(fcnPtr);
}

BlueComm::~BlueComm()
{
}

void BlueComm::Setup()
{
	std::ifstream inf("../catkin_ws/src/parent_comm/config/bdAddresses.txt"); 

	// if can't open file
	if(!inf.is_open())
	{
		std::cerr << "Uh oh, file could not be opened for reading! " << std::endl;
		exit(1);
	}

//	std::cout << "Opened bdAddresses correctly" << std::endl;

	int id;
	std::string address;
	
	while(!inf.eof())
	{
		inf >> id;
		inf >> address; 
		BdAddresses.insert(std::make_pair(id,address));
	}
	
}

void BlueComm::InitComm()
{
	// Set and Create separate thread
	Setup(); 

//	std::cout<< "Creating listening thread " << std::endl << std::flush; 
	
	std::thread ltThread(&BlueComm::ListeningThread, this);

	ltThread.detach();
	
//	std::cout << "Continuing " << std::endl << std::flush; 
}

void BlueComm::ListeningThread()
{

    bool terminate = false; 
    bool test = true; 
   // while(test == true)
    //{
//	std::cout<< "Launched my separate thread " << std::endl; 

	// Set up Bluetooth Socket and address parameters
	// to listen for a message 
	struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
	int bufTemp[1024] = {0};
	int t, client, bytes_read; 
	socklen_t opt = sizeof(rem_addr);

	// allocate a socket
    t = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);

	// bind socket to port 1 of the first available //
	// local bluetooth adapter 
	bdaddr_t temp = {};
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = temp; 
	loc_addr.rc_channel = (uint8_t) 1; 
	bind(t, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	int messageSize; 

	// put socket to listen mode 
	//std::cout << "Waiting to listen " << std::endl;
	listen(t,20);

	// Just for testing to keep track of the loop
	int counter = 0; 

	while (terminate == false)
	{
		//std::cout << "I AM IN THE WHILE LOOP " << std::endl; 
		//std::cout << "Counter " << counter << std::endl << std::flush; 

		// accept one connection 
		client = accept(t, (struct sockaddr *)&rem_addr, &opt);

		//std::cout << "After I accepted connection " << std::endl; 
		//ba2str(&rem_addr.rc_bdaddr, buf );
		//fprintf(stderr,"accepted connection from %s\n", buf);
		//memset(buf, 0, sizeof(buf));

		// read message
		char c;
		bytes_read = read(client, bufTemp, sizeof(bufTemp));
		if(bytes_read > 0 )
		{
			for(int i = 0; i < bytes_read; i++)
			{
					c = bufTemp[i];
				//	std::cout << c  << std::endl;
					//printf("received[%i]\n", buf + '0');
					//buf++;
			}
		}


		// put int data into buffer int pointer array 
		int * buf = new int[bufTemp[4]+9];
		for(int i = 0; i < bufTemp[4]+ 9; i++)
		{
			//std::cout << "temp buf " << bufTemp[i] << std::endl; 
			buf[i] = bufTemp[i];
		}

		// create correct Message type 
		//Message * msg = getMsgFromId(buf[2]);
		//msg->SetId(buf[0]);
		//std::tuple<int, int> source = std::make_tuple(buf[4], buf[5]);
		//std::tuple<int, int> dest = std::make_tuple(buf[6], buf[7]);
		//msg->SetHeaderAttr(buf[1], buf[2], buf[3], source, dest);
		//msg->DeSerialize(buf);
		//std::cout << "BLUETOOTH HEARD SOMETHING " << std::endl; 
		UpdateMessageLog(buf, buf[6]);
		delete buf; 

		counter++; 
	}

        terminate = false;   
		//shutdown(t, SHUT_RD);
        close(t);
   // }

	//// don't think I need the outer loop. As long as with this code it can accept enough connections 
	// to be determined upon testing to receive 6 messages. 
}

int BlueComm::SendPtoP(int * dataBuffer, std::string dest)
{

	//std::cout << "In bluecomm's sent p to p" << std::endl << std::flush; 
	// Find destination address //
	
	int id = GetId(dest);
	//std::cout << "Id i will send to " << id << std::endl << std::flush; 
	std::map<int, std::string>::iterator it = BdAddresses.begin();
	while(it->first != id)
	{
		//std::cout << "Comparing id " << it->first << "to " << id << std::endl;
		it++;
		//std::cout << "incremented " << std:: endl; 
	}

	//std::cout << "bluetooth address " << it->second << std::endl << std::flush;
	char destination[18];
	strcpy(destination,(it->second).c_str());

	//std::cout << "destination " << destination << std::endl << std::flush;
	
	int s, status; 
	struct sockaddr_rc addr = {0};

	s = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);


	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1; 
	str2ba(destination, &addr.rc_bdaddr);


	//int * dataBuf = new int(msg->GetSize() + msg->GetHeaderSize()); 
	//std::cout << "sizes " << msg->GetHeaderSize() << " " << msg->GetSize() << std::endl; 
	//msg->Serialize(dataBuf);

//	std::cout << "After serialization " << std::endl; 

	int int_array[dataBuffer[4 ]+ 9];
	//int int_array[msg->GetSize() + msg->GetHeaderSize()];

	//std::cout << "After creating array " << std::endl; 

	for(int i = 0; i < dataBuffer[4] + 9; i++)
	{
		//std::cout << i << " " << dataBuffer[i] << std::endl; 
		int_array[i] = dataBuffer[i];
	}

//	std::cout << "Before I connect " << std::endl; 

	status = connect(s,(struct sockaddr*)&addr, sizeof(addr));

	//std::cout << "After I connect " << std::endl; 
	//std::cout << "status " << status << std::endl; 
	if(status == 0)
	{
	//	std::cout << "before writing " << std::endl;
		write(s, int_array,sizeof(int_array));
	//	std::cout << "after writing " << std::endl; 
		close(s);
	//	std::cout << "Success in sending " << boost::posix_time::second_clock::local_time().time_of_day() << std::endl << std::flush;
		return 1; 
	}
	else if(status < 0 )
	{
		//std::cout  << "status " << status;
		//delete msg; 
		//std::cout << "Before perror" << std::endl; 
		perror("uh oh");
		//std::cout << "after perror" << std::endl; 
		//shutdown(s, SHUT_WR);
		close(s);
	//	std::cout << "I closed my socket" << std::endl; 
		return 0; 
	}
	close(s);
	return 0;

}

