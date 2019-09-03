#include "Message.h"

int Message::_id = 0;
int Message::_headerSize = 9; 

std::string Message::GetSender()
{
    return "who";
}

void Message::SetId(int msgASId, int myASId, int msgId)
{
    // If the AS ids match, then we need to increment the static id 
    if( msgASId == myASId)
    {
        _id++; 
        _msgId = std::make_pair(msgASId, _id);
    }
    // If the AS ids don't match it, then we set the id using the buffer information provided
    // by the parameters
    else
    {
        _msgId = std::make_pair(msgASId, msgId);
    }
     
}

void Message::SetSourceId(int ASId, int moduleId)
{
    _sourceId = std::make_pair(ASId, moduleId);
}

std::tuple<int, int> Message::GetSourceId()
{
    return _sourceId;
}

void Message::SetDestId(int ASId, int moduleId)
{
    _destId = std::make_pair(ASId, moduleId);
}

void Message::SetCommType(char commType)
{
    _commType = commType;
}
	
void Message::SetMsgDataSize(int msgDataSize)
{
    _msgDataSize = msgDataSize;
}

void Message::SetMsgType(int msgType)
{
    _msgType = msgType; 
}

int Message::GetType()
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
    std::cout << "Setting my attributes " <<  msgSize << std::endl;
    this->_commType = '1';
    std::cout<<"this:"<<this->_commType<<std::endl;
    std::cout<<"Did it\n";
    _msgId = std::make_pair(1, 1);
    std::cout << 1 << std::endl;
    _sourceId = sourceId;
    std::cout << 2 << std::endl; 
    _destId = destId;
    std::cout << 3 << std::endl; 
    _commType = (char) commType;
    std::cout << 4 << std::endl;
    _msgDataSize = msgSize;
    std::cout << 5 << std::endl; 
    _msgType = msgType;
    std::cout << "I set my attributes " << std::endl; 
}
