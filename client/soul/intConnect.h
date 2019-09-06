#pragma once
//PaoChat 2.0
//2019.9.2  14��26

#include<afxwin.h>
#include<iostream>
#include<WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
#define IP "192.168.43.189"
#define PORT 12000
#define CHECKNUM 1
#define MESSAGENUM 2
#define SIGNUPNUM 3
#define LOGOUTNUM 4
#define CHOOSENUM 5
#define ADDFRIENDNUM 6
#define CHOOSEFRIENDNUM 7

//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS


SOCKET clientSocket;

//��ͨ������
int prepare() {
	// 1.����汾Э��
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);
	if ((LOBYTE(wsData.wVersion) != 2) || (HIBYTE(wsData.wVersion) != 2)) {
		cout << "����汾Э��ʧ��" << endl;
		return -1;
	}

	cout << "����汾Э��ɹ�" << endl;


	// 2.����socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == SOCKET_ERROR) {
		cout << "����socketʧ��" << endl;
		WSACleanup();
		return -2;
	}

	cout << "����socket�ɹ�" << endl;

	// 3.����ip,port
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, (void*)& addr.sin_addr.S_un.S_addr);
	addr.sin_port = htons(PORT);


	// 4. ���ӷ�����
	int c = connect(clientSocket, (sockaddr*)& addr, sizeof addr);
	if (c == -1) {
		cout << "����������ʧ��" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return -2;
	}
	cout << "���������ӳɹ�" << endl;
	return 0;
}

int Login(char* _name, char* _password) {
	/*
	���ã���¼
	�������û���������
	����ֵ��0��ʾ��¼�ɹ�,1��ʾ��¼ʧ��
	*/
	string name = _name;
	string password = _password;
	string logInfo;
	//��ȡ�û���������
	//����ɺϷ���ʽ
	logInfo = name + ";" + password;
	char* buff = (char*)logInfo.data();
	int len = strlen(buff);
	buff[len] = CHECKNUM;
	buff[len + 1] = 0;
	//�������ͨѶ������֤
	send(clientSocket, buff, strlen(buff), NULL);
	recv(clientSocket, buff, 1024, NULL);

	if (buff[0] == 100 && buff[1] == CHECKNUM) {
		return -1;
	}
	else {
		return 0;
	}
}

int Register(char* _name, char* _password) {
	/*
	���ã�ע��
	�������û���������
	����ֵ��0��ʾע��ɹ�
			1��ʾ�û����ѱ�ռ�ã�ע��ʧ��
	*/
	string name = _name;
	string password = _password;
	string logInfo;
	//��ȡ�û���������
	//����ɺϷ���ʽ
	logInfo = name + ";" + password;
	char* buff = (char*)logInfo.data();
	int len = strlen(buff);
	buff[len] = SIGNUPNUM;
	buff[len + 1] = 0;
	//�������ͨѶ������֤
	send(clientSocket, buff, strlen(buff), NULL);
	recv(clientSocket, buff, 1024, NULL);
	if (buff[0] == 100 && buff[1] == SIGNUPNUM) {
		//cout << "���û����ѱ�ע��!" << endl;
		return -1;
	}
	else {
		//cout << "ע��ɹ�!" << endl;
		return 0;
	}
}

int choosePattern(int stutas) {
	/*
	���ã������������ѡ��Ⱥ�Ļ���
	������״̬��Ϊ 1 ��ʾȺ�ģ� Ϊ 2 ��ʾ����
	����ֵ��0�ɹ��� -1 ʧ��
	*/
	char buff[3];
	memset(buff, 0, sizeof(buff));
	buff[0] = stutas + '0';
	buff[1] = CHOOSENUM;
	buff[2] = 0;
	int s = send(clientSocket, buff, strlen(buff), NULL);
	if (s == -1) return -1;
	else return 0;
}

int sendMessage(char* buff) {
	/*
	���ã��������������Ϣ
	�������ַ�������Ϣ����
	����ֵ��0�ɹ��� -1 ʧ��
	*/
	int len = strlen(buff);
	buff[len] = MESSAGENUM;
	buff[len + 1] = 0;
	int s = send(clientSocket, buff, len + 1, NULL);
	if (s == -1) {
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	return 0;
}

string receiveMessage() {
	/*
	���ã�������Ϣ
	��������
	����ֵ��message���ݣ� NULL��ʾ����Ϣ
	*/

	char Buff[1024];
	int r = recv(clientSocket, Buff, 1023, NULL);
	if (r > 0 && Buff[r - 1] == MESSAGENUM) {
		Buff[r - 1] = 0;
		//
		return Buff;
	}
	return NULL;
}
int addFriend(char* name) {
	/*
	���ã���Ӻ���
	����������name
	����ֵ��0��ʾ�ɹ�,1��ʾʧ��
	*/
	char* buff = name;
	int len = strlen(buff);
	buff[len] = ADDFRIENDNUM;
	buff[len + 1] = 0;
	int s = send(clientSocket, buff, len + 1, NULL);
	if (s == -1) {
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	return 0;
}
string receiveFriendList() {
	/*
	���ã����շ����������ĺ����б�
	��������
	����ֵ����ʽΪ����������;������;������;...��
	*/
	char buff[1024];
	int r = recv(clientSocket, buff, 1023, NULL);
	if (r > 0 && buff[r - 1] == CHOOSEFRIENDNUM) {
		buff[r - 1] = 0;
		//cout <<"!!" << buff << endl;
		return buff;
	}
	return NULL;
}
int chooseFriend(int num) {
	/*
	���ã������������ѡ��ĺ��ѱ��
	���������ѱ��
	����ֵ��0�ɹ��� -1 ʧ��
	*/
	char buff[3];
	buff[0] = num + '0';
	buff[1] = CHOOSEFRIENDNUM;
	buff[2] = 0;
	int s = send(clientSocket, buff, strlen(buff), NULL);
	if (s == -1) {
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	return 0;
}


//δͨ������
int logOut() {
	/*
	���ã�����������������ź�
	��������
	����ֵ��0�ɹ��� -1 ʧ��
	*/
	char buff[2];
	buff[0] = MESSAGENUM;
	buff[1] = 0;
	int s = send(clientSocket, buff, strlen(buff), NULL);
	if (s == -1) {
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	return 0;
}
