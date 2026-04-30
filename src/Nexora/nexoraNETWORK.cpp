#ifdef _WIN32
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <winsock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	using SOCKET = uint64_t;
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "nexoraNETWORK.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <future>
#include <functional>
#include <signal.h>

#ifdef __linux__
const char* IP = "127.0.0.1"; //  111.88.154.47
#elif _WIN32
const whcar_t* IP = L"127.0.0.1"; //  111.88.154.47
#endif
const short PORT = 7327;

namespace clientData {
	uint64_t serverSocket;
	const int maxBuffer = 4096;
	SSL* SSL_Data = nullptr;
	SSL_CTX* SSL_ctx = nullptr;
	uint32_t lastRequestId = 0;

	Client* clientObject = nullptr;
}

std::unordered_map<size_t, Chat*> LoadedChats;
std::vector<Client*> Clients;

void init_openssl() {
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}

SSL_CTX* create_context() {
	const SSL_METHOD* method = TLS_client_method();
	SSL_CTX* ctx = SSL_CTX_new(method);
	if (!ctx) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	return ctx;
}

int recv_exact(char* buf, size_t payload) {
	size_t writed = 0;

	while (writed < payload) {
		int l = SSL_read(clientData::SSL_Data, buf + writed,
			((payload - writed < clientData::maxBuffer) ? payload - writed : clientData::maxBuffer));

		if (l > 0) {
			writed += l;
			continue;
		}

		int err = SSL_get_error(clientData::SSL_Data, l);

		switch (err) {
		case SSL_ERROR_ZERO_RETURN:
			std::cout << "Client closed connection (TLS)" << std::endl;
			return -1;

		case SSL_ERROR_SSL:
			std::cout << "SSL protocol error" << std::endl;
			return -1;

		case SSL_ERROR_SYSCALL:
			std::cout << "Socket error / unexpected EOF" << std::endl;
			return -1;

		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
			std::cout << "Unexpected WANT_READ/WRITE" << std::endl;
			return -1;

		default:
			std::cout << "Unknown SSL error: " << err << std::endl;
			return -1;
		}
	}

	return 0;
}

int send_exact(const char* buf, size_t payload) {
	size_t sended = 0;
	while (sended < payload) {
		int l = SSL_write(clientData::SSL_Data, buf + sended, ((payload - sended < clientData::maxBuffer) ? payload - sended : clientData::maxBuffer));

		if (l > 0) {
			sended += l;
			continue;
		}

		int err = SSL_get_error(clientData::SSL_Data, l);

		switch (err) {
		case SSL_ERROR_ZERO_RETURN:
			std::cout << "Client closed connection (TLS)" << std::endl;
			return -1;

		case SSL_ERROR_SSL:
			std::cout << "SSL protocol error" << std::endl;
			return -1;

		case SSL_ERROR_SYSCALL:
			std::cout << "Socket error / unexpected EOF" << std::endl;
			return -1;

		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_WRITE:
			std::cout << "Unexpected WANT_READ/WRITE" << std::endl;
			return -1;

		default:
			std::cout << "Unknown SSL error: " << err << std::endl;
			return -1;
		}
	}

	return 0;
}

std::mutex reader_mutex;
static std::unordered_map<uint32_t, std::shared_ptr<std::promise<std::pair<char*, size_t>>>> reader_map;
static std::function<void(std::pair<networkHeader, std::pair<char*, size_t>>)> asyncReaderFunction = [](std::pair<networkHeader, std::pair<char*, size_t>> data) {};
static std::mutex asyncReaderMutex;

std::thread* readerThread = nullptr;

void data_reader() {
	try {
		readerThread = new std::thread([]() {
			while (true) {
				networkHeader outputHeader;

				int errcode = recv_exact((char*)&outputHeader, sizeof(networkHeader));
				if (errcode == -1) {
					std::cout << "fatal error 3" << std::endl;
					exit(3);
				}

				if (memcmp(&(outputHeader.magicNumber), &magicNumber, sizeof(magicNumber))) {
					continue;
				}

				char* buf = new char[outputHeader.payload];
				recv_exact(buf, outputHeader.payload);

				std::shared_ptr<std::promise<std::pair<char*, size_t>>> prom;

				{
					std::lock_guard<std::mutex> lock(reader_mutex);
					auto it = reader_map.find(outputHeader.request_id);
					if (it != reader_map.end()) {
						prom = it->second;
						reader_map.erase(it);
					}
				}

				if (prom) {
					prom->set_value({ buf, outputHeader.payload });
				} else {
					if (asyncReaderFunction) {
						asyncReaderMutex.lock();
						asyncReaderFunction({outputHeader, { buf, outputHeader.payload } });
						asyncReaderMutex.unlock();
					} else {
						delete[] buf;
					}
				}
			}
		});

	} catch (const char* ex) {
		std::cout << "Exception: " << ex << std::endl;
	}
}

std::pair<char*, size_t> wait_data(uint32_t id) {
	auto prom = std::make_shared<std::promise<std::pair<char*, size_t>>>();
	auto fut = prom->get_future();

	{
		std::lock_guard<std::mutex> lock(reader_mutex);
		reader_map[id] = prom;
	}

	auto data = fut.get();
	return data;
}

std::pair<const char*, size_t> get_data(const char* inputData, QueryType type, size_t size) {
	networkHeader inputHeader;
	inputHeader.payload = size;
	inputHeader.type = type;
	inputHeader.request_id = clientData::lastRequestId++;

	int send_err1 = send_exact((char*)&inputHeader, sizeof(inputHeader));
	if (send_err1 == -1) {
		std::cout << "fatal error 1" << std::endl;
		return { nullptr, 0 };
	}

	int send_err2 = send_exact(inputData, size);

	if (send_err2 == -1) {
		std::cout << "fatal error 2" << std::endl;
		return { nullptr, 0 };
	}

	std::pair<const char*, size_t> d = wait_data(inputHeader.request_id);

	return d;
}

void serverDataFunction(std::function<void(std::pair<networkHeader, std::pair<char*, size_t>>)> f) {
	asyncReaderMutex.lock();
	asyncReaderFunction = f;
	asyncReaderMutex.unlock();
}

void cleanup() {
	SSL_shutdown(clientData::SSL_Data);
	SSL_free(clientData::SSL_Data);
	SSL_CTX_free(clientData::SSL_ctx);
	EVP_cleanup();
	
	#ifdef _WIN32
		closesocket(clientData::serverSocket);
		WSACleanup();
	#elif __linux__
		close(clientData::serverSocket);
	#endif

	try {
		delete readerThread;
	} catch (const char* ex) {
		std::cout << "Exception " << ex << std::endl;
	}
}

void initialNetwork() {
    signal(SIGPIPE, SIG_IGN);
	init_openssl();
	clientData::SSL_ctx = create_context();
#ifdef _WIN32
	WSAData wsa;
	int w = WSAStartup(MAKEWORD(2, 2), &wsa);
	clientData::serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	InetPton(AF_INET, IP, &addr.sin_addr);
	connect(clientData::serverSocket, (sockaddr*)&addr, sizeof(addr));
#elif __linux__
	clientData::serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientData::serverSocket < 0) {
		perror("socket");
		exit(1);
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	inet_pton(AF_INET, IP, &addr.sin_addr);
	connect(clientData::serverSocket, (sockaddr*)&addr, sizeof(addr));
#endif
	clientData::SSL_Data = SSL_new(clientData::SSL_ctx);
	if (!clientData::SSL_Data) {
		ERR_print_errors_fp(stderr);
		exit(4);
	}

	SSL_set_fd(clientData::SSL_Data, clientData::serverSocket);
	if (SSL_connect(clientData::SSL_Data) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(5);
	}

	data_reader();
}

Client** getClientPtr() {
	return &clientData::clientObject;
}