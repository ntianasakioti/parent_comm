#ifndef MESSAGE2_H
#define MESSAGE2_H
#include "Message.h"


class Message2 : public Message
{
public:
    Message2();
    Message2(Message2 & msg);
    ~Message2();
    Message2 * Clone();
    void Initialize(char character, double firstNum, float secondNum);
    void Serialize(int * dataBuf);
    void DeSerialize(int * dataBuf);
    int GetSize(); 
    void printData();

    char getCharacter();
    double getFirstNum();
    float getSecondNum(); 


private:

  // std::string _data; 
   char _character;
   double _firstNum;
 //  std::string _data2;
   float _secondNum;


};

#endif
