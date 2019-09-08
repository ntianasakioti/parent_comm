#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include <math.h>
#include <cstring>

//enum messageType {0, 1, 2};
class Message
{
public: 
	virtual void Serialize(int * dataBuf) = 0; 
	virtual void DeSerialize(int * dataBuf) = 0; 
	virtual int GetSize() = 0; 
	virtual void printData() = 0; 
	virtual Message * Clone() = 0; 
	std::string GetSender();
	void SetId(int msgASId, int myASId, int msgId);
	void SetSourceId(int ASId, int moduleId);
	std::tuple<int, int> GetSourceId();
	void SetDestId(int ASId, int moduleId);
	void SetCommType(char commType);
	void SetMsgDataSize(int msgDataSize);
	void SetMsgType(int msgType);
	int GetType();
	void SetBufHeader(int * buf);
	int GetHeaderSize();
	void SetHeaderAttr(std::tuple<int,int> msgId, int commType, int msgSize, int msgType, std::tuple<int, int> sourceId, std::tuple<int,int> destId);

	static int _headerSize;																	// header size

protected:
	static int _id; 																		// message id for this rover
	std::tuple<int, int> _msgId;															// msg ID: <AS id, msg id> 
	std::tuple<int,int> _sourceId;															// source ID : < AS id, module id>	
	std::tuple<int,int> _destId;  															// destination ID: <AS id, module id>
	char _commType; 																		// communication type (B for Bluetooth, R for Ros)	
	int _msgDataSize; 																		// message size 
	int _msgType; 

private:	
	
};

#endif