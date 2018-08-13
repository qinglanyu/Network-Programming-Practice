// TcpClient1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <WinSock2.h>

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

#define SVR_PORT 2345
#define SVR_IP	"192.168.68.131"

int _tmain(int argc, _TCHAR* argv[])
{
	// 0- initialize sock lib
	// Declare and initialize variables.
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	int sockfd;
	struct sockaddr_in addr;

	// 1- socket()
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket created error!");
		exit(-1);
	}
	else
	{
		printf("1- socket created successfully!\n");
		printf("socket id:%d\n", sockfd);
	}

	// 2- connect(): connect the local socket to a remote server(ip:port)
	ZeroMemory(&addr, sizeof(addr));	//bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SVR_PORT);
	addr.sin_addr.s_addr = inet_addr(SVR_IP);
	if (connect(sockfd, (struct sockaddr*)(&addr), sizeof(struct sockaddr)) < 0)
	{
		perror("connect error!");
		exit(-1);
	}
	else
	{
		printf("2- connected successfully!\n");
	}

	// 3- send()
	char sndbuf[] = "you are such a good man!";
	send(sockfd, sndbuf, sizeof(sndbuf), 0);

	// 4- recv()
	// Receive until the peer closes the connection
	char rcvbuf[200] = { 0 };
	do 
	{
		iResult = recv(sockfd, rcvbuf, sizeof(rcvbuf), 0);
		if (iResult > 0)
			printf("Bytes received: %d-%s\n", iResult, rcvbuf);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// 5- close()
	// cleanup
	closesocket(sockfd);


	return 0;
}

