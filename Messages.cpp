#include <iostream>
#include <fstream>
#include <exception>
#include <nlohmann/json.hpp>
#include <string>
#include "Messages.h"
#include "Message.h"
#include "ServiceFunc.h"
#include "Converter.h"
#include "Constants.h"

using json = nlohmann::json;
//using namespace std;

//private:
void Messages::CheckArraySize() {	// �������� ������� ���������� ����� � ������� � ��� ���������� ��� �������������
	if (message.capacity() <= msgsCount)
		message.reserve(message.capacity() * 2);	//	��� ���������� ������� ������ ��� ����������� ��� ������� �����
}


//public:
Messages::Messages() {
	message.reserve(INITIAL_MSG_COUNT);
}

//Messages::~Messages() {}

bool Messages::Add(const int _senderId, const std::string& _senderLogin, const int _receiverId, const std::string& _receiverLogin) {	// ���������� ���������
	CheckArraySize();
	std::cout << "������� ����� ���������: ";
	std::string text = get_string_from_console();
	if (!check_empty_name(text, "���������"))
		return false;
	message.emplace_back(msgsCount, _senderId, _senderLogin, _receiverId, _receiverLogin, GetTime(), text);	// ������� ��������� � ����� �������
	msgsCount++;
	isChanged = true;	// ��������� ����� ������� ��������� � ������� ���������
	return true;
}


int Messages::FindUnReaded(const int _receiverId) const {	// ������� ������������� ��������� ��� ���������� ����������
	int count = 0;
	for (auto i = 0; i < msgsCount; i++)
		if ((message[i].receiverId == _receiverId) && (!message[i].wasReaded))
			count++;
	return count;
}


void Messages::Read(const int _receiverId, const bool _showUnReadedOnly) {	// ����� ���������
	for (auto i = 0; i < msgsCount; i++) {
		if ((message[i].receiverId == _receiverId) && (!_showUnReadedOnly || (!message[i].wasReaded))) {
			std::cout << "#" << message[i].id << " " << message[i].date_time << " �� " << message[i].senderLogin << ": " << message[i].text << '\n';
			message[i].wasReaded = true;	// ������� �����������
			isChanged = true;	// ��������� ����� ������� ��������� � ������� ���������
		}
	}
}


bool Messages::Open() {	// ������ ��������� �� �������� �����
	std::fstream msgs_data;
	msgs_data.open(MSG_DATA_FILE_NAME, std::ios::in);
	if (!msgs_data.is_open()) {
		std::cout << "����������� ������ ��� �������� ���������!\n";
		return false;
	}
	bool result = false;
	json jsonObj;
	CheckArraySize();
	try {	// ������� �������� json-������� � ������ �� ����
		msgs_data >> jsonObj;
		for (auto i = 0; i < jsonObj.size(); i++)
			message.emplace_back(
				jsonObj[i]["id"],
				jsonObj[i]["senderId"],
				utf8_to_cp1251(jsonObj[i]["senderLogin"]),	// ��������������� ����� �� UTF-8
				jsonObj[i]["receiverId"],
				utf8_to_cp1251(jsonObj[i]["receiverLogin"]),	// ��������������� ����� �� UTF-8
				utf8_to_cp1251(jsonObj[i]["date_time"]),	// ��������������� ����� �� UTF-8
				utf8_to_cp1251(jsonObj[i]["text"]),	// ��������������� ����� �� UTF-8
				jsonObj[i]["wasReaded"]
			);
		msgsCount = (int)jsonObj.size();
		result = true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	msgs_data.close();
	return result;
}

bool Messages::Save() {	// ���������� ��������� �� ������� ����
	std::fstream msgs_data;
	msgs_data.open(MSG_DATA_FILE_NAME, std::ios::out);
	if (!msgs_data.is_open()) {
		std::cout << "����������� ������ ��� ���������� ���������!\n";
		return false;
	}
	bool result = false;
	json jsonObj;
	try {	// ������� �������� json-������� � ������ � ����
		for (auto i = 0; i < msgsCount; i++) {
			jsonObj[i]["id"] = message[i].id;
			jsonObj[i]["senderId"] = message[i].senderId;
			jsonObj[i]["senderLogin"] = cp1251_to_utf8(message[i].senderLogin);	// ��������������� ����� � UTF-8
			jsonObj[i]["receiverId"] = message[i].receiverId;
			jsonObj[i]["receiverLogin"] = cp1251_to_utf8(message[i].receiverLogin);	// ��������������� ����� � UTF-8
			jsonObj[i]["date_time"] = cp1251_to_utf8(message[i].date_time);	// ��������������� ����� � UTF-8
			jsonObj[i]["text"] = cp1251_to_utf8(message[i].text);	// ��������������� ����� � UTF-8
			jsonObj[i]["wasReaded"] = message[i].wasReaded;
		}
		msgs_data << std::setw(4) << jsonObj;
		result = true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	msgs_data.close();
	return result;
}


int Messages::GetLastMsgId() const {	// ���������� ���������
	return msgsCount;
}


bool Messages::IsChanged() const {	// ������� ����� ������� ��������� � ����������
	return isChanged;
}