#include "nexoraUI.h"
#include "nexoraNETWORK.h"
#include <thread>
#include <iostream>

int main() {
	std::thread networkThread(initialNetwork);
	std::thread interfaceThread(initialInterface);
	interfaceThread.join();
	networkThread.join();

	cleanup();
}