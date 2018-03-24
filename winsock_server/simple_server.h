#pragma once
// winsock2 사용을 위해 아래 코멘트 추가
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

class cSocket
{
public:
	cSocket();
	~cSocket();
	// 소켓 초기화
	bool InitSocket();
	// 소켓 연결 종료
	void CloseSocket(SOCKET socketClose, bool bIsForce = false);
	// 서버의 주소정보를 소켓과 연결시키고 접속 요청을 받기 위해 그 소켓을 등록함
	bool BindAndListen(int nBindPort);
	// 접속 요청을 수락하고 메시지를 받아서 처리
	bool StartServer();
	// ip, port 에 해당하는 서버로 접속
	bool Connect(const char * pszIP, int nPort);

private:
	// 클라이언트 에서는 서버에 연결하기 위한 소켓
	// 서버에서는 클라이언트의 접속요청을 기다리는 소켓
	SOCKET	socket_;
	// 서버에서 클라이언트의 접속 요청을 수락한 후에 클라이언트와 연결되는 소켓
	SOCKET	socket_connect_;
	// 소켓 버퍼
	char	sz_socketbuf_[1024];
};

