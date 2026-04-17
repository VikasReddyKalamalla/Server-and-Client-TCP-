// TCP Client Program (Windows - Winsock)

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[1000], reply[1000];
    int recv_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    // Server details
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5050);

    // Connect to server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        closesocket(s);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    while (1) {
        printf("Enter message: ");

        // Safe input
        fgets(message, sizeof(message), stdin);

        // Remove newline from fgets
        message[strcspn(message, "\n")] = '\0';

        // Send message
        send(s, message, strlen(message), 0);

        // Receive reply
        recv_size = recv(s, reply, sizeof(reply) - 1, 0);
        if (recv_size > 0) {
            reply[recv_size] = '\0';
            printf("Server: %s\n", reply);
        } else {
            printf("Server disconnected\n");
            break;
        }
    }

    // Cleanup
    closesocket(s);
    WSACleanup();

    return 0;
}