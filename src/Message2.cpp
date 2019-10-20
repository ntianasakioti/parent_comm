#include "Message2.h"

Message2::Message2()
{
    // All messages need to have the following two set" 
    SetMsgType(1);

   // _data = " ";
    _character = ' ';
    _firstNum = 45.67;
  //  _data2 = " ";
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

Message2 * Message2::Clone()
{
	return new Message2(*this);
}

void Message2::printData()
{}

void Message2::Initialize(char character, double firstNum, float secondNum)
{
   // _data = data;
    _character = character;
    _firstNum = firstNum;
  //  _data2 = data2;
    _secondNum = secondNum;
}

void Message2::Serialize(int * dataBuf)
{
	int index = Message::_headerSize;
	int *dataRef;

	dataRef = (int*) (&_firstNum);
    for(int i = 0; i < sizeof(_firstNum)/sizeof(int); i++){
        dataBuf[index++] = dataRef[i];}
    int temp = (int) _character;
    dataRef = (int*) (&temp);
    for(int i = 0; i < sizeof(temp)/sizeof(int); i++){
        dataBuf[index++] = dataRef[i];}
        /// DOUBLE conversion like this will work but when you output the single element it will not look right
    dataRef = (int*) (&_secondNum);
    for(int i = 0; i < sizeof(_secondNum)/sizeof(int); i++){
        dataBuf[index++] = dataRef[i];}

    std::cout << "Serialize:  " <<  ": buffersize = " << GetSize() + Message::_headerSize << std::endl << std::flush;
	for (int i = 0; i < GetSize() + Message::_headerSize; i++) {
		std::cout << dataBuf[i] << " "; }
	std::cout << std::endl << std::flush;
}

void Message2::DeSerialize(int * dataBuf)
{
	
    std::cout << "Deserialize: " << ": buffersize = " << GetSize() << std::endl << std::flush;
	for (int i = Message::_headerSize; i < GetSize() + Message::_headerSize; i++) {
		std::cout << dataBuf[i] << " "; }
	std::cout << std::endl << std::flush;

    int index = Message::_headerSize;
  //  std::cout << "HeaderSize " << Message::_headerSize << std::endl; 
    int * dataRef;

	 dataRef = (int*) (&_firstNum);
    for(int i = 0; i < sizeof(_firstNum)/sizeof(int); i++){
        dataRef[i] = dataBuf[index++];}
    dataRef = (int*) (&_character);
    for(int i = 0; i < sizeof(int)/sizeof(int); i++){
        dataRef[i] = (char) (dataBuf[index++]);}
    dataRef = (int*) (&_secondNum);
    for(int i = 0; i < sizeof(_secondNum)/sizeof(int); i++){
        dataRef[i] = dataBuf[index++];}
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
