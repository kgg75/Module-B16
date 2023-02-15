#include <iostream>
#include "Chat.h"
#include "ServiceFunc.h"

//using namespace std;

void print_help(const CommandSpace* _commandSpace) {	// ������ ������� �� ��������
	for (int i = 0; _commandSpace[i].command[0] != 0; i++)
		std::cout << "\t" << _commandSpace[i].command << "\t- " << _commandSpace[i].help << '\n';
}


void AtStart() {	// ��������� ���������
	std::cout.clear();
	std::cout << "����� ����� ��������� ��� ����!\n";
	std::cout << SEPARATOR;
	std::cout << "�������� ��������:\n";
	print_help(GLOBAL_COMMANDS);
	std::cout << SEPARATOR;
	std::cout << "��� ������� " << GetTime() << '\n';
	std::cout << SEPARATOR;
}


int get_command(const CommandSpace* _commandSpace, const std::string _msg, const std::string& _user_name) {	// ��������� ������� � �������, ������ ��������������� ���� ��������
	std::cout << _user_name << _msg;	// ����� ���������
	std::string command = get_value_from_console();
	lowercase(command);	//	�������������� ������ ������� � ������ �������
	for (int i = 0; _commandSpace[i].command[0] != 0; i++)	// ����� ���� �������
		if (command == _commandSpace[i].command || command == _commandSpace[i].command2)
			return i;
	return -1;
}


int Chat(Users& _users, Messages& _messages) {	// ������� ���������� ����
	AtStart();
	bool doing = true;
	while (doing) {
		switch (get_command(GLOBAL_COMMANDS, "������� ������� >> ")) {	// �������� ���������� ������� � �������
			case 0:	// ������� reg - ������������������
				std::cout << (_users.NewUser() ? "�� ������� ����������������!\n" : "��������� ������ �����������.\n");	// ������ ������ ������������
				break;
			case 1:	// ������� login - ����
				if (_users.Login()) {
					int unReadedMsgs = _messages.FindUnReaded(_users.GetUserId());	// ������� ����� ������������� ���������
					if (unReadedMsgs > 0) {
						std::cout << "���� ������������� ��������� (" << unReadedMsgs << "). ";
						if (QuestionYN("��������?"))
							_messages.Read(_users.GetUserId(), true);	// ����� ������������� ���������
					}
					// ������ ���������� ������������ ������
					while (_users.GetCurrentUser() != 0) {	// ���� ������������ �� ����� �� ����
						switch (get_command(LOCAL_COMMANDS, ", ������� ������� >> ",  _users.GetUserLogin())) {	// �������� ��������� ������� � �������
							case 0: {	// ��������� ������� send - ��������� ���������
									int receiverId = _users.ChooseUser();	// ����� ���������� � ��������� ��� id
									if (receiverId != -1) {
										if (receiverId != _users.GetUserId()) {
											if (_messages.Add(_users.GetUserId(), _users.GetUserLogin(), receiverId, _users.GetUserLogin(receiverId)))
												std::cout << "��������� ����������; id=" << _messages.GetLastMsgId() - 1 << '\n';
										}
										else
											std::cout << "������ ���������� ��������� ������ ����.\n";
									}
								}
								break;
							case 1:	// ��������� ������� read - ��������� ���������
								_messages.Read(_users.GetUserId(), QuestionYN("�������� ������ ������������� (y) ��� ��� ��������� (n)?"));
								break;
							case 2:	// ��������� ������� info - ������� ���������� � ������������
								_users.Show();
								break;
							case 3:	// ��������� ������� list - ������� ������ ���� �������������
								_users.List();
								break;
							case 4:	// ��������� ������� delete - ������� ������� ������ ������������
								if (QuestionYN("���������� � ������������ ����� �������. �� �������?"))
									_users.DeleteUser();
								break;
							case 5: {	// ��������� ������� change - �������� ������� ������ ������������
									bool loop = true;
									while (loop) {
										std::cout << "�� ���������� � ���� ��������� ������� ������ ������������ " << _users.GetUserLogin() << '\n';
										print_help(SUBLOCAL_COMMANDS);
										switch (auto cmd_code = get_command(SUBLOCAL_COMMANDS, ", �������� �������� ��� ������� >> ", _users.GetUserLogin())) {	// �������� ��������� ������� � �������
											case 0:	// ��������� ������� name - �������� ��� ������������ (cmd_code == __name)
											case 1:	// ��������� ������� log - �������� ������� ��� (login) ������������ (cmd_code == __login)
											case 2:	// ��������� ������� pwd - �������� ������ ������������ (cmd_code == __pwd)
												if (_users.ChangeRegData((Users::RegDataType)cmd_code)) {
													std::cout << "������� ������ ���� ������� ��������:\n";
													_users.Show();
												}
												break;
											case 3:	// ��������� ������� help - ������� ������� �� ��������
												print_help(SUBLOCAL_COMMANDS);
												break;
											case 4:	// ��������� ������� exit - ����� �� ����� ����
												loop = false;
												std::cout << "�� ����� �� ���� ��������� ������� ������.\n";
												break;
											default:
												std::cout << MSG_ERR_CMD;
										}
									}
								}	// ����� ���������� ������������ ������� change - �������� ������� ������ ������������
								break;
							case 6:	// ��������� ������� help - ������� ������� �� ��������
								print_help(LOCAL_COMMANDS);
								break;
							case 7:	// ��������� ������� logout - �����
								_users.Logout();
								break;
							default:
								std::cout << MSG_ERR_CMD;
						}
					}// ����� ���������� ������������ ������
				}
				break;
			case 2:	// ������� help - ������� ������� �� ��������
				print_help(GLOBAL_COMMANDS);
				break;
			case 3:	// ������� quit - ������� ���
				doing = false;
				if (_users.IsChanged() && !_users.Save())	// ���� ���� ���������, ��������� ������ ������������� �� ������� ����
					return 1;
				if (_messages.IsChanged())	// ���� ���� ���������, ��������� ��������� �� ������� ����
					_messages.Save();
				break;
			default:
				std::cout << MSG_ERR_CMD;
		}
	}
	return 0;
}
