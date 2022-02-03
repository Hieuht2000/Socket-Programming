#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <WinSock2.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Help: %s [ip] [port]\n", argv[0]); return 1;
	}
	WSADATA w;
	int ret = WSAStartup(0x00020002, &w);
	if (ret != 0) {
		printf("WinSock API is not ready\n"); return 1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR) {
		printf("Socket cannot created\n"); return 1;
	}
	// prepare remote (socket) address
	SOCKADDR_IN sa;  
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(atoi(argv[2]));
	ret = connect(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) {
		printf("Connection failed, please check your Internet\n"); return 1;
	}
	//while (1) is not necessary because our server will just send sth
	char rbuf[10000];
	memset (rbuf, 0, 10000);
	ret = recv(s, rbuf, 10000, 0);
	if (ret > 0) {
		printf("%d bytes are received\nMessage is: %s\n", ret, rbuf);
	}
	closesocket(s);
	WSACleanup();
}