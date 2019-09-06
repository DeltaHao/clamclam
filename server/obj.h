/*PaoChat 1.5
obj.h
修改日期 9/2/20:00
*/
#pragma once
/*
这里定义了数据库中的表所对应的类
*/
#include<iostream>
#include<string>
using namespace std;

class User {//用户信息
public:
	int id;//用户ID
	string ip;//用户IP地址
	string name;//用户名
	string password;//用户密码
	void update(int id, string ip, string name, string password) {
		this->id = id;
		this->ip = ip;
		this->name = name;
		this->password = password;
	}
};
class Friends {//好友对
public:
	int id;
	string user1name;//用户1的name
	string user2name;//用户2的name
	void update(int id, string user1name, string user2name) {
		this->id = id;
		this->user1name = user1name;
		this->user2name = user2name;
	}
};