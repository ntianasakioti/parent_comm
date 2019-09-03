#ifndef MESSAGE1_H
#define MESSAGE1_H
#include "Message.h"
#include <string>


class Message1 : public Message
{
public:
    Message1();
    void Initialize(int num1, char char1, double num2);
    ~Message1();
    void Serialize(int * dataAddress);
    void DeSerialize(int * dataAddress);
    int GetSize();
    std::string getData();

private:
   int _randomNum; 
   char _character;
   double _secondNum;
};

#endif