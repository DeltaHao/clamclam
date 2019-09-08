/*
PaoChat 1.5
修改时间 9/2/20:00

*/

#include <windows.h>
#include<iostream>
#include"intConnect.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int prepare();
int Login(char*, char*);
int Register(char*, char*);
int choosePattern(int);
string receiveMessage();
int sendMessage(char*);
int logOut();
string receiveFriendList();
int chooseFriend(int);

void recive() {
	string message;
	while (1) {
		message = receiveMessage(); 
		//cout << message << endl;
		if (message[0] != 0) {
			cout << message << endl;
		}
	}
}

int main() {
	if (prepare() < 0) {
		return -1;
	}
	//登录或注册
	char op;
	cout << ">>>>>请登录或注册(登录输1,注册输2):";
	cin >> op;
	if (op == '1') {
		while (1) {
			char name[1024];
			char password[1024];
			cout << ">>>登录" << endl;
			cout << ">>>>>用户名:";
			cin >> name;
			cout << ">>>>>密码:";
			cin >> password;
			int l = Login(name, password);
			if (l == 0) {
				cout << "登录成功！" << endl;
				break;
			}
			else {
				cout << "登陆失败，请检查账号密码！" << endl;
			}
		}
	}
	else if (op == '2') {
		while (1) {
			char name[1024];
			char password[1024];
			cout << ">>>注册" << endl;
			cout << ">>>>>用户名:";
			cin >> name;
			cout << ">>>>>密码:";
			cin >> password;
			int r = Register(name, password);
			if (r == 0) {
				cout << "注册成功！" << endl;
				break;
			}
			else {
				cout << "注册失败！用户名已被占用" << endl;
			}
		}
	}

	//选择单聊or群聊
	cout << "请选择群聊或单聊（群聊1，单聊2）" << endl;
	int choosenum;
	cin >> choosenum;
	getchar();
	int c = choosePattern(choosenum);
	if (c == 0)
		cout << "选择成功！" << endl;
	else  cout << "选择失败！" << endl;

	//接收服务器消息
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recive, NULL, NULL, NULL);

	if (choosenum == 1) {
		cout << "进入群聊界面" << endl;
		while (1) {
			char message[1024];
			gets_s(message);
			int s = sendMessage(message);
			if (s == 0) cout << "发送成功！" << endl;
			else cout << "发送失败！" << endl;
		}
	}
	else if (choosenum == 2) {
		cout << "进入单聊界面" << endl;
		string friendList;
		friendList = receiveFriendList();
		cout << "您的好友有：" ;
		if(friendList[0])
			cout << friendList << endl;
		cout << "请选择聊天对象：";
		int friendNum;
		cin >> friendNum;
		getchar();
		int c = chooseFriend(friendNum);
		if (c == 0) cout << "选择成功！" << endl;
		else cout << "选择失败！" << endl;
		//发送消息
		while (1) {
			char message[1024];
			gets_s(message);
			int s = sendMessage(message);
			if (s == 0) cout << "发送成功！" << endl;
			else cout << "发送失败！" << endl;
		}
	}

	return 0;
}