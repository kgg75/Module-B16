#pragma once
#include <string>
#include "SHA1.h"


struct User {
	int id;	// ���������� �������������
	std::string name;	// ������ ���
	std::string login;	// ������� ��� (login)
	SHA1PwdArray pass_sha1_hash;	// ��� ������
	bool locked = false;	// ���� ���������� ������������ (��������������)
	bool logged = false;	// ����, ����������� �� ���� ������������

	User(
		const int _id,
		const std::string& _name,
		const std::string& _login,
		const SHA1PwdArray& _pass_sha1_hash,
		const bool locked = false,
		const bool logged = false
	);
};

