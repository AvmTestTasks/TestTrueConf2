#pragma once

#include <vector>
#include <string>
#include "Socket.h"

using namespace std;

class ClientSocket : public Socket
{
public:

	void ConnectToServer(string ip_addr, int port);

	void ReadFile(string filename, vector<char>& buffer_filedata);

	int SendFile(string filename);

	void Send_parts(int sock, char *buffer, int len, int chunk_size);

	inline void Send_parts(int sock, char *buffer, int len) 
	{
		Send_parts(sock, buffer, len, len);
	}

	void Close();


	void ExtractFilename(string &filename);

};

