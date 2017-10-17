#include "ClientSocket.h"
#include <vector>
#include <string>
#include <iostream>
#include  <io.h>

#define port	1100

using namespace std;

int main(int argc, char* argv[]) {

	std::string server_addr;
	std::string filename;

	//Get file name and server address
	if (argc != 3) 
	{
		cerr << "Two argument must be provided, example: fileName.txt 192.168.1.165" << endl;
		
		system("pause");
		exit(EXIT_FAILURE);
	}
	else 
	{
		
		if ((_access(argv[1], 0)) == -1) 
		{
			cerr << "File doesn't exist." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}

		
		filename = argv[1];
		server_addr = argv[2];
		
	}

	ClientSocket client_socket;
	client_socket.ConnectToServer(server_addr, port);
	client_socket.SendFile(filename);

	cout <<"Success,"<< filename << " is sent to " << server_addr << "!" << endl;
	system("pause");
	return 0;
}