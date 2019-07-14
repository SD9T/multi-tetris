#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MSG_SIZE 9999

void error(const char* msg);

void main()
{
	SOCKET s, cs;
	SOCKADDR_IN server, client;
	WSADATA wsaData;
	int recvRetValue, clientSize;
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

	clientSize = sizeof(client);
	cs = accept(s, (SOCKADDR*)& client, &clientSize);
	if (cs == INVALID_SOCKET)
		error("accept");

	puts("Clients log...");
	for (;;)
	{
		memset(buffer, 0, MSG_SIZE);
		recvRetValue = recv(cs, buffer, MSG_SIZE, 0);
		if (recvRetValue == 0 || recvRetValue == SOCKET_ERROR)
			break;
		puts(buffer);
		send(cs, buffer, recvRetValue, 0);
	}
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