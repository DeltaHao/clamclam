/*
PaoChat 1.5
server.cpp
�޸����� 9/2/20:00
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
ÿ��send��recv����Ϣ�����һ����λ������Ϣ����
��Ϣ���� | ���
-----------
��¼��֤ | 1
��ʵ��Ϣ | 2
ע����֤ | 3
������Ϣ | 4
����Ⱥ�� | 5
��Ӻ��� | 6
ѡ����� | 7
*/
#define CHECKNUM 1
#define MESSAGENUM 2
#define SIGHUPNUM 3
#define LOGOUTNUM 4
#define CHOOSENUM 5
#define ADDFRIENDNUM 6
#define CHOOSEFRIENDNUM 7

#define WORNG_USER 100
//��������
int databaseInit();
int createUser();
int createFriends();
int insertUser(string, string, string);
int insertFriends(string, string);
int logInCheck(string, string);
int getUsers(User*);
int getFriends(Friends*);

//ȫ�ֱ���
SOCKET serverSocket;//������׽���
SOCKET clientSocket[100];//�ͻ����׽���
static int concOrder = 0;//�ͻ������ӵĴ���
static char usernames[1024][50];//���ӳɹ����û����û���
static int usersnum;//���ݿ����û�������
User users[100];//�����ݿ��н��յ��û���Ϣ
static int friendssnum;//���ݿ��к��ѶԵ�����
Friends friends[100];//�����ݿ��н��յĺ��Ѷ���Ϣ
char* userIP;

int prepare() {
	/*
	���ã�ǰ��׼�����������ݿ⣬����socket��
	�������ޣ�
	����ֵ��-1��ʾ����ֱ���˳���
	*/
	memset(users, 0, sizeof(users));
	//��ʼ�����ݿ�,����
	databaseInit();
	createUser();
	createFriends();
	usersnum = getUsers(users);
	friendssnum = getFriends(friends);
	// 1.����汾Э��
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);
	if ((LOBYTE(wsData.wVersion) != 2) || (HIBYTE(wsData.wVersion) != 2)) {
		printf("------����汾Э��ʧ��------\n");
		return -1;
	}
	printf("------����汾Э��ɹ�------\n");


	// 2.����socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		printf("����socketʧ��\n");
		WSACleanup();
		return -2;
	}
	printf("------����socket�ɹ�--------\n");


	// 3.����ip,port
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);


	// 4.��ip,port��socket
	int b = bind(serverSocket, (sockaddr*)& addr, sizeof addr);
	if (b == -1) {
		printf("��ip��portʧ��\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	} 
	printf("------��ip���˿ڳɹ�------\n");


	// 5. ����
	int l = listen(serverSocket, 10);
	if (l == -1) {
		printf("����ʧ��\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("���ڼ���......\n");
	return 0;
}
void service(int idx) {
	/*
	���ã�ÿ��һ���ͻ������ӣ������µ��̷߳���֮
	�������ͻ����������
	����ֵ����
	*/
	int flag = 1;
	while (flag) {
		char checkBuff[1024];
		int r = recv(clientSocket[idx], checkBuff, 1023, NULL);
		if (r > 0) {
			int type = checkBuff[r - 1];//������¼����ע��
			checkBuff[r - 1] = 0;
			char name[1024];
			char password[1024];

			//��checkBuff�е����ݷֱ����name�� password��
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


			if (type == CHECKNUM) {//��¼
				//��֤�û���������
				int userid = logInCheck(name, password);
				if (userid == -1) {
					cout << usernames[idx] << "��¼ʧ�ܣ�" << endl;
					checkBuff[0] = WORNG_USER;
				}
				else {
					flag = 0;
					cout << usernames[idx] << "��¼�ɹ���" << endl;
				}
				checkBuff[1] = CHECKNUM;
				checkBuff[2] = 0;
				send(clientSocket[idx], checkBuff, strlen(checkBuff), NULL);
			}
			else if (type == SIGHUPNUM) {//ע��
				//�����ݿ��в�ѯ
				int userid = logInCheck(name, password);
				if (userid != -1) {//����Ѿ��и��û�
					checkBuff[0] = WORNG_USER;
					cout << usernames[idx] << "ע��ʧ�ܣ�" << endl;
				}
				else {
					//�����ݿ�����³�Ա
					insertUser(userIP, name, password);
					//���±����û���˳��ı���ע���û�������״̬
					usersnum = getUsers(users);
					flag = 0;
					cout << usernames[idx] << "ע��ɹ���" << endl;
				}

				checkBuff[1] = SIGHUPNUM;
				checkBuff[2] = 0;
				send(clientSocket[idx], checkBuff, strlen(checkBuff), NULL);
			}

			//���û������¼��Ű�
			strcpy(usernames[idx], name);
		}
	}
	//�û�ѡ���ǵ��Ļ���Ⱥ��	
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	int r = recv(clientSocket[idx], buff, 1023, NULL);
	if (r > 0 && buff[r - 1] == CHOOSENUM) {
		if (buff[0] == '1') {
			//�������߳�, ����Ⱥ�Ľ���
			// 7. ���������տͻ��˷�������Ϣ
			cout << "�û�" << usernames[idx] << "ѡ����Ⱥ��" << endl;
			chatroomConnection(idx);
		}

		else if (buff[0] == '2') {
			cout << "�û�" << usernames[idx] << "ѡ���˵���" << endl;
			singleConnection(idx);
		}
	}
}
void singleConnection(int idx) {//���ĺ���

	//��ѯ���û��ĺ���,����friendList�����friendStream�ַ�����
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
	//��friendStream����ʶ����תΪchar*���͸��ͻ���
	char* buff = (char*)friendStream.data();
	int len = strlen(buff);
	buff[len] = CHOOSEFRIENDNUM;
	buff[len + 1] = 0;
	int s = send(clientSocket[idx], buff, len + 1, NULL);
	//���տͻ��˵�ѡ����Ϣ
	int r = recv(clientSocket[idx], buff, 1023, NULL);
	cout << "�û�ѡ����" << buff[0] << endl;
	string choosenFriend = friendList[buff[0] - '0'];
	//�������Ӧ���ѵ��������
	memset(buff, 0, sizeof(buff));
	while(1) {
		int r = recv(clientSocket[idx], buff, 1023, NULL);
		if (r > 0 && buff[r - 1] == MESSAGENUM) {
			buff[r - 1] = 0;
			printf(">>�û�%s:%s\n", usernames[idx], buff);
			//����Ϣǰ�����û�name
			char temp[50];
			strcpy(temp, usernames[idx]);
			strcat(temp, ":");
			strcat(temp, buff);
			strcpy(buff, temp);
			int len = strlen(buff);
			buff[len] = MESSAGENUM;
			buff[len + 1] = 0;
			//������¼���������ߣ����û���������Ϣ
			for (int i = 0; i < concOrder; i++) {
				if (usernames[i] == choosenFriend||i==idx) {
					int s = send(clientSocket[i], buff, len + 1, NULL);
				}
			}
		}
		//����Ҳ���Խ����û�������������Ӻ��ѵ�
		else if (r > 0 && buff[r - 1] == ADDFRIENDNUM) {
			buff[r - 1] = 0;
			string addFriendName = buff;
			int i = insertFriends(usernames[idx], addFriendName);
			if (i == 0) {
				friendssnum = getFriends(friends);
				cout << "�������ѹ�ϵ�ɹ���" << usernames[idx] << "��" << addFriendName << endl;
			}
			else {
				cout << "�������ѹ�ϵʧ�ܣ�" << endl;
			}
		}
	}
	
}
void chatroomConnection(int idx) {//Ⱥ�ĺ�����
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	while (1) {
		int r = recv(clientSocket[idx], buff, 1023, NULL);
		if (r > 0 && buff[r - 1] == MESSAGENUM) {
			buff[r - 1] = 0;
			printf(">>�û�%s:%s\n", usernames[idx], buff);
			
			//����Ϣǰ�����û�name
			char temp[50];
			strcpy(temp, usernames[idx]);
			strcat(temp, ":");
			strcat(temp, buff);
			strcpy(buff, temp);
			int len = strlen(buff);
			buff[len] = MESSAGENUM;
			buff[len + 1] = 0;
			//�㲥��Ϣ
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
				cout << "�������ѹ�ϵ�ɹ���" << usernames[idx] << "��" << addFriendName << endl;
			}
			else {
				cout << "�������ѹ�ϵʧ�ܣ�" << endl;
			}
		}
	}
}

int main() {
	if (prepare() < 0) return 0;
	//6.�ȴ��û�����
	while (1) {

		SOCKADDR_IN cAddr = { 0 };
		int len = sizeof(cAddr);
		clientSocket[concOrder] = accept(serverSocket, (sockaddr*)& cAddr, &len);
		if (clientSocket[concOrder] == -1) {
			printf("�ͻ�������ʧ��\n");
			closesocket(serverSocket);
			WSACleanup();
			return -2;
		}
		userIP = inet_ntoa(cAddr.sin_addr);
		printf("�û�%d�����˷�������%s!\n", concOrder, userIP);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)service, (char*)concOrder, NULL, NULL);
		concOrder++;

	}
	return 0;
}
