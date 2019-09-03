#ifndef MESSAGE2_H
#define MESSAGE2_H
#include "Message.h"


class Message2 : public Message
{
public:
    Message2();
    ~Message2();
    void Initialize(std::string data, char character, double firstNum, std::string data2, float secondNum);
    void Serialize(int * dataAddress);
    void DeSerialize(int * dataAddress);
    int GetSize(); 


private:

   std::string _data; 
   char _character;
   double _firstNum;
   std::string _data2;
   float _secondNum;


};

#endif