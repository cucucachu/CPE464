#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <cerrno>
#include "exceptions.h"

#define BUFFER_SIZE 1024

using namespace std;

int main(int argc, char **argv) {
	int port;
	int socket_fd, open_socket;
	uint8_t *buffer;
	struct sockaddr_in address;
	
	try {
		if (argc > 2)
			throw ARG_EX;
			
		port = 0;
		
		if (argc == 2)
			stringstream(argv[1]) >> port;
	
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
		if (socket_fd == -1)
			throw SOCK_EX;
			
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
	
		if (bind(socket_fd, (const sockaddr*)&address, sizeof(address)) == -1)
			throw BIND_EX;
			
		listen(socket_fd, 1);
		
		open_socket = accept(socket_fd, NULL, 0);
		//close(socket_fd);
		
		buffer = (uint8_t *)calloc(BUFFER_SIZE, sizeof(uint8_t));
		
		while(true) {
			if (recv(open_socket, buffer, sizeof(buffer), 0) == 0)
				break;
			cout << buffer;
		}
		
		free(buffer);
	}
	catch(int ex) {
		if (ex == ARG_EX)
			cout << "server <port #>";
		else if (ex == SOCK_EX)
			cout << "Socket failed, errno "  << errno << "\n";
		else if (ex == BIND_EX)
			cout << "Failed to bind, errno "  << errno << "\n";
		else
			cout << "Unknown exception.\n";
	}
}
