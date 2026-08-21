#define API_LIB_EXPORT
#include "SUIutils.h"
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#elif defined(__linux__)
#include <X11/XKBlib.h>
#include <gtk/gtk.h>
inline Display* d = XOpenDisplay(nullptr);
Window root = DefaultRootWindow(d);
#endif

extern "C" const char* getLayout() {
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

extern "C" bool capsLock() {
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

extern "C" float GetMouseScreenPositionX() {
#ifdef _WIN32
	POINT p;
	GetCursorPos(&p);
	return static_cast<float>(p.x);
#elif defined(__linux__)
	int root_x, root_y;
	int win_x, win_y;
	unsigned int mask;
	Window child, root_return;
	XQueryPointer(d, root, &root_return, &child, &root_x, &root_y, &win_x, &win_y, &mask);
	return (float)root_x;
#endif
}

extern "C" float GetMouseScreenPositionY() {
#ifdef _WIN32
	POINT p;
	GetCursorPos(&p);
	return static_cast<float>(p.y);
#elif defined(__linux__)
	int root_x, root_y;
	int win_x, win_y;
	unsigned int mask;
	Window child, root_return;
	XQueryPointer(d, root, &root_return, &child, &root_x, &root_y, &win_x, &win_y, &mask);
	return (float)root_y;
#endif
}

#ifdef _WIN32
extern "C" const wchar_t* GetFile() {
	static std::wstring result;
	wchar_t filename[MAX_PATH] = L"";

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Images (*.jpg;*.jpeg;*.png)\0*.jpg;*.jpeg;*.png\0All files (*.*)\0*.*\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn)) {
		result = filename;
		return result.c_str();
	}
	return L"";
}
#elif defined(__linux__)
extern "C" const char* GetFile() {
	static std::string result;
	static bool inited = false;
	if (!inited) {
		inited = true;
		gtk_init(nullptr, nullptr);
	}

	GtkWidget* dialog = gtk_file_chooser_dialog_new(
		"Open Image",
		nullptr,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"_Cancel", GTK_RESPONSE_CANCEL,
		"_Open", GTK_RESPONSE_ACCEPT,
		nullptr
	);

	GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);

	GtkFileFilter* filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "Images (*.jpg, *.jpeg, *.png)");
	gtk_file_filter_add_pattern(filter, "*.jpg");
	gtk_file_filter_add_pattern(filter, "*.jpeg");
	gtk_file_filter_add_pattern(filter, "*.png");
	gtk_file_chooser_add_filter(chooser, filter);

	int response = gtk_dialog_run(GTK_DIALOG(dialog));

	if (response == GTK_RESPONSE_ACCEPT) {
		char* filename = gtk_file_chooser_get_filename(chooser);
		if (filename) {
			result = filename;
			g_free(filename);
		}
	} else {
		result = "";
	}

	gtk_widget_hide(dialog);
	gtk_widget_destroy(dialog);

	while (g_main_context_pending(nullptr)) {
		g_main_context_iteration(nullptr, FALSE);
	}

	return result.c_str();
}
#endif