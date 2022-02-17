#pragma once
#include <iostream>     
#include <fstream>  
using namespace std;
//*****************************************our exception class for save and load function****************************
class Saveexception : public exception
{
public:
	const char* what() const throw()
	{
		return "failed to save";
	}
};
class Loadexception : public exception
{
public:
	const char* what() const throw()
	{
		return "failed to load";
	}
};
class Openexception : public exception
{
public:
	const char* what() const throw()
	{
		return "failed to open file";
	}
};