/*PaoChat 1.5
obj.h
�޸����� 9/2/20:00
*/
#pragma once
/*
���ﶨ�������ݿ��еı�����Ӧ����
*/
#include<iostream>
#include<string>
using namespace std;

class User {//�û���Ϣ
public:
	int id;//�û�ID
	string ip;//�û�IP��ַ
	string name;//�û���
	string password;//�û�����
	void update(int id, string ip, string name, string password) {
		this->id = id;
		this->ip = ip;
		this->name = name;
		this->password = password;
	}
};
class Friends {//���Ѷ�
public:
	int id;
	string user1name;//�û�1��name
	string user2name;//�û�2��name
	void update(int id, string user1name, string user2name) {
		this->id = id;
		this->user1name = user1name;
		this->user2name = user2name;
	}
};