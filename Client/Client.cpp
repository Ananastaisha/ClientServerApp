// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
// #include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#define PAUSE 10

int main(int argc, char** argv) // ��� ������� ��� ������� ����� ����� ������� ����� ��������� ��������� ������
{
    // � ��������� ����������� ��������� ������ ����������� ����������, ������� ���������� ���������� ��������� ������ �� �����������!
    // Validate the parameters
    //if (argc != 2) {
    //    printf("usage: %s server-name\n", argv[0]);
    //    return 10;
    //}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    setlocale(0, "");
    system("title CLIENT SIDE");
    cout << "������� ������� �������!\n";
    Sleep(PAUSE);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << "\n";
        return 11;
    }
    else {
        cout << "����������� Winsock.dll ������ �������!\n";
        Sleep(PAUSE);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    const char* ip = "localhost"; // �� ���������, ��� ����������, � ������, � ������, ����������� �� ����� � ��� �� ������

    //iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result); // �����������������, ���� ����� ����� ������ ��� ������� �� ��������� ������
    addrinfo* result = NULL;
    iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {
        cout << "getaddrinfo failed with error: " << iResult << "\n";
        WSACleanup();
        return 12;
    }
    else {
        cout << "��������� ������ � ����� ������� ������ �������!\n";
        Sleep(PAUSE);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Attempt to connect to an address until one succeeds
    SOCKET ConnectSocket = INVALID_SOCKET;

    for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) { // �������� ����� ���� ���������, ������� �� �������� ����

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << "\n";
            WSACleanup();
            return 13;
        }

        // Connect to server
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }

        cout << "�������� ������ �� ������� ������ �������!\n";
        Sleep(PAUSE);

        break;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "���������� ������������ � �������. ���������, ��� ������� ������� �������!\n";
        WSACleanup();
        return 14;
    }
    else {
        cout << "����������� � ������� ������ �������!\n";
        Sleep(PAUSE);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (true) {
        // Send an initial buffer
        char* message;
        message = new char[200];
        cout << "��������� �� ������� �������: ";
        cin.getline(message, 200);
        iResult = send(ConnectSocket, message, (int)strlen(message), 0);
        cout << "������ ������� ���������� �� ������: " << message << "\n";
        cout << "������ � ������� ����������: " << iResult << "\n";
        Sleep(PAUSE);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Receive until the peer closes the connection
        char answer[DEFAULT_BUFLEN];
        iResult = recv(ConnectSocket, answer, DEFAULT_BUFLEN, 0);
        answer[iResult] = '\0';

        if (iResult > 0) {
            cout << "������� ������� ������� �������� ���������: " << answer << "\n";
            cout << "������ ��������: " << iResult << "\n";
        }
        
    }
    

    // cleanup
    //closesocket(ConnectSocket);
    //WSACleanup();

    cout << "������� ������� ���������� ���� ������!\n";

    return 0;
}