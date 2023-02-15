#pragma once
#include <vector>
#include "Message.h"
#include "User.h"


class Messages {
	private:
		std::vector <Message> message;	// массив с сообщениями
		int msgsCount = 0;	// количество сообщений
		bool isChanged = false;	// флаг наличия изменений
		void CheckArraySize();	// проверка наличия свободного места в векторе и его увеличение при необходимости
	public:
		Messages();
		//~Messages();
		bool Add(const int _senderId, const std::string& _senderLogin, const int _receiverId, const std::string& _receiverLogin);	// добавление сообщения; true в случае успеха
		int FindUnReaded(const int _receiverId) const;	// подсчёт непрочитанных сообщений для указанного получателя; возвращает количество
		void Read(const int _receiverId, const bool _showUnReadedOnly);	// вывод сообщений
		bool Open();	// загрузка сообщений из внешнего файла; true в случае успеха
		bool Save();	// сохрание сообщенией во внешний файл; true в случае успеха
		int GetLastMsgId() const;	// возвращает id последнего сообщения 
		bool IsChanged() const;	// возврат флага наличия изменений в сообщениях
};

