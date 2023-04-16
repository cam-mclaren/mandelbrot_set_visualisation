#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define SOCKET_PATH "/usr/project/sockets/socket.sock"



int main(void)
{
	int sock_struct_len;
	int return_code;
	int sock_file_descriptor;
	struct sockaddr_un client_sockaddr;
	char buf[30];
	memset(&client_sockaddr, 0 , sizeof(struct sockaddr_un));

	// Create the socket 
	sock_file_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock_file_descriptor < 0)
	{
		fprintf(stderr, "Error calling socket on line %d\n", (__LINE__-2));
		exit(1);
	}


	client_sockaddr.sun_family= AF_UNIX;
	strcpy(client_sockaddr.sun_path, SOCKET_PATH);
	sock_struct_len = sizeof(client_sockaddr);


	return_code = connect(sock_file_descriptor, (struct sockaddr*)&client_sockaddr, sock_struct_len);
	if (return_code == -1)
	{
		fprintf(stderr, "Error calling connect()\n");
		exit(1);
	}

	int i; 
	char msg[10];
	for (i = 0 ; i < 10000000; i ++)
	{
		memset(msg, 0, sizeof(msg));
		sprintf(msg,"%d\n", i);
		send(sock_file_descriptor, msg, strlen(msg), 0);
		printf("%d\n",i);
	}



	close(sock_file_descriptor);

	return 0;
}



