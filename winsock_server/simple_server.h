#pragma once
// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

class cSocket
{
public:
	cSocket();
	~cSocket();
	// ���� �ʱ�ȭ
	bool InitSocket();
	// ���� ���� ����
	void CloseSocket(SOCKET socketClose, bool bIsForce = false);
	// ������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ �����
	bool BindAndListen(int nBindPort);
	// ���� ��û�� �����ϰ� �޽����� �޾Ƽ� ó��
	bool StartServer();
	// ip, port �� �ش��ϴ� ������ ����
	bool Connect(const char * pszIP, int nPort);

private:
	// Ŭ���̾�Ʈ ������ ������ �����ϱ� ���� ����
	// ���������� Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET	socket_;
	// �������� Ŭ���̾�Ʈ�� ���� ��û�� ������ �Ŀ� Ŭ���̾�Ʈ�� ����Ǵ� ����
	SOCKET	socket_connect_;
	// ���� ����
	char	sz_socketbuf_[1024];
};

