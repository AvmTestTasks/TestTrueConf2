#pragma once
#include <string>
#include <vector>
#include "Socket.h"

using namespace std;

class ServerSocket : public Socket
{
public:
	
	void StartServer(int port);

	void Bind(int port);

	void Listen();

private:
	void CreateEmptyFile(string filename);

	void WriteFile(string filename, vector<char>& data_buf);

};

