#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include<WinSock2.h>
#include <fstream>
#include<string>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	//시작
	WSAData WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl; 
		exit(-1);
	}

	//서버소캣 만들기
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0); //눈에 보이지 않는 콘센트를 만든다 그걸 박는다.
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "fail create Socket : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerSockAddr; //ip와 포트를 적는다.
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN)); //초기화함수
	ServerSockAddr.sin_family = PF_INET; //프로토콜 설정
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//원하는ip설정
	ServerSockAddr.sin_port = htons(4000); //포트 설정(htone = 네트워크 모드)
	int Result = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));// 가상 소캣과 ip, 포트연결, (소캣, ,데이터 길이)
	if (Result == SOCKET_ERROR)
	{
		cout << "fail bind : " << GetLastError() << endl;
		exit(-1);
	}

	//외부입력을 기다려라
	Result = listen(ServerSocket, 0); //누가왔냐? 잘들어봐
	if (Result == SOCKET_ERROR)
	{
		cout << "fail listen : " << GetLastError() << endl;
		exit(-1);
	}

	//수락 후에 새로운 포트를 연결해서 클라이언트 소캣 반환
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(SOCKADDR_IN));
	int ClientSockAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);// 들어온 놈의 정보를 받을 수 있다.
	if (ClientSocket == SOCKET_ERROR)
	{
		cout << "fail accept : " << GetLastError() << endl;
		exit(-1);
	}

	//파일 정보 <주기1>(파일 용량이 작을 때)
	string filePath = "logo.png";

	ifstream OpenFile(filePath, ifstream::binary); //읽기용
	ofstream WriteFile("Test.png", ifstream::binary);//출력용
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

	//파일 정보 <주기2>(파일 용량이 클 때)
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