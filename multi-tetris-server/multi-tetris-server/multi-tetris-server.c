#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define CLIS_NUMBER 2
#define MSG_SIZE 9999
#define CLIA_TO_CLIB(i) (i != 0 ? 0: 1)

void error(const char* msg);

void main()
{
	SOCKET s, cs[CLIS_NUMBER];
	SOCKADDR_IN server, client[CLIS_NUMBER];
	WSADATA wsaData;
	int recvRetValue[CLIS_NUMBER], clientSize[CLIS_NUMBER], i;
	char buffer[MSG_SIZE];

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
		error("socket");
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = htonl(ADDR_ANY);
	if (bind(s, (SOCKADDR*)& server, sizeof(server)) == SOCKET_ERROR)
		error("bind");
	if (listen(s, SOMAXCONN) != 0)
		error("listen");

	puts("hi!");

	for (i = 0; i < CLIS_NUMBER; i++)
	{
		printf("%d\n", i);
		clientSize[i] = sizeof(client[i]);
		printf("%d\n", i);
		cs[0] = accept(s, (SOCKADDR*)& client[0], &clientSize[0]);
		printf("%d\n", i);
		if (cs[i] == INVALID_SOCKET)
			error("accept");
		printf("%d\n", i);
	}

	puts("Clients log...");
	for (;;)
	{
		for (i = 0; i < CLIS_NUMBER; i++)
		{
			memset(&buffer[i], 0, MSG_SIZE);
			recvRetValue[i] = recv(cs[i], &buffer[i], MSG_SIZE, 0);
			if (recvRetValue[i] == 0 || recvRetValue[i] == SOCKET_ERROR)
				goto exit;
			puts(buffer);
			send(cs[i], &buffer[CLIA_TO_CLIB(i)], recvRetValue[CLIA_TO_CLIB(i)], 0);
		}
	}
exit:
	closesocket(s);
	WSACleanup();
	exit(0);
}

void error(const char* msg)
{
	printf("%d %s error\n", WSAGetLastError(), msg);
	WSACleanup();
	exit(0);
}