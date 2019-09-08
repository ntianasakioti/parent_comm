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

Message2::~Message2()
{
    
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