#include <stdio.h>
#include <stdlib.h>
//#include <cstring.h>
#include <string.h>
//structure
#include <netinet/in.h>

//socket
#include <sys/socket.h> 
#include <sys/types.h>



//perror
#include <errno.h>



int main(int argc, char const *argv[])
{
	int sock, client;
	struct sockaddr_in server_addr, client_addr;
	unsigned int len;
	char mesg[] = "Hello to the world of socket!";

	//Socket error checking
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket not found");
		exit(-1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(10000);    //htons : host to network short
	server_addr.sin_addr.s_addr = INADDR_ANY;   //INADDR_ANY binds the socket to all available interfaces.
	bzero(&server_addr.sin_zero, 8);

	//bind:      bind(int socket, const struct sockaddr *address, socklen_t address_len);

	len = sizeof(struct sockaddr_in);

	if (bind(sock, (struct sockaddr*) &server_addr, len) == -1)
	{
		perror("bind not found");
		exit(-1);
	} 



	if ((listen(sock, 5)) == -1)
	{
		perror("listen not found");
		exit(-1);
	}

	while(1)
	{
		if((client = accept(sock, (struct sockaddr *)&client, &len)) == -1)
		{
			perror("accept");
			exit(-1);
		}

		send(client, mesg ,strlen(mesg), 0);
		printf("send %d bytes to client %s\n", send, inet_ntoa(client_addr.sin_addr));

		close(client);
	}


	}




//	return 0;
