#pragma once
#include <utility>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <set>
#include <stdint.h>
#include <string.h>
#include <mutex>

enum QueryType {
	SignIn = 0,
	SignUp,
	GET_USER_INFO,
	SEND_MESSAGE,
	GET_MESSAGES,
	NEW_DATA_FROM_SERVER,
	GET_CHAT,
	GET_CHATS,
	GET_CHAT_MEMBERS
};

class Client;
class Message;
class Chat;

static constexpr size_t magicNumber = 0x5a7f8d123;
struct networkHeader {
	const size_t magicNumber = 0x5a7f8d123;
	QueryType type = SignUp;
	size_t payload = 0;
	uint32_t request_id = 0;
};

enum MessageType {
	MT_Text = 0,
	MT_Image,
	MT_Video,
	MT_File
};

extern std::unordered_map<size_t, Chat*> LoadedChats;
extern std::vector<Client*> Clients;
constexpr int maxLoadedMessagesPerChat = 200;

class Icon {
	char* ptr = nullptr;
	size_t _size = 0;
public:
	size_t size() const {
		return _size;
	}

	bool loaded() {
		return ptr == nullptr;
	}

	std::pair<char*, size_t> getIcon() const {
		return { ptr, _size };
	}

	void loadIcon(const char* p, size_t s) {
		if (ptr) {
			if (_size == s) {
				memcpy((void*)p, ptr, s);
				return;
			}
		}
		else {
			delete[]ptr;
		}

		ptr = new char[s];
		_size = s;
		memcpy((void*)p, ptr, s);
	}

	void unloadIcon() {
		if (ptr) {
			delete[] ptr;
		}
		_size = 0;
	}

	Icon() {}
	Icon(const char* p, size_t s) {
		loadIcon(p, s);
	}

	~Icon() {
		unloadIcon();
	}
};

class Client {
public:
	std::mutex asyncDataMutex;

	std::string userName = "";
	std::string login = "";
	const unsigned long userID = 0;
	size_t lastActivity = 0;
	Icon avatar;

	Client(const std::string& n, unsigned long i) : userID(i), userName(n) {
		Clients.push_back(this);
	}

	~Client() {
		int i = 0;
		for (Client* c : Clients) {
			if (c->userID == userID) {
				Clients.erase(Clients.begin() + i);
				return;
			}
			i++;
		}
	}
};

// MsgID;ClientID;ChatID;MsgType;MessageTime;size;data 
class Message {
	char* data = nullptr;
	size_t size = 0;
public:
	const size_t ChatID = 0;
	const size_t ClientID = 0;
	const size_t MessageID = 0;
	MessageType MsgType = MT_Text;
	size_t MessageTime = 0;
	bool Changed = false;

	void setData(char* src, size_t size) {
		if (data) delete[]data;
		data = new char[size];
		if (data) {
			memcpy(data, src, size);
		}
	}

	Message() = delete;
	Message(size_t chatID, size_t clientID, MessageType msgType, size_t messageTime)
		: ChatID(chatID), ClientID(clientID), MsgType(msgType), MessageTime(messageTime) {}
	bool operator<(const Message& other) const {
		if (MessageTime < other.MessageTime) return true;
		return false;
	}
	~Message() {
		if (data) delete[]data;
	}
};

struct msgCompare {
	bool operator()(const Message* a, const Message* b) const {
		return a->MessageTime < b->MessageTime;
	}
};

class Chat {
	std::set<Message*, msgCompare> LoadedMessages;
public:
	const size_t ChatID = 0;
	std::string Name = "Chat";
	std::set<size_t> Members;
	size_t OwnerID = 0;
	const size_t CreateTime = 0;

	void AddMessage(Message* msg) {

	}

	Chat(const size_t id, std::string Name, size_t Owner, size_t CreateTime) : ChatID(id), CreateTime(CreateTime), OwnerID(Owner), Name(Name) {};
};

std::pair<const char*, size_t> get_data(const char*, QueryType, size_t);
void initialNetwork();
void cleanup();
void serverDataFunction(std::function<void(std::pair<networkHeader, std::pair<char*, size_t>>)>);
Client** getClientPtr();