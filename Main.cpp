#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include<WinSock2.h>
#include <fstream>
#include<string>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	//����
	WSAData WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl; 
		exit(-1);
	}

	//������Ĺ �����
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0); //���� ������ �ʴ� �ܼ�Ʈ�� ����� �װ� �ڴ´�.
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "fail create Socket : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerSockAddr; //ip�� ��Ʈ�� ���´�.
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN)); //�ʱ�ȭ�Լ�
	ServerSockAddr.sin_family = PF_INET; //�������� ����
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//���ϴ�ip����
	ServerSockAddr.sin_port = htons(4000); //��Ʈ ����(htone = ��Ʈ��ũ ���)
	int Result = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));// ���� ��Ĺ�� ip, ��Ʈ����, (��Ĺ, ,������ ����)
	if (Result == SOCKET_ERROR)
	{
		cout << "fail bind : " << GetLastError() << endl;
		exit(-1);
	}

	//�ܺ��Է��� ��ٷ���
	Result = listen(ServerSocket, 0); //�����Գ�? �ߵ���
	if (Result == SOCKET_ERROR)
	{
		cout << "fail listen : " << GetLastError() << endl;
		exit(-1);
	}

	//���� �Ŀ� ���ο� ��Ʈ�� �����ؼ� Ŭ���̾�Ʈ ��Ĺ ��ȯ
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(SOCKADDR_IN));
	int ClientSockAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);// ���� ���� ������ ���� �� �ִ�.
	if (ClientSocket == SOCKET_ERROR)
	{
		cout << "fail accept : " << GetLastError() << endl;
		exit(-1);
	}

	//���� ���� <�ֱ�1>(���� �뷮�� ���� ��)
	string filePath = "logo.png";

	ifstream OpenFile(filePath, ifstream::binary); //�б��
	ofstream WriteFile("Test.png", ifstream::binary);//��¿�
	if (OpenFile.is_open())
	{
		OpenFile.seekg(0, OpenFile.end);
		int length = (int)OpenFile.tellg();
		OpenFile.seekg(0, OpenFile.beg);

		char* Buffer = new char[length];
		OpenFile.read(Buffer, length);

		send(ClientSocket, Buffer, length, 0);
		
		delete[] Buffer;
		Buffer = nullptr;
	}
	OpenFile.close();

	//���� ���� <�ֱ�2>(���� �뷮�� Ŭ ��)
	/*
	string filePath = "logo.png";

	FLIE* OpenFile;
	OpenFile = fopen(filePath.c_str(), "rb");
	if (OpenFile)
	{
		char* Buffer = new char[1025];
		int ReadSize = 0;
		do
		{
			int ReadSize = fread(Buffer, 1, 1024, OpenFile);
			send(ClientSocket, Buffer, ReadSize, 0);
		} while (!feof(OpenFile));

		delete[] Buffer;
		Buffer = nullptr;
	}
	fclose(OpenFile);*/

	closesocket(ServerSocket);
	closesocket(ClientSocket);

	WSACleanup();

	return 0;
}