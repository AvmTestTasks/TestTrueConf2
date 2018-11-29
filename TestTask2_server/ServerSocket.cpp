#include "ServerSocket.h"
#include "Socket.h"
#include "Exception.h"
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>



void ServerSocket::Bind(int port)
{
	//cleanup and initialize addres structure
	memset(&stSockAddr, 0, sizeof(stSockAddr));

	stSockAddr.sin_family = PF_INET;
	stSockAddr.sin_port = htons(port);
	stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind to port
	try 
	{
		if (bind(SocketFD, (struct sockaddr*) &stSockAddr, sizeof(stSockAddr)) == -1) 
		{
			throw Exception(10, "ServerSocket: Failed to bind");
		}
	}
	catch (Exception& excp) 
	{
		excp.Handle(*this);
		exit(EXIT_FAILURE);
	}
}

void ServerSocket::Listen()
{
	try {
		//start listening
		if (listen(SocketFD, 10) == -1) 
		{
			throw Exception(11, "ServerSocket: Failed to start listening");
		}

		//wait for incoming connections
		for (;;)
		{
			//block until an incoming connection is received
			cout << "Server launched, waiting for connection..." << endl;
			int ConnectFD = accept(SocketFD, 0, 0);
			cout << "Connection accepeted." << endl;

			if (ConnectFD < 0) 
			{
				throw Exception(12, "ServerSocket: Failed to accept connection");
			}

			//receive file length
			size_t length_received = 0;
			size_t messageLength;
			while (length_received < length_maxlen)
			{

				int length_read = recv(ConnectFD, (char *)&messageLength + length_received,
						       length_maxlen - length_received, 0);
				if (length_read == SOCKET_ERROR)
				{
					int er_code = WSAGetLastError();
					throw Exception(er_code, "ServerSocket: Receiving data length failed.");
				}
				length_received += length_read;
			}
			messageLength = ntohl(messageLength);

			//receive file name
			size_t filename_received = 0;
			stringstream filename_ss;
			string filename;

			while (filename_received < filename_maxlen)
			{
				vector<char> filename_buf(filename_maxlen);
				int filename_read = recv(ConnectFD, filename_buf.data() + filename_received,
							 filename_maxlen - filename_received, 0);
				if ((filename_read == SOCKET_ERROR) || (filename_read == 0))
				{
					int er_code = WSAGetLastError();
					throw Exception(er_code, "ServerSocket: Receiving filename failed.");
				}
				filename_received += filename_read;
				filename_ss << filename_buf.data();
			}
			filename_ss << '\0';
			filename = filename_ss.str();

			//if file is empty
			if (messageLength == 0)
			{
				CreateEmptyFile(filename);
				cout << "File received." << endl;

				shutdown(ConnectFD, 2);
				cout << "Connection closed." << endl;

				continue;
			}

			//receive file data
			size_t filedata_received = 0;
			while (filedata_received < messageLength) 
			{
				vector<char> filedata_buf(messageLength);
				int filedata_read = 0;
				if (filedata_received == 0) {
					CreateEmptyFile(filename);
				}
				filedata_read = recv(ConnectFD, filedata_buf.data(), chunk_maxlen, 0);
				if ((filedata_read == SOCKET_ERROR) || (filedata_read == 0)) {
					int er_code = WSAGetLastError();
					throw Exception(er_code, "ServerSocket: Receiving file data failed.");
				}
				else {
					filedata_received += filedata_read;
					WriteFile(filename, filedata_buf);
				}
			}
			cout << "File received." << endl;

			//close connection
			shutdown(ConnectFD, 2);
			cout << "Connection closed." << endl;
		}
	}
	catch (Exception& excp) 
	{
		excp.Handle(*this);
		exit(EXIT_FAILURE);
	}
}

void ServerSocket::StartServer(int port)
{
	Bind(port);
	Listen();
}

void ServerSocket::CreateEmptyFile(std::string filename)
{
	ofstream ofs;
	ofs.exceptions(ifstream::failbit | ifstream::badbit);
	try 
	{
		ofs.open(filename);
		ofs.clear();
		ofs.close();
	}
	catch (ofstream::failure e) 
	{
		Exception excp(18, "File creation failed");
		excp.Handle(*this);
	}
}

void ServerSocket::WriteFile(string filename, vector<char>& data_buf)
{
	ofstream ofs;
	ofs.exceptions(ifstream::failbit | ifstream::badbit);
	try 
	{
		ofs.open(filename, ios_base::app | ios_base::binary);
		if (ofs) 
		{
			ofs << data_buf.data();
			ofs.close();
		}
	}
	catch (ofstream::failure e) 
	{
		Exception excp(20, "Fail write received data to file.");
		excp.Handle(*this);
	}
}

