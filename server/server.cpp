/*
PaoChat 1.5
server.cpp
修改日期 9/2/20:00
*/
#include<iostream>
#include<string>
#include <windows.h>
using namespace std;

#include "database.h"
#include "obj.h"
#pragma comment(lib, "ws2_32.lib")

#define IP "10.195.159.153"
#define PORT 12000

/*
每次send和recv的消息的最后一附加位代表消息类型
消息类型 | 编号
-----------
登录验证 | 1
真实消息 | 2
注册验证 | 3
下线信息 | 4
单聊群聊 | 5
添加好友 | 6
选择好友 | 7
*/
#define CHECKNUM 1
#define MESSAGENUM 2
#define SIGHUPNUM 3
#define LOGOUTNUM 4
#define CHOOSENUM 5
#define ADDFRIENDNUM 6
#define CHOOSEFRIENDNUM 7

#define WORNG_USER 100
//函数声明
int databaseInit();
int createUser();
int createFriends();
int insertUser(string, string, string);
int insertFriends(string, string);
int logInCheck(string, string);
int getUsers(User*);
int getFriends(Friends*);

//全局变量
SOCKET serverSocket;//服务端套接字
SOCKET clientSocket[100];//客户端套接字
static int concOrder = 0;//客户端连接的次序
static char usernames[1024][50];//连接成功的用户的用户名
static int usersnum;//数据库中用户的数量
User users[100];//从数据库中接收的用户信息
static int friendssnum;//数据库中好友对的数量
Friends friends[100];//从数据库中接收的好友对信息
char* userIP;

int prepare() {
	/*
	作用：前期准备，连接数据库，创建socket等
	参数：无；
	返回值：-1表示出错，直接退出；
	*/
	memset(users, 0, sizeof(users));
	//初始化数据库,建表
	databaseInit();
	createUser();
	createFriends();
	usersnum = getUsers(users);
	friendssnum = getFriends(friends);
	// 1.请求版本协议
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);
	if ((LOBYTE(wsData.wVersion) != 2) || (HIBYTE(wsData.wVersion) != 2)) {
		printf("------请求版本协议失败------\n");
		return -1;
	}
	printf("------请求版本协议成功------\n");


	// 2.创建socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		printf("创建socket失败\n");
		WSACleanup();
		return -2;
	}
	printf("------创建socket成功--------\n");


	// 3.创建ip,port
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);


	// 4.绑定ip,port给socket
	int b = bind(serverSocket, (sockaddr*)& addr, sizeof addr);
	if (b == -1) {
		printf("绑定ip，port失败\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	} 
	printf("------绑定ip，端口成功------\n");


	// 5. 监听
	int l = listen(serverSocket, 10);
	if (l == -1) {
		printf("监听失败\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("正在监听......\n");
	return 0;
}
void service(int idx) {
	/*
	作用：每有一个客户端连接，建立新的线程服务之
	参数：客户端连接序号
	返回值：无
	*/
	int flag = 1;
	while (flag) {
		char checkBuff[1024];
		int r = recv(clientSocket[idx], checkBuff, 1023, NULL);
		if (r > 0) {
			int type = checkBuff[r - 1];//决定登录还是注册
			checkBuff[r - 1] = 0;
			char name[1024];
			char password[1024];

			//将checkBuff中的内容分别存入name， password中
			char* p = password;
			int temp = 0;
			for (int i = 0; checkBuff[i]; i++) {
				if (temp)* (p++) = checkBuff[i];
				if (checkBuff[i] != ';') {
					name[i] = checkBuff[i];
				}
				else {
					name[i] = 0;
					temp = 1;
				}
			}
			*p = 0;


			if (type == CHECKNUM) {//登录
				//验证用户名和密码
				int userid = logInCheck(name, password);
				if (userid == -1) {
					cout << usernames[idx] << "登录失败！" << endl;
					checkBuff[0] = WORNG_USER;
				}
				else {
					flag = 0;
					cout << usernames[idx] << "登录成功！" << endl;
				}
				checkBuff[1] = CHECKNUM;
				checkBuff[2] = 0;
				send(clientSocket[idx], checkBuff, strlen(checkBuff), NULL);
			}
			else if (type == SIGHUPNUM) {//注册
				//在数据库中查询
				int userid = logInCheck(name, password);
				if (userid != -1) {//如果已经有该用户
					checkBuff[0] = WORNG_USER;
					cout << usernames[idx] << "注册失败！" << endl;
				}
				else {
					//向数据库插入新成员
					insertUser(userIP, name, password);
					//更新本地用户类顺便改变新注册用户的在线状态
					usersnum = getUsers(users);
					flag = 0;
					cout << usernames[idx] << "注册成功！" << endl;
				}

				checkBuff[1] = SIGHUPNUM;
				checkBuff[2] = 0;
				send(clientSocket[idx], checkBuff, strlen(checkBuff), NULL);
			}

			//将用户名与登录序号绑定
			strcpy(usernames[idx], name);
		}
	}
	//用户选择是单聊还是群聊	
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	int r = recv(clientSocket[idx], buff, 1023, NULL);
	if (r > 0 && buff[r - 1] == CHOOSENUM) {
		if (buff[0] == '1') {
			//创造新线程, 进入群聊界面
			// 7. 服务器接收客户端发来的信息
			cout << "用户" << usernames[idx] << "选择了群聊" << endl;
			chatroomConnection(idx);
		}

		else if (buff[0] == '2') {
			cout << "用户" << usernames[idx] << "选择了单聊" << endl;
			singleConnection(idx);
		}
	}
}
void singleConnection(int idx) {//单聊函数

	//查询该用户的好友,加入friendList链表和friendStream字符串流
	int count=0;
	string friendList[100];
	string friendStream;
	for (int i = 0; i < friendssnum; i++) {
		if (usernames[idx] == friends[i].user1name) {
			friendList[count++] = friends[i].user2name;
			friendStream += friends[i].user2name;
			friendStream += ";";
		}
	}	
	//将friendStream加上识别码转为char*发送给客户端
	char* buff = (char*)friendStream.data();
	int len = strlen(buff);
	buff[len] = CHOOSEFRIENDNUM;
	buff[len + 1] = 0;
	int s = send(clientSocket[idx], buff, len + 1, NULL);
	//接收客户端的选择消息
	int r = recv(clientSocket[idx], buff, 1023, NULL);
	cout << "用户选择了" << buff[0] << endl;
	string choosenFriend = friendList[buff[0] - '0'];
	//进入与对应好友的聊天界面
	memset(buff, 0, sizeof(buff));
	while(1) {
		int r = recv(clientSocket[idx], buff, 1023, NULL);
		if (r > 0 && buff[r - 1] == MESSAGENUM) {
			buff[r - 1] = 0;
			printf(">>用户%s:%s\n", usernames[idx], buff);
			//在消息前加上用户name
			char temp[50];
			strcpy(temp, usernames[idx]);
			strcat(temp, ":");
			strcat(temp, buff);
			strcpy(buff, temp);
			int len = strlen(buff);
			buff[len] = MESSAGENUM;
			buff[len + 1] = 0;
			//遍历登录过（正在线）的用户，发送消息
			for (int i = 0; i < concOrder; i++) {
				if (usernames[i] == choosenFriend||i==idx) {
					int s = send(clientSocket[i], buff, len + 1, NULL);
				}
			}
		}
		//这里也可以接受用户的其他请求，如加好友等
		else if (r > 0 && buff[r - 1] == ADDFRIENDNUM) {
			buff[r - 1] = 0;
			string addFriendName = buff;
			int i = insertFriends(usernames[idx], addFriendName);
			if (i == 0) {
				friendssnum = getFriends(friends);
				cout << "建立好友关系成功！" << usernames[idx] << "和" << addFriendName << endl;
			}
			else {
				cout << "建立好友关系失败！" << endl;
			}
		}
	}
	
}
void chatroomConnection(int idx) {//群聊函数；
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	while (1) {
		int r = recv(clientSocket[idx], buff, 1023, NULL);
		if (r > 0 && buff[r - 1] == MESSAGENUM) {
			buff[r - 1] = 0;
			printf(">>用户%s:%s\n", usernames[idx], buff);
			
			//在消息前加上用户name
			char temp[50];
			strcpy(temp, usernames[idx]);
			strcat(temp, ":");
			strcat(temp, buff);
			strcpy(buff, temp);
			int len = strlen(buff);
			buff[len] = MESSAGENUM;
			buff[len + 1] = 0;
			//广播信息
			for (int i = 0; i < concOrder; i++) {
				//if (i == idx) continue;
				int s = send(clientSocket[i], buff, len + 1, NULL);
			}
			buff[0] = 0;
		}
		else if (r > 0 && buff[r - 1] == ADDFRIENDNUM) {
			buff[r - 1] = 0;
			string addFriendName = buff;
			int i = insertFriends(usernames[idx], addFriendName);
			if (i == 0) {
				cout << "建立好友关系成功！" << usernames[idx] << "和" << addFriendName << endl;
			}
			else {
				cout << "建立好友关系失败！" << endl;
			}
		}
	}
}

int main() {
	if (prepare() < 0) return 0;
	//6.等待用户连接
	while (1) {

		SOCKADDR_IN cAddr = { 0 };
		int len = sizeof(cAddr);
		clientSocket[concOrder] = accept(serverSocket, (sockaddr*)& cAddr, &len);
		if (clientSocket[concOrder] == -1) {
			printf("客户端连接失败\n");
			closesocket(serverSocket);
			WSACleanup();
			return -2;
		}
		userIP = inet_ntoa(cAddr.sin_addr);
		printf("用户%d连接了服务器：%s!\n", concOrder, userIP);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)service, (char*)concOrder, NULL, NULL);
		concOrder++;

	}
	return 0;
}
