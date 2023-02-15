#pragma once
#include <vector>
#include "Message.h"
#include "User.h"


class Messages {
	private:
		std::vector <Message> message;	// ������ � �����������
		int msgsCount = 0;	// ���������� ���������
		bool isChanged = false;	// ���� ������� ���������
		void CheckArraySize();	// �������� ������� ���������� ����� � ������� � ��� ���������� ��� �������������
	public:
		Messages();
		//~Messages();
		bool Add(const int _senderId, const std::string& _senderLogin, const int _receiverId, const std::string& _receiverLogin);	// ���������� ���������; true � ������ ������
		int FindUnReaded(const int _receiverId) const;	// ������� ������������� ��������� ��� ���������� ����������; ���������� ����������
		void Read(const int _receiverId, const bool _showUnReadedOnly);	// ����� ���������
		bool Open();	// �������� ��������� �� �������� �����; true � ������ ������
		bool Save();	// �������� ���������� �� ������� ����; true � ������ ������
		int GetLastMsgId() const;	// ���������� id ���������� ��������� 
		bool IsChanged() const;	// ������� ����� ������� ��������� � ����������
};

