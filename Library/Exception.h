#pragma once
#include <string>
#include "Socket.h"

using namespace std;

class Exception
{
public:
	Exception(int err_no, string err_msg);
	~Exception();
	void Handle(Socket sock);

private:
	int error_number;
	string error_message;
};

