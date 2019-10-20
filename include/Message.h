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
	// Virtual functions to be implemented in child classes
	virtual void Serialize(int * dataBuf) = 0; 
	virtual void DeSerialize(int * dataBuf) = 0; 
	virtual int GetSize() = 0; 
	virtual void printData() = 0; 
	virtual Message * Clone() = 0; 

	// Setters and Getters for header attributed
	void SetId(int ASid);
	std::tuple<int,int> GetMsgId();

	void SetSourceId(int ASId, int moduleId);
	std::tuple<int, int> GetSourceId();

	void SetDestId(int ASId, int moduleId);
	std::tuple<int,int> GetDestId(); 

	void SetCommType(char commType);
	char GetCommType();

	void SetMsgDataSize(int msgDataSize);
	int GetMsgDataSize();

	void SetMsgType(int msgType);
	int GetMsgType();

	void SetBufHeader(int * buf);
	void SetHeaderAttr(std::tuple<int,int> msgId, int commType, int msgSize, int msgType, std::tuple<int, int> sourceId, std::tuple<int,int> destId);

	int GetHeaderSize();

	// Header size
	static int _headerSize;																	// header size

protected:
	static int _id; 																		// message id for this rover
	std::tuple<int, int> _msgId;															// msg ID: <AS id, msg id> 
	std::tuple<int,int> _sourceId;															// source ID : < AS id, module id>	
	std::tuple<int,int> _destId;  															// destination ID: <AS id, module id>
	char _commType; 																		// communication type (B for Bluetooth, R for Ros)	
	int _msgDataSize; 																		// message size 
	int _msgType; 
	bool _msgIdFlag = false; 

private:	
	
};

#endif
