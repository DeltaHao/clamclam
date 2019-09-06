#pragma once
//PaoChat 2.0
//2019.9.2  14：26

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

//已通过测试
int prepare() {
	// 1.请求版本协议
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);
	if ((LOBYTE(wsData.wVersion) != 2) || (HIBYTE(wsData.wVersion) != 2)) {
		cout << "请求版本协议失败" << endl;
		return -1;
	}

	cout << "请求版本协议成功" << endl;


	// 2.创建socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == SOCKET_ERROR) {
		cout << "创建socket失败" << endl;
		WSACleanup();
		return -2;
	}

	cout << "创建socket成功" << endl;

	// 3.创建ip,port
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, (void*)& addr.sin_addr.S_un.S_addr);
	addr.sin_port = htons(PORT);


	// 4. 连接服务器
	int c = connect(clientSocket, (sockaddr*)& addr, sizeof addr);
	if (c == -1) {
		cout << "服务器连接失败" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return -2;
	}
	cout << "服务器连接成功" << endl;
	return 0;
}

int Login(char* _name, char* _password) {
	/*
	作用：登录
	参数：用户名，密码
	返回值：0表示登录成功,1表示登录失败
	*/
	string name = _name;
	string password = _password;
	string logInfo;
	//获取用户名和密码
	//处理成合法格式
	logInfo = name + ";" + password;
	char* buff = (char*)logInfo.data();
	int len = strlen(buff);
	buff[len] = CHECKNUM;
	buff[len + 1] = 0;
	//与服务器通讯进行验证
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
	作用：注册
	参数：用户名，密码
	返回值：0表示注册成功
			1表示用户名已被占用，注册失败
	*/
	string name = _name;
	string password = _password;
	string logInfo;
	//获取用户名和密码
	//处理成合法格式
	logInfo = name + ";" + password;
	char* buff = (char*)logInfo.data();
	int len = strlen(buff);
	buff[len] = SIGNUPNUM;
	buff[len + 1] = 0;
	//与服务器通讯进行验证
	send(clientSocket, buff, strlen(buff), NULL);
	recv(clientSocket, buff, 1024, NULL);
	if (buff[0] == 100 && buff[1] == SIGNUPNUM) {
		//cout << "该用户名已被注册!" << endl;
		return -1;
	}
	else {
		//cout << "注册成功!" << endl;
		return 0;
	}
}

int choosePattern(int stutas) {
	/*
	作用：向服务器表明选择群聊或单聊
	参数：状态，为 1 表示群聊， 为 2 表示单聊
	返回值：0成功， -1 失败
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
	作用：向服务器发送消息
	参数：字符串，消息内容
	返回值：0成功， -1 失败
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
	作用：接受消息
	参数：无
	返回值：message内容， NULL表示无消息
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
	作用：添加好友
	参数：好友name
	返回值：0表示成功,1表示失败
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
	作用：接收服务器发来的好友列表
	参数：无
	返回值：格式为：“好友名;好友名;好友名;...”
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
	作用：向服务器发送选择的好友编号
	参数：好友编号
	返回值：0成功， -1 失败
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


//未通过测试
int logOut() {
	/*
	作用：向服务器发送下线信号
	参数：无
	返回值：0成功， -1 失败
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
