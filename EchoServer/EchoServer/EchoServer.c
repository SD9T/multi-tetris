#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void error(const char* msg);

int main(int argc, char *argv[])
{
	puts(argv[0]);
	SOCKET sockServer, sockClient;
	SOCKADDR_IN server, client;
	WSADATA wsaData;
	int clientSize, i, recvRetValue;
	char buffer[9999];

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);

	if (sockServer == INVALID_SOCKET)
	{
		error("socket");
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = htonl(ADDR_ANY);
	if (bind(sockServer, (SOCKADDR*)& server, sizeof(server)) == SOCKET_ERROR)
	{
		error("bind");
	}
	if (listen(sockServer, SOMAXCONN) != 0)
	{
		error("listen");
	}

	puts("클라이언트로 부터 접속 대기중...");
	// for (i = 0; i < 2; i++)
	{
		clientSize = sizeof(client);
		sockClient = accept(sockServer, (SOCKADDR*)& client, &clientSize);
		if (sockClient == INVALID_SOCKET)
		{
			error("socket");
		}
	}
	for (;;)
	{
		memset(buffer, 0, sizeof(buffer));
		// for (;;)
		recvRetValue = recv(sockClient, buffer, sizeof(buffer), 0);
		if (recvRetValue == SOCKET_ERROR)
			goto error;
	    if (recvRetValue == 0)
			goto normal;
		puts(buffer);
		send(sockClient, buffer, recvRetValue, 0);
	}
error:
	closesocket(sockServer);
	WSACleanup();
	return -1;
normal:
	closesocket(sockServer);
	WSACleanup();
	return 0;
}

void error(const char* msg)
{
	printf("%d %s error\n", WSAGetLastError(), msg);
	WSACleanup();
}