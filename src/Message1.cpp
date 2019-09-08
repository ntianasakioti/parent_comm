#include "Message1.h"


Message1::Message1()
{
    // All messages need to have the following two set" 
    SetMsgType(0);
	SetId(1,1,0);

   // _data = " ";
    _randomNum = 0;
    _character = ' ';
    _secondNum = 0.0;
}
Message1::Message1(Message1 & msg)
{
	_randomNum = msg.getRandomNum();
	_character = msg.getCharacter();
	_secondNum = msg.getSecondNum();
	SetHeaderAttr(msg.GetMsgId(), msg.GetCommType(), msg.GetMsgType(), msg.GetMsgDataSize(), 
	msg.GetSourceId(), msg.GetDestId());

}

Message1::~Message1()
{

}

Message * Message1::Clone()
{
	return new Message1(*this);
}

void Message1::printData()
{
	std::cout << _randomNum << " " << _character << " " << _secondNum << std::endl; 
}

void Message1::Initialize(int num1, char char1, double num2)
{
    std::cout << "Initialize" << std::endl;
	SetMsgType(0);
    _randomNum = num1;
    _character = char1;
    _secondNum = num2; 
}


int Message1::GetSize()
{
    // add and return all sizes ( 1 for integers and characters sinze they will be represented by 
    // one integer)
    return ( 1 + 1 + (sizeof(_secondNum)/sizeof(int)));
}

void Message1::Serialize(int * dataBuf)
{
    int index = Message::_headerSize;
	int *dataRef;

    //std::cout << "HeaderSize " << Message::_headerSize << std::endl; 


	/// MY STRING TO INT ARRAY CONVERSION ///
    // convert string to 8 element integer array
	/* 
    int elementsInIndex = ceil(_data.length()/double((sizeof(_data)/sizeof(int))));														// number of characters in each myArray index
    int intArray[elementsInIndex];																										// array holding the characters turned into integers for one index of myArray
    std::string tempString;																												// temporary string used in the serialization
    int myArray[sizeof(_data)/sizeof(int)];																								// int array that will hold the serialized string size (8)
	int stringIndex = 0; 																												// used to keep track of the iteration through the original string

	// For the number of indexes set aside for the string (8)
    for(int i = 0; i < sizeof(_data)/sizeof(int); i++)
    {
		std::cout << i << std::endl; 

		// For the number of elements in the index 
		for(int j = 0; j < elementsInIndex; j++)
		{
			// if the string is less than 8 characters long
		//	if (_data.length() < sizeof(_data)/sizeof(int))
       	//	{
				   // if we have surpassed the end of the string 
				   if(stringIndex > _data.length())
				   {
					   std::cout << "Ending eh " << std::endl; 
					   tempString = tempString + "0";
					   std::cout << "blah " << tempString << std::endl; 
					    			// was continue; 
					}                                            
        //	}

		else{	intArray[j] = int(_data[stringIndex++]);
			std::cout << "int " << intArray[j] << std::endl;
			tempString = tempString + std::to_string(intArray[j]);
		}
		}

		/* if( stringIndex > _data.length())
		{
            std::cout<< "stringIndex " << stringIndex << "greater than data length " << _data.length() << std::endl; 
			myArray[i] = 0; 
            if( i == sizeof(_data)/sizeof(int) -1)
			{
				std::cout << "Before Setting my array " << std::endl; 
                myArray[i] = stoi(tempString);
				std::cout << "After setting my array " << std::endl; 
			}
			break; 				//was continue; seems wrong still 
		}*/

	/* 	std::cout << "temp string " << tempString << std::endl; 
        myArray[i] = stoi(tempString);
        tempString = "\0";
    }
    

    dataRef = myArray; 
	for (int i = 0; i < sizeof(_data)/sizeof(int); ++i){
        std::cout << "index " << index <<  std::endl; 
        dataBuf[index++] = dataRef[i];}*/


	/// Dr. L's STRING TO INT ARRAY CONVERSION ///


	/// Just 8 char String Conversion ///

    dataRef = (int*) (&_randomNum);
    for(int i = 0; i < sizeof(_randomNum)/sizeof(int); i++){
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

void Message1::DeSerialize(int * dataBuf)
{
    std::cout << "Deserialize: " << ": buffersize = " << GetSize() << std::endl << std::flush;
	for (int i = Message::_headerSize; i < GetSize() + Message::_headerSize; i++) {
		std::cout << dataBuf[i] << " "; }
	std::cout << std::endl << std::flush;

    int index = Message::_headerSize;
  //  std::cout << "HeaderSize " << Message::_headerSize << std::endl; 
    int * dataRef;

    // get string from size 8 int array
	/* 
    std::string temp;
	std::string temp2;
	int tempInt;
	std::string myString; 
	char character;
	char charArray[3];
    int stringIndex = 0; 
	int j  = 0;
	for(int i = Message::_headerSize; i < Message::_headerSize + 8; i++)
	{
        std::cout << "i " << i << std::endl; 
		std::cout << "In for loop" << std::endl; 
		temp = std::to_string(dataBuf[i]);
        std::cout << "temp " << temp << std::endl; 
		int tempIntArray[temp.length()];
		int elementSize = temp.length();
		int tempIndex = 0;

		while( stringIndex < temp.length())
		{
			std::cout << "In while loop " << temp[stringIndex] << std::endl; 
			if(temp[stringIndex] == '1')
			{
                std::cout << "first if " << std::endl; 
				temp2 = temp.substr(stringIndex, 3);

				std::cout << "my integer " << temp2 << std::endl; 
				tempInt = std::stoi(temp2);
				character = char (tempInt);
				myString = myString + character; 
				stringIndex = stringIndex + 3; 
			}
			else if(temp[i] == '0')
			{
                std::cout << "found a  null" << std::endl; 
				break;
			}
			else
			{
                std::cout << "else " << std::endl; 
				temp2 = temp.substr(stringIndex, 2);
                std::cout << "after" << std::endl; 
				tempInt = std::stoi(temp2);
				character = char (tempInt);
				myString = myString + character;
				stringIndex = stringIndex + 2; 
			}
		}

		stringIndex = 0; 
	}
	_data = myString; 
	std::cout << "MY STRING " << _data << std::endl; 
*/
    // Continuing deserialization
     dataRef = (int*) (&_randomNum);
    for(int i = 0; i < sizeof(_randomNum)/sizeof(int); i++){
        dataRef[i] = dataBuf[index++];}
    dataRef = (int*) (&_character);
    for(int i = 0; i < sizeof(int)/sizeof(int); i++){
        dataRef[i] = dataBuf[index++];}
    dataRef = (int*) (&_secondNum);
    for(int i = 0; i < sizeof(_secondNum)/sizeof(int); i++){
        dataRef[i] = dataBuf[index++];}
}

int Message1::getRandomNum()
{
	return _randomNum;
}

char Message1::getCharacter()
{
	return _character;
}
    
double Message1::getSecondNum()
{
	return _secondNum; 
}