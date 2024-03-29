#ifndef MESSAGE1_H
#define MESSAGE1_H
#include "Message.h"
#include <string>


class Message1 : public Message
{
public:
    Message1();
    Message1(Message1 & msg);
    void Initialize(int num1, char char1, double num2);
    ~Message1();
    Message1 * Clone(); 
    void Serialize(int * dataAddress);
    void DeSerialize(int * dataAddress);
    int GetSize();
    void printData();

    int getRandomNum();
    char getCharacter();
    double getSecondNum(); 

private:
   int _randomNum; 
   char _character;
   double _secondNum;
};

#endif
