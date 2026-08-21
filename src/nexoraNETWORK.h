#pragma once
#include <utility>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>
#include <set>
#include <stdint.h>
#include <string.h>
#include <mutex>
#include <sstream>
#include <iostream>
#include <thread>

enum QueryType {
	SignIn = 0,
	SignUp,
	GET_USER_INFO,
	SEND_MESSAGE,
	GET_MESSAGES,
	NEW_DATA_FROM_SERVER,
	GET_CHAT,
	GET_CHATS,
	GET_CHAT_MEMBERS,
	UPDATE_NAME,
	UPDATE_AVATAR
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

std::pair<const char*, size_t> get_data(const char*, QueryType, size_t);
void initialNetwork();
void cleanup();
void serverDataFunction(std::function<void(std::pair<networkHeader, std::pair<char*, size_t>>)>);
Client** getClientPtr();

class AsyncData {
	const char* ptr = nullptr;
	QueryType type = SignUp;
	size_t size = 0;
	std::pair<const char*, size_t> output_data{};
	bool completed = false;

	std::function<void(std::pair<const char*, size_t>)> completedFunction;
	std::function<void(void)> sendedFunction;
public:
	bool deleteOnCompleted = true;

	bool isCompleted() const {
		return completed;
	}

	std::pair<const char*, size_t> getOutput() const {
		return output_data;
	}

	void send() {
		std::thread a([this]() {

			bool completed = false;
			if (sendedFunction) {
				sendedFunction();
			}

			output_data = get_data(ptr, type, size);
			completed = true;

			if (completedFunction) {
				completedFunction(output_data);
			}

			if (deleteOnCompleted) {
				delete this;
			}
			});
		a.detach();
		return;
	}

	void Completed(std::function<void(std::pair<const char*, size_t>)> f) {
		completedFunction = f;
	}

	void Sended(std::function<void(void)> f) {
		sendedFunction = f;
	}

	AsyncData(const char* c, QueryType t, size_t s) : ptr(c), type(t), size(s) {}
};

constexpr std::size_t operator"" _kb(unsigned long long value) {
    return value * 1024ULL;
}

constexpr std::size_t operator"" _mb(unsigned long long value) {
    return value * 1024ULL * 1024ULL;
}

constexpr std::size_t operator"" _gb(unsigned long long value) {
    return value * 1024ULL * 1024ULL * 1024ULL;
}

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
				memcpy(ptr, (void*)p, s);
				return;
			} else {
				delete[] ptr;
				_size = 0;
			}
		}

		ptr = new char[s];
		_size = s;
		memcpy(ptr, (void*)p, s);
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

	void setData(char* src, size_t sz) {
		if (data) {
			if (size != sz) delete[]data;
			data = new char[size];
		} else {
			data = new char[size];
		}

		size = sz;
		
		memcpy(data, src, sz);
	}

	std::pair<const char*, size_t> getData() const {
		return {data, size};
	}

	Message() = delete;
	Message(size_t msgID, size_t chatID, size_t clientID, MessageType msgType, size_t messageTime)
		: MessageID(msgID), ChatID(chatID), ClientID(clientID), MsgType(msgType), MessageTime(messageTime) {}
	bool operator<(const Message& other) const {
		if (MessageTime < other.MessageTime) return true;
		return false;
	}
	~Message() {
		if (data) delete[]data;
	}
};

std::vector<Message*> deserializeMessages(std::pair<const char*, size_t>);

struct msgCompare {
	bool operator()(const size_t a, const size_t b) const {
		return a < b;
	}
};

class Chat {
	std::map<size_t, Message*, msgCompare> LoadedMessages;
	std::recursive_mutex mtx;
public:
	const size_t ChatID = 0;
	std::string Name = "Chat";
	std::set<size_t> Members;
	size_t OwnerID = 0;
	const size_t CreateTime = 0;

	void AddMessage(Message* msg) {

	}

	void LoadMessages(size_t quantity, size_t from, std::function<void(void)> f) {
		std::string string_input = std::to_string(quantity) + '|' + std::to_string(from) + '|' + std::to_string(ChatID);
		size_t size = string_input.size();
		char* input = new char[size];
		memcpy(input, string_input.data(), size);
		auto as = new AsyncData(input, GET_MESSAGES, size);
		as->Completed([input, f, this](std::pair<const char*, size_t> data){
			delete[] input;
			if (!data.first) {
				return;
			}
			
			std::vector<Message*> messages = deserializeMessages(data);
			mtx.lock();
			for (Message* m : messages) {
				auto it = LoadedMessages.find(m->MessageID);
				if (it == LoadedMessages.end()) {
					LoadedMessages.insert({m->MessageID, m});
				} else {
					delete it->second;
					it->second = m; 
				}
			}
			mtx.unlock();
			f();
		});
		as->send();
	}

	const std::map<size_t, Message*, msgCompare>& GetMessages() {
		return LoadedMessages;
	}

	Chat(const size_t id, std::string Name, size_t Owner, size_t CreateTime) : ChatID(id), CreateTime(CreateTime), OwnerID(Owner), Name(Name) {};
};

inline Chat* deserializeChat(std::pair<const char*, size_t> data) {
	std::string tmp(data.first, data.second);
	std::istringstream in(tmp);

	size_t id, owner, time, len;
	std::string name;

	in >> id >> owner >> time >> len;
	in.get();
	name.resize(len);
	in.read(name.data(), len);

	return new Chat(id, name, owner, time);
}

inline std::vector<Chat*> deserializeChats(std::pair<const char*, size_t> data) {
	const char* ptr = data.first;
	const char* end = ptr + data.second;

	if (!strcmp(data.first, "e1")) {
		std::cout << "Error while deserialize chats" << std::endl;
		return {};
	}

	if (!strcmp(data.first, "w1")) {
		std::cout << "No chats to deserialize" << std::endl;
		return {};
	}

	if (ptr + sizeof(size_t) > end) abort();

	size_t count;
	memcpy(&count, ptr, sizeof(count));
	ptr += sizeof(count);

	std::vector<Chat*> result;

	for (size_t i = 0; i < count; i++) {
		if (ptr + sizeof(size_t) > end) abort();

		size_t size;
		memcpy(&size, ptr, sizeof(size));
		ptr += sizeof(size);

		if (ptr + size > end) abort();

		std::string chunk(ptr, size);
		ptr += size;

		result.push_back(deserializeChat({ chunk.data(), chunk.size() }));
	}

	return result;
}

inline Client* deserializeClient(const std::string& data) {
	std::istringstream in(data);
	size_t id, lastActivity, len1, len2, len3;
	std::string name, login, icon;

	in >> id >> lastActivity >> len1;
	in.get();
	name.resize(len1);
	in.read(name.data(), len1);
	in >> len2;
	in.get();
	login.resize(len2);
	in.read(login.data(), len2);
	in >> len3;
	in.get();
	icon.resize(len3);
	in.read(icon.data(), len3);

	Client* c = new Client(name, id);
	c->lastActivity = lastActivity;
	c->avatar.loadIcon(icon.data(), icon.size());
	c->login = login;

	return c;
}

// MsgID;ChatID;ClientID;MsgType;MessageTime;size;data 
inline Message* deserializeMessage(const std::string& msg_data) {
	std::istringstream in(msg_data);
	size_t msgID, chatID, clientID, messageTime, size;
	int type;
	std::string data;

	in >> msgID >> chatID >> clientID >> type >> messageTime >> size;
	in.get();
	data.resize(size);
	in.read(data.data(), size);

	Message* msg = new Message(msgID, chatID, clientID, (MessageType)type, messageTime);
	msg->setData(data.data(), data.size());
	return msg;
}

inline std::vector<Message*> deserializeMessages(std::pair<const char*, size_t> data) {
	const char* ptr = data.first;
	const char* end = ptr + data.second;

	if (!strcmp(data.first, "e1")) {
		std::cout << "Error while deserialize chats" << std::endl;
		return {};
	}

	if (!strcmp(data.first, "w1")) {
		std::cout << "No chats to deserialize" << std::endl;
		return {};
	}

	if (ptr + sizeof(size_t) > end) abort();

	size_t count;
	memcpy(&count, ptr, sizeof(count));
	ptr += sizeof(count);

	std::vector<Message*> out;

	for (size_t i = 0; i < count; i++) {
		if (ptr + sizeof(size_t) > end) abort();

		size_t size;
		memcpy(&size, ptr, sizeof(size));
		ptr += sizeof(size);

		if (ptr + size > end) abort();

		std::string chunk(ptr, size);
		ptr += size;

		out.push_back(deserializeMessage(chunk));
	}

	return out;
}