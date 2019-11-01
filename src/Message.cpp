#include "Message.h"

int Message::_id = 0;
int Message::_headerSize = 9; 

void Message::SetId(int ASid)
{
    // If the AS ids match, then we need to increment the static id 
    if(_msgIdFlag == false)
    {
        _msgIdFlag = true; 
        _msgId = std::make_pair(ASid, _id++);
    }
}

std::tuple<int,int> Message::GetMsgId()
{
    return _msgId;
}

void Message::SetSourceId(int ASId, int moduleId)
{
    _sourceId = std::make_pair(ASId, moduleId);
}

std::tuple<int, int> Message::GetSourceId()
{
   // std::cout << "I want to return " << std::get<0>(_sourceId) << " " << std::get<1>(_sourceId) << std::endl;
    return _sourceId;
}

void Message::SetDestId(int ASId, int moduleId)
{
    _destId = std::make_pair(ASId, moduleId);
}

std::tuple<int,int> Message::GetDestId()
{
    return _destId;
}

void Message::SetCommType(char commType)
{
    _commType = commType;
}

char Message::GetCommType()
{
    return _commType;
}
	
void Message::SetMsgDataSize(int msgDataSize)
{
    _msgDataSize = msgDataSize;
}

int Message::GetMsgDataSize()
{
    return _msgDataSize;
}

void Message::SetMsgType(int msgType)
{
    _msgType = msgType; 
}

int Message::GetMsgType()
{
    return _msgType;
}

void Message::SetBufHeader(int * buf)
{
    buf[0] = std::get<0>(_msgId);   
    buf[1] = std::get<1>(_msgId);
    buf[2] = int (_commType);
    buf[3] = _msgType;  
    buf[4] = _msgDataSize;    
    // hard coded for testing
    //buf[4], buf[5], buf[6], buf[7] = 0; 


    buf[5] = std::get<0>(_sourceId);
    buf[6] = std::get<1>(_sourceId);
    buf[7] = std::get<0>(_destId);
    buf[8] = std::get<1>(_destId);
}

int Message::GetHeaderSize()
{
    return Message::_headerSize;
}

void Message::SetHeaderAttr(std::tuple<int,int> msgId, int commType, int msgType, int msgSize, std::tuple<int, int> sourceId, std::tuple<int,int> destId)
{
   // std::cout << "Setting my attributes " <<  std::get<0>(msgId) << " " << std::get<1>(msgId) 
  //  << commType << " " << msgType << " " << msgSize << " " << std::get<0>(sourceId) << " "
   // << std::get<1>(sourceId)<< " " << std::get<0>(destId) << " " << std::get<1>(destId) << std::endl;
    _msgId = std::make_tuple(std::get<0>(msgId), std::get<1>(msgId));
    _sourceId = std::make_tuple(std::get<0>(sourceId), std::get<1>(sourceId));
    _destId = std::make_tuple(std::get<0>(destId), std::get<1>(destId));
    _commType = (char) commType;
    _msgDataSize = msgSize;
    _msgType = msgType;
}
