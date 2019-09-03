#ifndef ROSCOMM_H
#define ROSCOMM_H

///#pragma once
#include "BaseComm.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

class RosComm: public BaseComm
{
public:
	RosComm();
	~RosComm(); 
	void Setup();
	int SendPtoP(std::string message);
	int SendBd(std::string message);
	

protected:

private:
	
};

#endif
