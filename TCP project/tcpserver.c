#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <winsock2.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Help: %s [port]\n", argv[0]); return 1;
	}
	WSADATA w;
	int ret = WSAStartup(0x00020002, &w);
	if (ret != 0) {
		printf("Winsock API cannot start\n"); return 1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == SOCKET_ERROR) return 1;
	// prepare local socket address
	SOCKADDR_IN sa, ca;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[1]));
	sa.sin_addr.s_addr = INADDR_ANY;
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) {
		printf("Binding failed, please check the port\n"); return 1;
	}
	ret = listen(s, 5);
	if (ret == SOCKET_ERROR) {
		printf("Listen failed\n"); return 1;
	}
	printf("TCP server is ready\n"); 
	while(1) { // the server is waiting for the incoming connections
		SOCKET cs = SOCKET_ERROR;
		int calen = sizeof(ca);
		while(cs== SOCKET_ERROR) cs	= accept(s, (SOCKADDR*)&ca, &calen);
		printf("A client %s:%d comes in\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
		// communicate with this client
		char msg[] = "This is not the right guy u can play with\n";
		for (int i = 0; i < 10; i++)
			send(cs, msg, strlen(msg), 0);
		closesocket(cs);
	}
}
