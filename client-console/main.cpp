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
string* reciveFriendList();
int chooseFriend(int);

void recive(){
	string message;
	while (1) {
		message = receiveMessage();
		if (message[0] != 0){
			cout << message << endl;
		}
	}
}

int main() {
	prepare();
	//��¼��ע��
	int op;
	cout << ">>>>>���¼��ע��(��¼��1,ע����2):";
	cin >> op;
	if (op == '1') {
		while (1) {
			char name[1024];
			char password[1024];
			cout << ">>>��¼" << endl;
			cout << ">>>>>�û���:";
			gets_s(name);
			cout << ">>>>>����:";
			gets_s(password);
			int l = Login(name, password);
			if (l == 0) {
				cout << "��½�ɹ���" << endl;
				break;
			}
			else {
				cout << "��½ʧ�ܣ������˺����룡" << endl;
			}
		}
	}
	else if (op == '2') {
		while (1) {
			char name[1024];
			char password[1024];
			cout << ">>>ע��" << endl;
			cout << ">>>>>�û���:";
			gets_s(name);
			cout << ">>>>>����:";
			gets_s(password);
			int r = Register(name, password);
			if (r == 0) {
				cout << "ע��ɹ���" << endl;
				break;
			}
			else {
				cout << "ע��ʧ�ܣ��û����ѱ�ռ��" << endl;
			}
		}
	}

	//ѡ����orȺ��
	cout << "��ѡ��Ⱥ�Ļ��ģ�Ⱥ��1������2��" << endl;
	int choosenum;
	cin >> choosenum;
	int c = choosePattern(choosenum);
	if (c == 0) 
		cout << "ѡ��ɹ���" << endl;
	else  cout << "ѡ��ʧ�ܣ�" << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recive, NULL, NULL, NULL);

	if (choosenum == 1) {
		cout << "����Ⱥ�Ľ���" << endl;
		while (1) {
			char message[1024];
			gets_s(message);
			int s = sendMessage(message);
			if (s == 0)cout << "���ͳɹ���" << endl;
			else cout << "����ʧ�ܣ�" << endl;
		}
	}
	else if (choosenum == 2) {
		cout << "����Ⱥ�Ľ���" << endl;

	}

	return 0;
}