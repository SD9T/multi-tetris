#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void error(const char* msg);

int main(void)
{
	SOCKET sockClient;
	WSADATA wsaData;
	struct sockaddr_in sin;
	char msg[9999];
	int recvRetValue;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockClient = socket(AF_INET, SOCK_STREAM, 0);

	if (sockClient == INVALID_SOCKET)
	{
		error("socket");
		return -1;
	}
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr.s_addr);
	sin.sin_port = htons(10000);
	if (connect(sockClient, (struct sockaddr*) & sin, sizeof(sin)) != 0)
	{
		closesocket(sockClient);
		error("connect");
		return -1;
	}
	puts("IP = 127.0.0.1 PORT = 10000 connect!");
	for (;;)
	{
		memset(msg, 0, sizeof(msg));
		puts("hi!");
		recvRetValue = recv(sockClient, msg, sizeof(msg), 0);
		if (recvRetValue == SOCKET_ERROR)
		{
			puts("hi!");
			goto error;
		}
		if (recvRetValue == 0)
		{
			puts("hi");
			goto normal;
		}
		puts("hi!");
		gets(msg);
		send(sockClient, msg, recvRetValue, 0);
		puts(msg);
	}
error:
	puts("hi!");
	closesocket(sockClient);
	WSACleanup();
	return -1;
normal:
	closesocket(sockClient);
	WSACleanup();
	return 0;
}

void error(const char* msg)
{
	printf("%d %s error\n", WSAGetLastError(), msg);
	WSACleanup();
}