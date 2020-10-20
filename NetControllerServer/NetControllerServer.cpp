// NetControllerServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PORT 23513
#define BUFFER_SIZE 1024


int main()
{
    SOCKET s;
    SOCKADDR_IN server, si_other;
    int slen = sizeof(si_other);
    int recvlen;
    char buf[BUFFER_SIZE];
    WSADATA wsaData;
    bool connectionSuccess;

    //Initialize winsock
    printf("Server: Initializing server at port %d...\n", PORT);
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Server: WSAStartup failed with error (%ld)\n", WSAGetLastError());
        connectionSuccess = false;
        return -1;
    }
    else {
        printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);
    }
    printf("Server: Winsock Initialized!\n");

    // Create a Socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET)
    {
        printf("Server: Error at socket() (%ld)\n", WSAGetLastError());
        WSACleanup();
        connectionSuccess = false;
        return -1;
    }
    else
    {
        connectionSuccess = true;
        printf("Server: socket() is OK!\n");
    }

    // Prepare the sockaddr_in structure
    printf("Server: Preparing server structure...");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Server: Bind failed with error code (%d)", WSAGetLastError());
        return -1;
    }
    printf("Server: Bind Completed!");

    // Listen for data
    while (true)
    {
        fflush(stdout);

        // Clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFFER_SIZE);
        
        // try to receeive some data,  this is a blocking call
        if ((recvlen = recvfrom(s, buf, BUFFER_SIZE, 0, (SOCKADDR*)&si_other, &slen)) == SOCKET_ERROR)
        {
            printf("SERVER: recvfrom() failed with error code (%d)", WSAGetLastError());
        }

        printf("Received Packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        // REPLY CLIENT
        /*if (sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }*/
    }

    closesocket(s);
    WSACleanup();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
