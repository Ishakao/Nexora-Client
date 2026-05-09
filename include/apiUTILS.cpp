#include <string>
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
#elif defined(__linux__)
	#include <X11/XKBlib.h>

	inline Display* d = XOpenDisplay(nullptr);
	Window root = DefaultRootWindow(d);
#endif

std::string getLayout() {
	#ifdef _WIN32
		HKL layout = GetKeyboardLayout(0);
		if (LOWORD(layout) == 0x0409)
			return "EN";
		else if (LOWORD(layout) == 0x0419)
			return "RU";
		return "IDK";
	#elif defined(__linux__)
		if (!d) return "EN";

		XkbStateRec state;
		XkbGetState(d, XkbUseCoreKbd, &state);

		return (state.group == 1) ? "RU" : "EN";
	#endif
}

bool capsLock() {
	#ifdef _WIN32
		return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
	#elif defined(__linux__)
		if (!d) return false;

		unsigned int state = 0;
		XkbGetIndicatorState(d, XkbUseCoreKbd, &state);

		return (state & 0x01);
	#endif
	return false;
}

float GetMouseScreenPositionX() {
	#ifdef _WIN32
		POINT p;
		GetCursorPos(&p);
		return p.x;
	#elif defined(__linux__)
		int root_x, root_y;
		int win_x, win_y;
		unsigned int mask;
		Window child, root_return;

		XQueryPointer(d, root, &root_return, &child, &root_x, &root_y, &win_x, &win_y, &mask);

		return (float)root_x;
	#endif
}

float GetMouseScreenPositionY() {
	#ifdef _WIN32
		POINT p;
		GetCursorPos(&p);
		return p.y;
	#elif defined(__linux__)
		int root_x, root_y;
		int win_x, win_y;
		unsigned int mask;
		Window child, root_return;

		XQueryPointer(d, root, &root_return, &child, &root_x, &root_y, &win_x, &win_y, &mask);

		return (float)root_y;
	#endif
}