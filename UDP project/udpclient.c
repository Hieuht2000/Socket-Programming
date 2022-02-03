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
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == SOCKET_ERROR) {
		printf("Socket cannot created\n"); return 1;
	}
	// prepare remote (server) address
	SOCKADDR_IN sa;   //Server address
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	sa.sin_port = htons(atoi(argv[2]));
	while (1) { // repeat as needed, when the user enters "EXIT", the loop stops
		char sbuf[100];
		printf("Message>> ");
		fgets(sbuf, 100, stdin);
		ret = sendto(s, sbuf, strlen(sbuf), 0, (SOCKADDR*)&sa, sizeof(sa));
		if (ret == SOCKET_ERROR) {
			printf("Sending is failed\n"); return 1;
		}
		// wait for the server to reply
		int salen = sizeof(sa);
		memset(sbuf, 0, 100);
		ret = recvfrom(s, sbuf, 100, 0, (SOCKADDR*)&sa, &salen);
		if (ret > 0) {
			printf("Server:: %s\n", sbuf);
		}
	}

}