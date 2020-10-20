// NetControllerClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "main.h"
#include <stdio.h>
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PORT 23513
#define BUFFER_SIZE 1024

int main()
{
    // Socket Initialization
    WSADATA wsaData;
    SOCKADDR_IN ReceiverAddr;
    SOCKET SendSocket;
    std::string ipAdd;
    int port = -1;
    char ReceiveBuf[BUFFER_SIZE];
    int BufLength = BUFFER_SIZE;
    SOCKADDR_IN SenderAddr;
    int senderAddrSize = sizeof(SenderAddr);
    int ByteReceived = 5, SelectTiming, ErrorCode;
    bool connectionSuccess = false;

    // Controller Initialization
    bool wasConn = true;
    XController xcon;

    // User Request
    std::cout << "NetControllerClient v" << MAJOR_VERSION << "." << MINOR_VERSION << "\n";
    std::cout << "Host IP: ";
    std::cin >> ipAdd;
    std::cout << "Port: ";
    std::cin >> port;

    if (port == -1)
    {
        port = PORT;
    }
    while (ipAdd.size() == 0)
    {
        std::cout << "No Host IP supplied. Please input your host IP Address again: ";
        std::cin >> ipAdd;
    }

    std::cout << "Client: Conecting to " << ipAdd << ":" << port << "...";

    // Initialize Winsock version 2.2
    printf("\nClient: Initializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Client: WSAStartup failed with error %ld\n", WSAGetLastError());
        connectionSuccess = false;
        return -1;
    }
    else {
        printf("Client: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);
    }
    printf("Client: Winsock Initialized!\n");
    
    // Create a new socket to receive datagrams on.
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET)
    {
        printf("Client: Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        connectionSuccess = false;
        return -1;
    }
    else {
        connectionSuccess = true;
        printf("Client: socket() is OK!\n");
    }

    // Setup Address Structure
    memset((char*)&ReceiverAddr, 0, sizeof(ReceiverAddr));
    ReceiverAddr.sin_family = AF_INET;
    ReceiverAddr.sin_port = htons(port);
    ReceiverAddr.sin_addr.S_un.S_addr = inet_addr(ipAdd.c_str());
    printf("Client: Successfully created address structure.");

    // Start Communication
    while (true && connectionSuccess)
    {
        Sleep(LISTENER_RATE);
        if (!xcon.Refresh())
        {
            if (wasConn)
            {
                wasConn = false;
                std::cout << "Please connect your controller." << std::endl;
            }
        }
        else {
            if (!wasConn)
            {
                wasConn = true;
                std::cout << "Controller connected to port " << xcon.GetPort() << std::endl;
            }
            std::cout << "Left thumb stick: (" << xcon.leftStickX << ", " << xcon.leftStickY << ") Right thumb stick: (" << xcon.rightStickX << ", " << xcon.rightStickY << ")" << std::endl;
            std::cout << "Left analog trigger: " << xcon.leftTrigger << " Right analog trigger: " << xcon.rightTrigger << std::endl;
            
            // Listen for key presses here
            if (xcon.IsPressed(XINPUT_GAMEPAD_A))
            {
                sendKeyPress(xcon_keys::KEY_A, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_B))
            {
                sendKeyPress(xcon_keys::KEY_B, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_X))
            {
                sendKeyPress(xcon_keys::KEY_X, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_Y))
            {
                sendKeyPress(xcon_keys::KEY_Y, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_DPAD_UP))
            {
                sendKeyPress(xcon_keys::KEY_UP, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN))
            {
                sendKeyPress(xcon_keys::KEY_DOWN, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
            {
                sendKeyPress(xcon_keys::KEY_LEFT, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
            {
                sendKeyPress(xcon_keys::KEY_RIGHT, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER))
            {
                sendKeyPress(xcon_keys::KEY_LB, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER))
            {
                sendKeyPress(xcon_keys::KEY_RB, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.leftTrigger != 0.0f)
            {
                sendKeyPress(xcon_keys::KEY_LT, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.rightTrigger != 0.0f)
            {
                sendKeyPress(xcon_keys::KEY_RT, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_LEFT_THUMB))
            {
                sendKeyPress(xcon_keys::KEY_TOGGLE_L, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB))
            {
                sendKeyPress(xcon_keys::KEY_TOGGLE_R, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_START))
            {
                sendKeyPress(xcon_keys::KEY_PAUSE, SendSocket, ReceiverAddr, senderAddrSize);
            }
            if (xcon.IsPressed(XINPUT_GAMEPAD_BACK))
            {
                sendKeyPress(xcon_keys::KEY_SELECT, SendSocket, ReceiverAddr, senderAddrSize);
            }
        }
    }
    //connectHost(ipAdd, port);
    closesocket(SendSocket);
    WSACleanup();
    return 0;
}

void sendKeyPress(const char* k, SOCKET s, SOCKADDR_IN si_other, int slen)
{
    char* key = strdup(k);
    printf("Client: Key (%s) pressed!", key);
    if (sendto(s, key, strlen(key), 0, (SOCKADDR*)&si_other, slen) == SOCKET_ERROR)
    {
        printf("Client: sendto() failed with error code: %d", WSAGetLastError());
        return;
    }
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
