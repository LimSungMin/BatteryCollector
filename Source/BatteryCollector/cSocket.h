#pragma once
// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include "Runtime/Core/Public/Math/Vector.h"

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
	// ip, port �� �ش��ϴ� ������ ����
	bool Connect(const char * pszIP, int nPort);
	// ��ġ �۽�
	bool SendLocation(float x, float y, float z);
	// ��ġ ��û
	FVector RequestLocation();

private:
	// Ŭ���̾�Ʈ ������ ������ �����ϱ� ���� ����
	// ���������� Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET	socket_;
	// �������� Ŭ���̾�Ʈ�� ���� ��û�� ������ �Ŀ� Ŭ���̾�Ʈ�� ����Ǵ� ����
	SOCKET	socket_connect_;
	// ���� ����
	char	sz_socketbuf_[1024];
};

