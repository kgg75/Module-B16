#pragma once
#include <string>
#include <vector>
#include "User.h"


class Users {	// ��� ����� bool ������������ �������� true � ������ ������, false ��� ��������� �����������
	private:
		std::vector<User> user;	// ������ � ������� �������������
		int usersCount = 0;	// ���������� �������������
		int idsCount = 0; // ���������� �������������� ���������������
		int currentUser = idsCount; // ���������� ����� �������� ������������ (���������� � 1)
		bool isChanged = false;	// ���� ������� ��������� � ������ �������������
		SHA1PwdArray shaPwdArray;	// ��������� ��������� ���� ������
// �������
		void CheckArraySize();	// �������� ������� ���������� ����� � ������� � ��� ���������� ��� �������������
		int FindLogin(const std::string& _login, const bool _show_message = true) const;	// �������� ������������ ��������� login ������������; ������� ����������� ������ � ������ ������
		bool SetName(std::string& _name);
		bool SetLogin(std::string& _login);
		bool SetPassword(std::string& _password);
	public:
		enum RegDataType {
			__name,
			__login,
			__pwd
		};
		Users();
		//~Users();
		bool NewUser();	// ����������� ������ ������������
		bool Login();	// ���� ������������ � ���
		void Logout();	// ����� ������������ �� ����
		int ChooseUser();	// ����� ���������� ��� �������� ���������; ���������� id ������������
		bool Open();	// ������ ������ ������������� �� �������� �����
		bool Save();	// ���������� ������ ������������� �� ������� ����
		bool DeleteUser();	// �������� ������� ������ ������������
		bool ChangeRegData(Users::RegDataType _type);	// ��������� ������� ������ ������������
		void Show(const int _user_index = -1);	// ����������� ������ ���������� ������������; �� ��������� - ��������
		void List();	// ����� ������ ���� �������������
// getters
		int GetCurrentUser() const;	// ���������� ����� �������� ������������
		int GetUserId(const int _user_index = -1) const;	// id ���������� ������������; �� ��������� - ��������
		std::string GetUserLogin(const int _id = -1) const;	// login ������������, ���������� �� id; �� ��������� - ��������
		bool IsChanged() const;	// ������� ����� ������� ��������� � ������ �������������
};

