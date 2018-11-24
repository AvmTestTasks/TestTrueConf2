#include "Exception.h"
#include <iostream>
#include <string>

Exception::Exception(int err_no, std::string err_msg) :	error_number(err_no), error_message(err_msg)
{

}

Exception::~Exception()
{
}

void Exception::Handle(Socket sock)
{
	cerr << "Error " << error_number << ": " << error_message << endl;
	system("pause");
}
