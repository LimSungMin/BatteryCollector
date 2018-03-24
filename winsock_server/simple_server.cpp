#include "stdafx.h"
#include <iostream>

#include "simple_server.h"


cSocket::cSocket()
{
	socket_ = INVALID_SOCKET;
	socket_connect_ = INVALID_SOCKET;
	ZeroMemory(sz_socketbuf_, 1024);
}

cSocket::~cSocket()
{
	// winsock ����� ����
	WSACleanup();
}

bool cSocket::InitSocket()
{
	WSADATA wsaData;
	// ���� ������ 2.2�� �ʱ�ȭ
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// TCP ���� ����
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
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER �� ����
										// bIsFoce �� true �̸� SO_LINGER, timeout = 0 ���� �����Ͽ�
										// ���� ���� ��Ų��. ���� : ������ �ս��� ���� �� ����
	if (bIsForce == true) {
		stLinger.l_onoff = true;
		// socketClose ������ ������ �ۼ����� ��� �ߴ�
		shutdown(socketClose, SD_BOTH);
		// ���� �ɼ��� ����
		setsockopt(socketClose, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
		// ���� ������ ����
		closesocket(socketClose);

		socketClose = INVALID_SOCKET;
	}
}

bool cSocket::BindAndListen(int nBindPort)
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
	// ���� ��Ʈ�� ����
	stServerAddr.sin_port = htons(nBindPort);
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ������ ������ ���� �ּ� ������ ������ ����
	int nRet = bind(socket_, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// ���� ��û�� �޾Ƶ��̱� ���� ������ ����ϰ� ���� ���ť�� 5�� ����
	nRet = listen(socket_, 5);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "server enroll success." << std::endl;
	return true;
}
struct location
{
	float x;
	float y;
	float z;
};
bool cSocket::StartServer()
{
	char szOutStr[1024];
	// ���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	SOCKADDR_IN	stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	std::cout << "Server Starting..." << std::endl;

	// Ŭ���̾�Ʈ ���� ��û�� ���� �� ���� ���
	socket_connect_ = accept(socket_, (SOCKADDR*)&stClientAddr, &nAddrLen);
	if (socket_connect_ == INVALID_SOCKET) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	sprintf_s(szOutStr, "Client Connect : IP(%s) SOCKET(%d)",
		inet_ntoa(stClientAddr.sin_addr), socket_connect_);
	std::cout << szOutStr << std::endl;

	// Ŭ���̾�Ʈ���� �޽����� ���� �ٽ� Ŭ���̾�Ʈ���� ����
	while (true) {
		int nRecvLen = recv(socket_connect_, sz_socketbuf_, 1024, 0);
		if (nRecvLen == 0) {
			std::cout << "client connection has been closed" << std::endl;
			CloseSocket(socket_connect_);

			// �ٽ� ������ ������ ���� ��û�� ����
			StartServer();
			return false;
		}
		else if (nRecvLen == -1) {
			std::cout << "Error : " << WSAGetLastError() << std::endl;
			CloseSocket(socket_connect_);

			// �ٽ� ������ ������ ���� ��û�� ����
			StartServer();
			return false;
		}

		sz_socketbuf_[nRecvLen] = NULL;
		std::cout << "message received : bytes[" << nRecvLen << "], message : ["
			<< sz_socketbuf_ << "]" << std::endl;

		if (!strcmp(sz_socketbuf_, "req_location")) {
			location loc;
			loc.x = 2000 + (std::rand() % (600 + 1));
			loc.y = (std::rand() % (500 + 1));
			loc.z = 500 + (std::rand() % (150 + 1));

			int nSendLen = send(socket_connect_, (char*)&loc, sizeof(loc), 0);

			std::cout << "���͸� ��ǥ ���� : X = " << loc.x <<
				", Y = " << loc.y << ", Z = " << loc.z << std::endl;
		}
	}

	// Ŭ���̾�Ʈ ���� ����
	CloseSocket(socket_connect_);
	// ���� ���� ���� ����
	CloseSocket(socket_);

	std::cout << "Server has been closed..." << std::endl;
	return true;
}

bool cSocket::Connect(const char * pszIP, int nPort)
{
	// ������ ���� ������ ������ ����ü
	SOCKADDR_IN stServerAddr;

	char		szOutMsg[1024];
	stServerAddr.sin_family = AF_INET;
	// ������ ���� ��Ʈ �� IP
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);

	int nRet = connect(socket_, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Connection success..." << std::endl;
	while (true) {
		std::cout << ">>";
		std::cin >> szOutMsg;
		if (_strcmpi(szOutMsg, "quit") == 0) break;

		int nSendLen = send(socket_, szOutMsg, strlen(szOutMsg), 0);

		if (nSendLen == -1) {
			std::cout << "Error : " << WSAGetLastError() << std::endl;
			return false;
		}

		std::cout << "Message sended : bytes[" << nSendLen << "], message : [" <<
			szOutMsg << "]" << std::endl;

		int nRecvLen = recv(socket_, sz_socketbuf_, 1024, 0);
		if (nRecvLen == 0) {
			std::cout << "Client connection has been closed" << std::endl;
			CloseSocket(socket_);
			return false;
		}
		else if (nRecvLen == -1) {
			std::cout << "Error : " << WSAGetLastError() << std::endl;
			CloseSocket(socket_);
			return false;
		}

		sz_socketbuf_[nRecvLen] = NULL;
		std::cout << "Message received : bytes[" << nRecvLen << "], message : [" <<
			sz_socketbuf_ << "]" << std::endl;
	}

	CloseSocket(socket_);
	std::cout << "Client has been terminated..." << std::endl;
	return true;
}


