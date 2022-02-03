#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Help: %s [port]\n", argv[0]); return 1;
	}

	// The WSADATA structure contains information about the Windows Sockets implementation
	WSADATA w; // Windows needs to have this line while Linux doesn't 
	WORD ver = MAKEWORD(2, 2); // version 2.2
	int ret = WSAStartup(ver, &w);

	if (ret != 0) {
		printf("Winsock API is not ready\n"); return 1;
	}

	// Setup socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == SOCKET_ERROR) { printf("Socket cannot be created\n"); return 1; }

	SOCKADDR_IN la; // Server needs to know local address
	la.sin_family = AF_INET;
	la.sin_addr.s_addr = INADDR_ANY;
	la.sin_port = htons(atoi(argv[1]));

	// Assign the specified address strcuture to the socket
	ret = bind(s, (SOCKADDR*)&la, sizeof(la));

	if (ret == SOCKET_ERROR) { printf("Bind Failed\n"); return 1; }
	printf("UDP is ready to receive!\n");

	while (1) {
		char buf[100];
		memset(buf, 0, 100);	// Clean Garbage
		SOCKADDR_IN ca;
		int calen = sizeof(ca);
		ret = recvfrom(s, buf, 100, 0, (SOCKADDR*)&ca, &calen);

		if (ret > 0) {
			printf("Message (%s:%d): %s\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port), buf);
			sprintf(buf, "%s (%d)", buf, ret);
			ret = sendto(s, buf, strlen(buf), 0, (SOCKADDR*)&ca, calen);
			if (ret == SOCKET_ERROR) { printf("Sendto Failed\n"); return 1; }
		}
	}
	return 0;
}