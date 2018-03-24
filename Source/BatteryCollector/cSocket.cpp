#include "cSocket.h"
#include <iostream>


cSocket::cSocket()
{
	socket_ = INVALID_SOCKET;
	socket_connect_ = INVALID_SOCKET;
	ZeroMemory(sz_socketbuf_, 1024);
}

cSocket::~cSocket()
{
	// winsock 사용을 끝냄
	WSACleanup();
}

bool cSocket::InitSocket()
{
	WSADATA wsaData;
	// 윈속 버전을 2.2로 초기화
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// TCP 소켓 생성
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ == INVALID_SOCKET) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "socket initialize success." << std::endl;
	return true;
}

void cSocket::CloseSocket(SOCKET socketClose, bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER 로 설정
										// bIsFoce 가 true 이면 SO_LINGER, timeout = 0 으로 설정하여
										// 강제 종료 시킨다. 주의 : 데이터 손실이 있을 수 있음
	if (bIsForce == true) {
		stLinger.l_onoff = true;
		// socketClose 소켓의 데이터 송수신을 모두 중단
		shutdown(socketClose, SD_BOTH);
		// 소켓 옵션을 설정
		setsockopt(socketClose, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
		// 소켓 연결을 종료
		closesocket(socketClose);

		socketClose = INVALID_SOCKET;
	}
}

bool cSocket::BindAndListen(int nBindPort)
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
	// 서버 포트를 설정
	stServerAddr.sin_port = htons(nBindPort);
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 위에서 지정한 서버 주소 정보와 소켓을 연결
	int nRet = bind(socket_, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// 접속 요청을 받아들이기 위해 소켓을 등록하고 접속 대기큐를 5로 설정
	nRet = listen(socket_, 5);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "server enroll success." << std::endl;
	return true;
}

bool cSocket::Connect(const char * pszIP, int nPort)
{
	// 접속할 서버 정보를 저장할 구조체
	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// 접속할 서버 포트 및 IP
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);

	int nRet = connect(socket_, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Connection success..." << std::endl;
	
	return true;
}

bool cSocket::SendLocation(float x, float y, float z)
{
	char		szOutMsg[1024];
	sprintf_s(szOutMsg, "배터리 생성 위치 X = %f, Y = %f, Z = %f", x, y, z);

	int nSendLen = send(socket_, szOutMsg, strlen(szOutMsg), 0);

	if (nSendLen == -1) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

struct location
{
	float x;
	float y;
	float z;
};

FVector cSocket::RequestLocation()
{
	char		szOutMsg[1024];
	location loc;
	strcpy_s(szOutMsg, "req_location");

	int nSendLen = send(socket_, szOutMsg, strlen(szOutMsg), 0);

	if (nSendLen == -1) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return FVector();
	}

	int nRecvLen = recv(socket_, (char*)&loc, 1024, 0);

	FVector result(loc.x, loc.y, loc.z);

	return result;
}


