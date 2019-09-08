#include "Message2.h"

Message2::Message2()
{
    // All messages need to have the following two set" 
    SetMsgType(1);

    _data = " ";
    _character = ' ';
    _firstNum = 45.67;
    _data2 = " ";
    _secondNum = 26.23;
}

Message2::Message2(Message2 & msg)
{
    _character = msg.getCharacter();
    _firstNum = msg.getFirstNum();
    _secondNum = msg.getSecondNum(); 
}
Message2::~Message2()
{
    
}

Message * Message2::Clone()
{
	return new Message2(*this);
}

void Message2::printData()
{}

void Message2::Initialize(std::string data, char character, double firstNum, std::string data2, float secondNum)
{
    _data = data;
    _character = character;
    _firstNum = firstNum;
    _data2 = data2;
    _secondNum = secondNum;
}

void Message2::Serialize(int * dataAddress)
{

}

void Message2::DeSerialize(int * dataAddress)
{

}

int Message2::GetSize()
{
    return 1; 
}

char Message2::getCharacter()
{
    return _character;
}

double Message2::getFirstNum()
{
    return _firstNum;
}

float Message2::getSecondNum()
{
    return _secondNum; 
}