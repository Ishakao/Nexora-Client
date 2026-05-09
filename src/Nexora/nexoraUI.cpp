#include "../../include/simpleUI.h"
#include "nexoraNETWORK.h"
#include <thread>
#include <mutex>
#include <string>
#define _SHOW_NEXORA
#define BAR_SIZE 25
#define CHAT_SIZE 60
const int BAR_BUTTONS_SIZE = 45;
const Color DEFAULT_BACKGROUND = {42,42,42,255};
const Color DEFAULT_TEXT = { 226,200,54,255 };
const int minW = 560;
const int minH = 520;

Instance* StartInstance = nullptr;
bool Resizing_EW = false;
Object2D* ChatTemplate = nullptr;
ScrollFrame* CurrentChatScroll = nullptr;
size_t CurrentChatID = 0;
Object2D* settingsFrame = nullptr;

static inline bool Authenticated = false;
static inline bool ChatsUpdated = false;
unsigned long clientId = 0;

enum WhereOnUI {
	AUTH = 0,
	GENERAL,
	PROFILE,
	SETTINGS,
	LEFT_MENU
};

namespace GlobalStates {
	WhereOnUI UIstate = AUTH;
};

std::mutex ImagesLoadingMtx;
std::unordered_map<std::string, Image> loadedImages;
void loadImage(const std::string& name, const std::string& path) {
	ImagesLoadingMtx.lock();

	loadedImages.insert({name, LoadImage(path.c_str())});

	ImagesLoadingMtx.unlock();
}

void unloadImage(const std::string& name) {
	ImagesLoadingMtx.lock();

	auto it = loadedImages.find(name);
	if (it != loadedImages.end()) {
		UnloadImage(it->second);
		loadedImages.erase(it);
	}

	ImagesLoadingMtx.unlock();
}

Image getImage(const std::string& name) {
	auto it = loadedImages.find(name);
	if (it != loadedImages.end()) {
		return it->second;
	}

	throw "Image not found";
}

std::string long_to_string(size_t l) {
	std::string stringID = "";
	size_t num = l;
	while (num >= 1) {
		stringID += '0' + (num % 10);
		num /= 10;
	}
	std::reverse(stringID.begin(), stringID.end());
	return stringID;
}

size_t string_to_size_t(std::string s) {
	size_t n = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] < '0' or s[i] > '9') {
			break;
		}

		n *= 10;
		n += s[i] - '0';
	}

	return n;
}

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


Chat* deserializeChat(std::pair<const char*, size_t> data) {
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

std::vector<Chat*> deserializeChats(std::pair<const char*, size_t> data) {
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

	int to_read = sizeof(size_t);
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

	std::cout << 4 << std::endl;
	return result;
}

Client* deserializeClient(const std::string& data) {
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


std::mutex loadChatsMutex;

void loadChats(const size_t chatID, std::function<void(void)> f) { // chatID == 0: all chats
	if (chatID) {
		std::string stringID = long_to_string(chatID);
		char* input = new char[stringID.size()+1];
		memcpy(input, stringID.data(), stringID.size());
		input[stringID.size()] = '\0';

		AsyncData* chat = new AsyncData(input, GET_CHAT, stringID.size());
		chat->Completed([f, input, chatID](std::pair<const char*, size_t> data) {
			Chat* ch = deserializeChat(data);
			size_t id = ch->ChatID;
			if (id == chatID) {
				std::cout << "Chat " << id << " loaded successfully" << std::endl;
			}
			loadChatsMutex.lock();
			auto it = LoadedChats.find(id);
			if (it != LoadedChats.end()) {
				it->second->Name = ch->Name;
				it->second->OwnerID = ch->OwnerID;
				delete ch;
			} else {
				LoadedChats.insert({ id, ch });
			}
			loadChatsMutex.unlock();

			f();
			delete[] input;
			delete[] data.first;
		});
		chat->send();
	} else {
		AsyncData* chat = new AsyncData(nullptr, GET_CHATS, 0);
		chat->Completed([f](std::pair<const char*, size_t> data) {
			if (!data.first) {
				std::cout << "No chats data" << std::endl;
				return;
			} else {
				std::cout << "Chats data: " << data.first << " | " << data.second << std::endl;
			}

			if (!memcmp(data.first, "e1", 3)) {
				std::cout << "Chats e1 error" << std::endl;
				delete[] data.first;
				return;
			}

			std::vector<Chat*> chats = deserializeChats(data);
			if (chats.size()) {
				loadChatsMutex.lock();
				for (Chat* c : chats) {
					size_t id = c->ChatID;
					auto it = LoadedChats.find(id);
					if (it != LoadedChats.end()) {
						it->second->Name = c->Name;
						it->second->OwnerID = c->OwnerID;
						delete c;
					}
					else {
						LoadedChats.insert({ id, c });
					}
				}
				loadChatsMutex.unlock();
			}

			f();
			delete[] data.first;
		});
		chat->send();
	}
}

void changeCurrentChat(const size_t id) {
	loadChatsMutex.lock();
	auto it = LoadedChats.find(id);
	if (it != LoadedChats.end()) {
		Chat* chat = it->second;
		loadChatsMutex.unlock();
	}
	else {
		loadChatsMutex.unlock();
		loadChats(id, [id]() {
			changeCurrentChat(id);
		});
	}
}

size_t loadMessages(const size_t chatID, const size_t lastLoadedMsgId) {
	return 0;
}

int initUpperBar() {
	static bool shiftWindow = false;
	static Vector2 dragOffset = { 0, 0 };

	Object2D* upperBar = new Object2D(StartInstance);
	upperBar->ZIndex = 1000;
	upperBar->Name = "Upper Bar";
	upperBar->Active = true;
	upperBar->Position = { 0,0 };
	upperBar->Size = { 1, 0 };
	upperBar->SizeOFFSET = { 0, BAR_SIZE };
	upperBar->BackgroundColor = { 42,42,42,255 };
	static bool dragging = false;
	static Vector2 startMouseScreen = { 0,0 };
	static Vector2 startWinPos = { 0,0 };

	static double cooldown = 0;
	static bool tapped = false;

	upperBar->SetMouse1HoldStart([](Object2D* t) {
		if (cooldown > 0.3) tapped = false;
		if (!IsWindowMaximized() and !tapped) {
			tapped = true;
			cooldown = 0;
		} else if (!IsWindowMaximized() and cooldown < 0.3 and tapped) {
			MaximizeWindow();
			tapped = false;
			return;
		}

		if (GetMousePosition().y <= 2 and !IsWindowMaximized()) return;
		startMouseScreen = GetMouseScreenPosition();
		startWinPos = GetWindowPosition();
		if (IsWindowMaximized()) {
			RestoreWindow();
		}
		dragging = true;
	});
	upperBar->SetMouse1HoldEnd([](Object2D* t) { dragging = false; });
	upperBar->SetForTick([]() {
		double tickCooldown = 1.0 / GetMonitorRefreshRate(GetCurrentMonitor());
		static double currentCooldown = 0;
		currentCooldown += dt;
		if (tapped) cooldown += dt;

		static bool pressed = false;
		static bool released = false;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pressed = true;
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) released = true;

		if (released) dragging = false;

		if (currentCooldown < tickCooldown) return;

		currentCooldown = 0;

		if (IsWindowMaximized()) {
			if (Resizing_EW)
				SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
			else
				SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			return;
		}

		if (dragging) {
			Vector2 m = GetMouseScreenPosition();
			Vector2 delta = { m.x - startMouseScreen.x, m.y - startMouseScreen.y };

			SUI_SetWindowPosition((int)roundf(startWinPos.x + delta.x), (int)roundf(startWinPos.y + delta.y));
		} else {
			static bool resizing = false;
			static int resizeMask = 0;
			static Vector2 startMouseScreen = { 0,0 };
			static Vector2 startWinPos = { 0,0 };
			static int startW = 0, startH = 0;

			const int border = 6;

			Vector2 mouse = GetMousePosition();
			int w = GetScreenWidth();
			int h = GetScreenHeight();

			bool left = mouse.x <= border;
			bool right = mouse.x >= w - border;
			bool top = mouse.y <= 2;
			bool bottom = mouse.y >= h - border;

			if ((left and top) or (right and bottom))
				SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
			else if ((right and top) or (left and bottom))
				SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
			else if (left or right)
				SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
			else if (top or bottom)
				SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
			else if (Resizing_EW)
				SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
			else
				SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if (!resizing and pressed and (left or right or top or bottom)) {
				resizing = true;

				resizeMask = 0;
				if (left) resizeMask |= 1;
				if (right) resizeMask |= 2;
				if (top) resizeMask |= 4;
				if (bottom) resizeMask |= 8;

				Vector2 winPos = GetWindowPosition();
				Vector2 mouseInWin = GetMousePosition();
				startMouseScreen = { winPos.x + mouseInWin.x, winPos.y + mouseInWin.y };

				startWinPos = winPos;
				startW = w;
				startH = h;
			}

			if (resizing and released) {
				resizing = false;
				resizeMask = 0;
			}

			if (resizing) {
				Vector2 winPosNow = GetWindowPosition();
				Vector2 mouseInWinNow = GetMousePosition();
				Vector2 mouseScreen = { winPosNow.x + mouseInWinNow.x, winPosNow.y + mouseInWinNow.y };

				float dx = mouseScreen.x - startMouseScreen.x;
				float dy = mouseScreen.y - startMouseScreen.y;

				int newW = startW;
				int newH = startH;

				float newX = startWinPos.x;
				float newY = startWinPos.y;

				if (resizeMask & 2) {
					newW = (int)lroundf((float)startW + dx);
				}
				if (resizeMask & 1) {
					newW = (int)lroundf((float)startW - dx);
					newX = startWinPos.x + dx;
				}
				if (resizeMask & 8) {
					newH = (int)lroundf((float)startH + dy);
				}
				if (resizeMask & 4) {
					newH = (int)lroundf((float)startH - dy);
					newY = startWinPos.y + dy;
				}
				if (newW < minW) {
					if (resizeMask & 1) newX -= (minW - newW);
					newW = minW;
				}
				if (newH < minH) {
					if (resizeMask & 4) newY -= (minH - newH);
					newH = minH;
				}

				SUI_SetWindowPosition((int)lroundf(newX), (int)lroundf(newY));
				SUI_SetWindowSize(newW, newH);
			}
		}

		if (pressed) pressed = false;
		if (released) released = false;
	});

	ImageLabel* icon = new ImageLabel(upperBar);
	icon->ZIndex = 2;
	icon->BackgroundTransparency = 1;
	icon->setImage(getImage("app_icon"));
	icon->Overlay = FIT;
	icon->Position = { 0,0 };
	icon->Size = { 0, 1 };
	icon->SizeOFFSET.x = 50;

	#ifdef _SHOW_NEXORA
		TextLabel* name = new TextLabel(upperBar);
		name->Position = { 0, 0 };
		name->PositionOFFSET.x = 60;
		name->Size = { 0, 1 };
		name->SizeOFFSET.x = 200;
		name->Text = "Nexora";
		name->TextAnchor = TextAnchorEnum::W;
		name->font = "SegoeB";
		name->BackgroundTransparency = 1;
		name->TextColor = DEFAULT_TEXT;
	#endif

	ImageLabel* Exit = new ImageLabel(upperBar);
	Exit->Position = { 1, 0.6 };
	Exit->AnchorPosition = { 0,0.5 };
	Exit->Size = { 0, 0.5 };
	Exit->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	Exit->PositionOFFSET.x = -BAR_BUTTONS_SIZE;
	Exit->BackgroundTransparency = 1;
	Exit->Overlay = FIT;
	Exit->setImage(getImage("exit"));
	Exit->ImageColor = { 255,100,100,255 };
	Object2D* ExitButton = new Object2D(upperBar);
	ExitButton->ZIndex = 3;
	ExitButton->Position = { 1, 0.6 };
	ExitButton->AnchorPosition = { 0,0.5 };
	ExitButton->Size = { 0, 1 };
	ExitButton->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	ExitButton->PositionOFFSET.x = -BAR_BUTTONS_SIZE;
	ExitButton->BackgroundTransparency = 1;
	ExitButton->BackgroundColor = { 255,255,255,255 };
	ExitButton->Active = true;
	ExitButton->SetMouseEnter([ExitButton](Object2D* t) { ExitButton->BackgroundTransparency = 0.8; });
	ExitButton->SetMouseLeave([ExitButton](Object2D* t) { ExitButton->BackgroundTransparency = 1; });
	ExitButton->SetMouse1HoldEnd([](Object2D* t) { 
		programRunning = false; 
	});

	ImageLabel* Window = new ImageLabel(upperBar);
	Window->Position = { 1, 0.6 };
	Window->AnchorPosition = { 0,0.5 };
	Window->Size = { 0, 0.5 };
	Window->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	Window->PositionOFFSET.x = -BAR_BUTTONS_SIZE*2 - 1;
	Window->BackgroundTransparency = 1;
	Window->Overlay = FIT;
	Window->setImage(getImage("window"));
	Window->ImageColor = { 255,255,255,255 };
	Object2D* WindowButton = new Object2D(upperBar);
	WindowButton->ZIndex = 3;
	WindowButton->Position = { 1, 0.6 };
	WindowButton->AnchorPosition = { 0,0.5 };
	WindowButton->Size = { 0, 1 };
	WindowButton->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	WindowButton->PositionOFFSET.x = -BAR_BUTTONS_SIZE*2 - 1;
	WindowButton->BackgroundTransparency = 1;
	WindowButton->BackgroundColor = { 255,255,255,255 };
	WindowButton->Active = true;
	WindowButton->SetMouseEnter([WindowButton](Object2D* t) { WindowButton->BackgroundTransparency = 0.8; });
	WindowButton->SetMouseLeave([WindowButton](Object2D* t) { WindowButton->BackgroundTransparency = 1; });
	WindowButton->SetMouse1HoldEnd([](Object2D* t) {
		if (IsWindowMaximized())
			RestoreWindow();
		else
			MaximizeWindow();
	});

	ImageLabel* Hide = new ImageLabel(upperBar);
	Hide->Position = { 1, 0.5 };
	Hide->AnchorPosition = { 0,0.5 };
	Hide->Size = { 0, 0.6 };
	Hide->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	Hide->PositionOFFSET.x = -BAR_BUTTONS_SIZE * 3 - 2;
	Hide->BackgroundTransparency = 1;
	Hide->Overlay = FIT;
	Hide->setImage(getImage("hide"));
	Hide->ImageColor = { 255,255,255,255 };
	Object2D* HideButton = new Object2D(upperBar);
	HideButton->ZIndex = 3;
	HideButton->Position = { 1, 0.6 };
	HideButton->AnchorPosition = { 0,0.5 };
	HideButton->Size = { 0, 1 };
	HideButton->SizeOFFSET.x = BAR_BUTTONS_SIZE;
	HideButton->PositionOFFSET.x = -BAR_BUTTONS_SIZE * 3 - 2;
	HideButton->BackgroundTransparency = 1;
	HideButton->BackgroundColor = { 255,255,255,255 };
	HideButton->Active = true;
	HideButton->SetMouseEnter([HideButton](Object2D* t) { HideButton->BackgroundTransparency = 0.8; });
	HideButton->SetMouseLeave([HideButton](Object2D* t) { HideButton->BackgroundTransparency = 1; });
	HideButton->SetMouse1HoldEnd([](Object2D* t) {
		MinimizeWindow();
	});

	return 0;
}

int initAuth() {
	Object2D* AuthFrame = new Object2D(StartInstance);
	AuthFrame->Name = "Auth Frame";
	AuthFrame->PositionOFFSET = { 0, BAR_SIZE };
	AuthFrame->Size = { 1, 1 };
	AuthFrame->SizeOFFSET = { 0, -25 };
	AuthFrame->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.2);
	AuthFrame->ZIndex = 15;

	ImageLabel* AuthBackground = new ImageLabel(AuthFrame);
	AuthBackground->Size = { 1.05,1.05 };
	AuthBackground->AnchorPosition = { 0.5,0.5 };
	AuthBackground->Position = { 0.5,0.5 };
	AuthBackground->BackgroundTransparency = 1;
	AuthBackground->ImageColor = DEFAULT_TEXT;
	AuthBackground->Name = "AuthBackground";
	AuthBackground->setImage(getImage("auth_background"));
	AuthBackground->ImageTransparency = 0.5;
	AuthBackground->Overlay = CROP;
	AuthBackground->SetForTick([AuthBackground]() {
		static Vector2 lastMouse{};
		Vector2 mousePos = GetMousePosition();

		if (mousePos.x != lastMouse.x or mousePos.y != lastMouse.y) {
			lastMouse = mousePos;
			Vector2 mouseRelative = { mousePos.x / winWidth, mousePos.y / winHeight };
			float x = 0.025f * mouseRelative.x;
			float y = 0.025f * mouseRelative.y;
			AuthBackground->Position.x = 0.5 - x;
			AuthBackground->Position.y = 0.5 - y;
		}
	});

	ScrollFrame* Scissors = new ScrollFrame(AuthFrame);
	Scissors->BackgroundTransparency = 1;
	Scissors->AnchorPosition = { 0.5,0.5 };
	Scissors->Size = { 0, 0 };
	Scissors->SizeOFFSET = { 540, 420 };
	Scissors->Position = { 0.5, 0.585 };
	Scissors->ScrollEnabled = false;
	Scissors->SliderTransparency = 1;

	Object2D* SignInFrame = new Object2D(Scissors);
	SignInFrame->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 0.8);
	SignInFrame->BorderColor = mulColor(DEFAULT_BACKGROUND, 1.5);
	SignInFrame->BorderThickness = 3;
	SignInFrame->Roundness = 0.15;
	SignInFrame->AnchorPosition = { 0.5,0.5 };
	SignInFrame->Size = { 0.75, 0.9 };
	SignInFrame->Position = { -0.4, 0.5 };
	SignInFrame->Segments = 10;

	Object2D* SignUpFrame = new Object2D(Scissors);
	SignUpFrame->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 0.8);
	SignUpFrame->BorderColor = mulColor(DEFAULT_BACKGROUND, 1.5);
	SignUpFrame->BorderThickness = 3;
	SignUpFrame->Roundness = 0.15;
	SignUpFrame->AnchorPosition = { 0.5,0.5 };
	SignUpFrame->Size = { 0.75, 0.9 };
	SignUpFrame->Position = { 0.5, 0.5 };
	SignUpFrame->Segments = 10;

	TextLabel* SignIn = new TextLabel(AuthFrame);
	SignIn->Size = { 0, 0 };
	SignIn->SizeOFFSET = { 135, 50 };
	SignIn->Position = { 0.5, 0.585 };
	SignIn->PositionOFFSET = { -80, -230 };
	SignIn->AnchorPosition = { 0.5,0.5 };
	SignIn->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1);
	SignIn->BorderColor = mulColor(DEFAULT_BACKGROUND, 1.4);
	SignIn->BorderThickness = 2;
	SignIn->Text = " Sign in ";
	SignIn->font = "SegoeB";
	SignIn->TextColor = mulColor(DEFAULT_TEXT, 0.6);
	SignIn->Name = "SingIn";
	SignIn->Roundness = 0.2;
	SignIn->Active = true;
	SignIn->SetMouseEnter([SignIn](Object2D* t) {
		Animate::Create(&SignIn->SizeOFFSET, 0.125f, { 170, 65 });
		Animate::Create(&SignIn->BorderColor, 0.15f, mulColor(DEFAULT_BACKGROUND, 1.6));
		Animate::Create(&SignIn->BackgroundColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 1.2));
	});
	SignIn->SetMouseLeave([SignIn](Object2D* t) {
		Animate::Create(&SignIn->SizeOFFSET, 0.125f, { 135, 50 });
		Animate::Create(&SignIn->BorderColor, 0.15f, mulColor(DEFAULT_BACKGROUND, 1.4));
		Animate::Create(&SignIn->BackgroundColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 1));
	});

	TextLabel* SignUp = new TextLabel(AuthFrame);
	SignUp->Size = { 0, 0 };
	SignUp->SizeOFFSET = { 135, 50 };
	SignUp->Position = { 0.5, 0.585 };
	SignUp->PositionOFFSET = { 80, -250 };
	SignUp->AnchorPosition = { 0.5,0.5 };
	SignUp->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1);
	SignUp->BorderColor = mulColor(DEFAULT_BACKGROUND, 1.4);
	SignUp->BorderThickness = 2;
	SignUp->Text = " Sign up ";
	SignUp->font = "SegoeB";
	SignUp->TextColor = mulColor(DEFAULT_TEXT, 1);
	SignUp->Name = "SignUp";
	SignUp->Roundness = 0.2;
	SignUp->Active = true;
	SignUp->SetMouseEnter([SignUp](Object2D* t) {
		Animate::Create(&SignUp->SizeOFFSET, 0.125f, { 170, 65 });
		Animate::Create(&SignUp->BorderColor, 0.15f, mulColor(DEFAULT_BACKGROUND, 1.6));
		Animate::Create(&SignUp->BackgroundColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 1.2));
	});
	SignUp->SetMouseLeave([SignUp](Object2D* t) {
		Animate::Create(&SignUp->SizeOFFSET, 0.125f, { 135, 50 });
		Animate::Create(&SignUp->BorderColor, 0.15f, mulColor(DEFAULT_BACKGROUND, 1.4));
		Animate::Create(&SignUp->BackgroundColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 1));
	});

	SignIn->SetMouseClick1([SignIn, SignUp, SignInFrame, SignUpFrame](Object2D* t) {
		Animate::Create(&SignUp->TextColor, 0.125f, mulColor(DEFAULT_TEXT, 0.6));
		Animate::Create(&SignIn->TextColor, 0.125f, mulColor(DEFAULT_TEXT, 1));

		Animate::Create(&SignIn->PositionOFFSET.y, 0.125f, -250);
		Animate::Create(&SignUp->PositionOFFSET.y, 0.125f, -230);

		Animate::Create(&SignInFrame->Position.x, 0.2f, 0.5f, Animate::Circular, Animate::Out);
		Animate::Create(&SignUpFrame->Position.x, 0.2f, 1.6f, Animate::Circular, Animate::Out);
	});

	SignUp->SetMouseClick1([SignIn, SignUp, SignInFrame, SignUpFrame](Object2D* t) {
		Animate::Create(&SignIn->TextColor, 0.125f, mulColor(DEFAULT_TEXT, 0.6));
		Animate::Create(&SignUp->TextColor, 0.125f, mulColor(DEFAULT_TEXT, 1));
	
		Animate::Create(&SignIn->PositionOFFSET.y, 0.125f, -230);
		Animate::Create(&SignUp->PositionOFFSET.y, 0.125f, -250);

		Animate::Create(&SignInFrame->Position.x, 0.2f, -0.4f, Animate::Circular, Animate::Out);
		Animate::Create(&SignUpFrame->Position.x, 0.2f, 0.5f, Animate::Circular, Animate::Out);
	});

	TextLabel* SignInInternal = new TextLabel(SignInFrame);
	SignInInternal->Size = { 0.5, 0.12 };
	SignInInternal->Position = { 0.5, 0 };
	SignInInternal->AnchorPosition = { 0.5,0 };
	SignInInternal->BackgroundTransparency = 1;
	SignInInternal->Text = "Sign in";
	SignInInternal->font = "SegoeB";
	SignInInternal->TextColor = mulColor(DEFAULT_TEXT, 1);
	SignInInternal->Name = "SingIn";

	TextBox* Login = new TextBox(SignInFrame);
	Login->Size = { 0.9, 0.1 };
	Login->Position = { 0.5, 0.3 };
	Login->AnchorPosition = { 0.5,0.5 };
	Login->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.7);
	Login->TextSize = -1;
	Login->maxSymbols = 20;
	Login->PlaceholderText = "Account login";
	Login->TextAnchor = TextAnchorEnum::W;
	Login->font = "SegoeB";
	Login->TextColor = DEFAULT_TEXT;
	Login->CursorColor = mulColor(DEFAULT_TEXT, 0.7);
	Login->Spacing = 0;
	Login->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	Login->BorderThickness = 4;
	Login->ClearOnClick = false;
	Login->CursorSize = 2;
	Login->AllowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_";

	TextBox* Password = new TextBox(SignInFrame);
	Password->Size = { 0.8, 0.1 };
	Password->Position = { 0.45, 0.44 };
	Password->AnchorPosition = { 0.5,0.5 };
	Password->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.7);
	Password->TextSize = -1;
	Password->maxSymbols = 20;
	Password->PlaceholderText = "Account password";
	Password->TextAnchor = TextAnchorEnum::W;
	Password->font = "SegoeB";
	Password->TextColor = DEFAULT_TEXT;
	Password->CursorColor = mulColor(DEFAULT_TEXT, 0.7);
	Password->Spacing = 0;
	Password->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	Password->BorderThickness = 4;
	Password->HideText = '*';
	Password->ClearOnClick = false;
	Password->CursorSize = 2;
	Password->AllowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_!+-=#$%^&*().,/\\`~[]{}";

	ImageLabel* TextBoxEye = new ImageLabel(SignInFrame);
	TextBoxEye->setImage(getImage("textbox_eye"));
	TextBoxEye->Size = {0.085, 0.085};
	TextBoxEye->Position = {0.915, 0.44};
	TextBoxEye->Active = true;
	TextBoxEye->AnchorPosition = {0.5,0.5};
	TextBoxEye->BackgroundTransparency = 1;
	TextBoxEye->ImageColor = DEFAULT_TEXT;
	TextBoxEye->SetMouse1HoldEnd([Password, TextBoxEye](Object2D* t){
		Password->HideText = (Password->HideText == '\0' ? '*' : '\0');
		TextBoxEye->ImageColor = mulColor(DEFAULT_TEXT, (Password->HideText != '\0') ? 1 : 0.8);
	});
	TextBoxEye->SetMouseEnter([TextBoxEye](Object2D* t){
		Animate::Create(&TextBoxEye->Size, 0.125, {0.105, 0.105});
	});
	TextBoxEye->SetMouseLeave([TextBoxEye](Object2D* t){
		Animate::Create(&TextBoxEye->Size, 0.125, {0.085, 0.085});
	});

	TextLabel* DataIncorrect = new TextLabel(SignInFrame);
	DataIncorrect->Size = { 0.9, 0.06 };
	DataIncorrect->Position = { 0.5, 0.55 };
	DataIncorrect->AnchorPosition = { 0.5,0.5 };
	DataIncorrect->Text = "Login Incorrect";
	DataIncorrect->TextColor = { 255, 40, 40, 255 };
	DataIncorrect->font = "SegoeB";
	DataIncorrect->BackgroundTransparency = 1;
	DataIncorrect->TextTransparency = 1;

	TextLabel* CheckLogin = new TextLabel(SignInFrame);
	CheckLogin->Position = {0.5, 0.65};
	CheckLogin->Size = { 0.4, 0.125 };
	CheckLogin->AnchorPosition = { 0.5, 0.5 };
	CheckLogin->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.5);
	CheckLogin->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	CheckLogin->BorderThickness = 4;
	CheckLogin->font = "SegoeB";
	CheckLogin->Active = true;
	CheckLogin->Text = "Log in";
	CheckLogin->Roundness = 0.15;
	CheckLogin->Segments = 7;
	CheckLogin->TextColor = mulColor(DEFAULT_TEXT, 0.8);
	CheckLogin->SetMouseEnter([CheckLogin](Object2D* t) {
		Animate::Create(&CheckLogin->Size, 0.1f, {0.5, 0.135});
		Animate::Create(&CheckLogin->BorderColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 0.8));
		Animate::Create(&CheckLogin->TextColor, 0.1f, mulColor(DEFAULT_TEXT, 1.1));
	});
	CheckLogin->SetMouseLeave([CheckLogin](Object2D* t) {
		Animate::Create(&CheckLogin->Size, 0.1f, { 0.4, 0.125 });
		Animate::Create(&CheckLogin->BorderColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 0.5));
		Animate::Create(&CheckLogin->TextColor, 0.1f, mulColor(DEFAULT_TEXT, 0.8));
	});
	CheckLogin->SetMouse1HoldEnd([DataIncorrect, CheckLogin, Login, Password](Object2D* t) {
		if (Authenticated) return;
		if (Login->Text.empty() or Password->Text.empty()) return;

		DataIncorrect->TextTransparency = 1;

		size_t size = Login->Text.size() + Password->Text.size() + 1;

		char* input = new char[size];
		memcpy(input, Login->Text.c_str(), Login->Text.size());
		input[Login->Text.size()] = '|';
		memcpy(input + Login->Text.size() + 1, Password->Text.c_str(), Password->Text.size());

		AsyncData* data = new AsyncData(input, QueryType::SignIn, size);
		data->deleteOnCompleted = false;
		data->Completed([DataIncorrect, Login, Password, CheckLogin, data, input](std::pair<const char*, size_t> output) {
			CheckLogin->Active = true;
			CheckLogin->TextColor = mulColor(DEFAULT_TEXT, 0.8);
			Login->Active = true;
			Login->TextColor = mulColor(DEFAULT_TEXT, 1);
			Password->Active = true;
			Password->TextColor = mulColor(DEFAULT_TEXT, 1);
			
			delete[]input;

			if (output.first) {
				if (!strcmp(output.first, "-1")) {
					std::cout << "Header is incorrect" << std::endl;
					delete data;
					return;
				}

				if (!strcmp(output.first, "e1")) {
					DataIncorrect->Text = "Data is incorrect";
					DataIncorrect->TextTransparency = 0;
				} else if (!strcmp(output.first, "e2")) {
					DataIncorrect->Text = "Login is incorrect";
					DataIncorrect->TextTransparency = 0;
				} else if (!strcmp(output.first, "e3")) {
					DataIncorrect->Text = "Password is incorrect";
					DataIncorrect->TextTransparency = 0;
				} else {
					unsigned long id = 0;
					for (int i = 0; i < output.second; i++) {
						if (output.first[i] == '\0') break;
						id *= 10;
						id += output.first[i] - '0';
					}
					clientId = id;
					Authenticated = true;
					loadChats(0, []() {
						ChatsUpdated = true;
					});
					DataIncorrect->TextTransparency = 1;
					
					std::string string_id = long_to_string(id);
					char* input2 = new char[string_id.size() + 1];
					memcpy(input2, string_id.data(), string_id.size());
					input2[string_id.size()] = '\0';

					AsyncData* data2 = new AsyncData(input2, QueryType::GET_USER_INFO, string_id.size());
					data2->Completed([input2, data](std::pair<const char*, size_t> output1){
						delete[] input2;

						if (!output1.first) {
							delete data;
							return;
						}

						if (!strcmp(output1.first, "e1")) {
							std::cout << "No client with id " << input2 << std::endl;
							delete[] output1.first;
							delete data;
							return;
						}

						if (*getClientPtr()) {
							delete* getClientPtr();
						}
							
						*getClientPtr() = deserializeClient(std::string(output1.first));
						delete[] output1.first;
						delete data;
					});

					data2->send();
				}
				delete[]output.first;
			} else {
				DataIncorrect->Text = "Server doesn't answer";
				DataIncorrect->TextTransparency = 0;
				delete data;
			}
		});
		data->Sended([Login, Password, CheckLogin, DataIncorrect]() {
			DataIncorrect->TextTransparency = 1;
			CheckLogin->Active = false;
			Login->Active = false;
			Password->Active = false;
		});
		data->send();
	});

	/*************
	*   Sign Up  *
	*************/

	TextLabel* SignUpInternal = new TextLabel(SignUpFrame);
	SignUpInternal->Size = { 0.5, 0.12 };
	SignUpInternal->Position = { 0.5, 0 };
	SignUpInternal->AnchorPosition = { 0.5,0 };
	SignUpInternal->BackgroundTransparency = 1;
	SignUpInternal->Text = "Sign up";
	SignUpInternal->font = "SegoeB";
	SignUpInternal->TextColor = mulColor(DEFAULT_TEXT, 1);
	SignUpInternal->Name = "SingIn";

	TextBox* Login2 = new TextBox(SignUpFrame);
	Login2->Size = { 0.9, 0.1 };
	Login2->Position = { 0.5, 0.3 };
	Login2->AnchorPosition = { 0.5,0.5 };
	Login2->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.7);
	Login2->TextSize = -1;
	Login2->maxSymbols = 20;
	Login2->PlaceholderText = "New account login";
	Login2->TextAnchor = TextAnchorEnum::W;
	Login2->font = "SegoeB";
	Login2->TextColor = DEFAULT_TEXT;
	Login2->CursorColor = mulColor(DEFAULT_TEXT, 0.7);
	Login2->Spacing = 0;
	Login2->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	Login2->BorderThickness = 4;
	Login2->ClearOnClick = false;
	Login2->CursorSize = 2;
	Login2->AllowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_";

	TextBox* Password2 = new TextBox(SignUpFrame);
	Password2->Size = { 0.8, 0.1 };
	Password2->Position = { 0.45, 0.44 };
	Password2->AnchorPosition = { 0.5,0.5 };
	Password2->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.7);
	Password2->TextSize = -1;
	Password2->maxSymbols = 20;
	Password2->PlaceholderText = "Password";
	Password2->TextAnchor = TextAnchorEnum::W;
	Password2->font = "SegoeB";
	Password2->TextColor = DEFAULT_TEXT;
	Password2->CursorColor = mulColor(DEFAULT_TEXT, 0.7);
	Password2->Spacing = 0;
	Password2->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	Password2->BorderThickness = 4;
	Password2->HideText = '*';
	Password2->ClearOnClick = false;
	Password2->CursorSize = 2;
	Password2->AllowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_!+-=#$%^&*().,/\\`~[]{}";

	TextBox* Password22 = new TextBox(SignUpFrame);
	Password22->Size = { 0.8, 0.1 };
	Password22->Position = { 0.45, 0.57 };
	Password22->AnchorPosition = { 0.5,0.5 };
	Password22->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.7);
	Password22->TextSize = -1;
	Password22->maxSymbols = 20;

	Password22->PlaceholderText = "Confirm password";
	Password22->TextAnchor = TextAnchorEnum::W;
	Password22->font = "SegoeB";
	Password22->TextColor = DEFAULT_TEXT;
	Password22->CursorColor = mulColor(DEFAULT_TEXT, 0.7);
	Password22->Spacing = 0;
	Password22->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	Password22->BorderThickness = 4;
	Password22->HideText = '*';
	Password22->ClearOnClick = false;
	Password22->CursorSize = 2;
	Password22->AllowedSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_!+-=#$%^&*().,/\\`~[]{}";

	ImageLabel* TextBoxEye2 = new ImageLabel(SignUpFrame);
	TextBoxEye2->setImage(getImage("textbox_eye"));
	TextBoxEye2->Size = {0.085, 0.085};
	TextBoxEye2->Position = {0.915, 0.505};
	TextBoxEye2->Active = true;
	TextBoxEye2->AnchorPosition = {0.5,0.5};
	TextBoxEye2->BackgroundTransparency = 1;
	TextBoxEye2->ImageColor = DEFAULT_TEXT;
	TextBoxEye2->SetMouse1HoldEnd([Password2, Password22, TextBoxEye2](Object2D* t){
		Password2->HideText = (Password2->HideText == '\0' ? '*' : '\0');
		Password22->HideText = (Password22->HideText == '\0' ? '*' : '\0');
		TextBoxEye2->ImageColor = mulColor(DEFAULT_TEXT, (Password2->HideText != '\0') ? 1 : 0.8);
	});
	TextBoxEye2->SetMouseEnter([TextBoxEye2](Object2D* t){
		Animate::Create(&TextBoxEye2->Size, 0.125, {0.105, 0.105});
	});
	TextBoxEye2->SetMouseLeave([TextBoxEye2](Object2D* t){
		Animate::Create(&TextBoxEye2->Size, 0.125, {0.085, 0.085});
	});

	TextLabel* DataIncorrect2 = new TextLabel(SignUpFrame);
	DataIncorrect2->Size = { 0.9, 0.06 };
	DataIncorrect2->Position = { 0.5, 0.67 };
	DataIncorrect2->AnchorPosition = { 0.5,0.5 };
	DataIncorrect2->Text = "Login already exists";
	DataIncorrect2->TextColor = { 255, 40, 40, 255 };
	DataIncorrect2->font = "SegoeB";
	DataIncorrect2->BackgroundTransparency = 1;
	DataIncorrect2->TextTransparency = 1;

	TextLabel* CheckLogin2 = new TextLabel(SignUpFrame);
	CheckLogin2->Position = { 0.5, 0.8 };
	CheckLogin2->Size = { 0.4, 0.125 };
	CheckLogin2->AnchorPosition = { 0.5, 0.5 };
	CheckLogin2->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.5);
	CheckLogin2->BorderColor = mulColor(DEFAULT_BACKGROUND, 0.5);
	CheckLogin2->BorderThickness = 4;
	CheckLogin2->font = "SegoeB";
	CheckLogin2->Active = true;
	CheckLogin2->Text = "Create";
	CheckLogin2->Roundness = 0.15;
	CheckLogin2->Segments = 7;
	CheckLogin2->TextColor = mulColor(DEFAULT_TEXT, 0.8);
	CheckLogin2->SetMouseEnter([CheckLogin2](Object2D* t) {
		Animate::Create(&CheckLogin2->Size, 0.1f, { 0.5, 0.135 });
		Animate::Create(&CheckLogin2->BorderColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 0.8));
		Animate::Create(&CheckLogin2->TextColor, 0.1f, mulColor(DEFAULT_TEXT, 1.1));
	});
	CheckLogin2->SetMouseLeave([CheckLogin2](Object2D* t) {
		Animate::Create(&CheckLogin2->Size, 0.1f, { 0.4, 0.125 });
		Animate::Create(&CheckLogin2->BorderColor, 0.1f, mulColor(DEFAULT_BACKGROUND, 0.5));
		Animate::Create(&CheckLogin2->TextColor, 0.1f, mulColor(DEFAULT_TEXT, 0.8));
	});
	CheckLogin2->SetMouse1HoldEnd([DataIncorrect2, CheckLogin2, Login2, Password2, Password22](Object2D* t) {
		if (Authenticated) return;
		if (Login2->Text.empty() or Password2->Text.empty() or Password22->Text.empty()) return;
		if (strcmp(Password2->Text.c_str(), Password22->Text.c_str())) {
			DataIncorrect2->Text = "Passwords must match";
			DataIncorrect2->TextTransparency = 0;
			return;
		}

		DataIncorrect2->TextTransparency = 1;

		size_t size = Login2->Text.size() + Password2->Text.size() + 1;

		char* input = new char[size];
		memcpy(input, Login2->Text.c_str(), Login2->Text.size());
		input[Login2->Text.size()] = '|';
		memcpy(input + Login2->Text.size() + 1, Password2->Text.c_str(), Password2->Text.size());

		AsyncData* data = new AsyncData(input, QueryType::SignUp, size);
		data->deleteOnCompleted	= false;
		data->Completed([DataIncorrect2, Login2, Password2, Password22, CheckLogin2, data, input](std::pair<const char*, size_t> output) {
			CheckLogin2->Active = true;
			CheckLogin2->TextColor = mulColor(DEFAULT_TEXT, 0.8);
			Login2->Active = true;
			Login2->TextColor = mulColor(DEFAULT_TEXT, 1);
			Password2->Active = true;
			Password2->TextColor = mulColor(DEFAULT_TEXT, 1);
			Password22->Active = true;
			Password22->TextColor = mulColor(DEFAULT_TEXT, 1);
			delete[]input;
			
			if (output.first) {
				if (!strcmp(output.first, "-1")) {
					std::cout << "Header is incorrect" << std::endl;
					delete data;
					return;
				}

				if (!strcmp(output.first, "e1")) {
					DataIncorrect2->Text = "Data is incorrect";
					DataIncorrect2->TextTransparency = 0;
				} else if (!strcmp(output.first, "e2")) {
					DataIncorrect2->Text = "Login is incorrect";
					DataIncorrect2->TextTransparency = 0;
				} else if (!strcmp(output.first, "e3")) {
					DataIncorrect2->Text = "Password is incorrect";
					DataIncorrect2->TextTransparency = 0;
				} else {
					size_t id = string_to_size_t(output.first);
					clientId = id;
					Authenticated = true;
					loadChats(0, []() {
						ChatsUpdated = true;
					});
					DataIncorrect2->TextTransparency = 1;
					
					std::string string_id = long_to_string(id);
					char* input2 = new char[string_id.size() + 1];
					memcpy(input2, string_id.data(), string_id.size());
					input2[string_id.size()] = '\0';
					
					AsyncData* data2 = new AsyncData(input2, QueryType::GET_USER_INFO, string_id.size());
					data2->Completed([input2, data](std::pair<const char*, size_t> output1){
						if (!output1.first) {
							delete data;
							delete[] input2;
							return;
						}

						if (!strcmp(output1.first, "e1")) {
							std::cout << "No client with id " << input2 << std::endl;
							delete[] output1.first;
							delete data;
							delete[] input2;
							return;
						}

						if (*getClientPtr()) delete *getClientPtr();
						*getClientPtr() = deserializeClient(std::string(output1.first, output1.second));
						delete[] output1.first;
						delete data;
						delete[] input2;
					});

					data2->send();
				}
				delete[]output.first;
			} else {
				DataIncorrect2->Text = "Server doesn't answer";
				DataIncorrect2->TextTransparency = 0;
				delete data;
			}
		});
		data->Sended([Login2, Password2, Password22, CheckLogin2, DataIncorrect2]() {
			DataIncorrect2->TextTransparency = 1;
			CheckLogin2->Active = false;
			Login2->Active = false;
			Password2->Active = false;
			Password22->Active = false;
		});
		data->send();
	});

	return 0;
}

ScrollFrame* settingsAnimationScroll = nullptr;

int settingsUI() {
	settingsAnimationScroll = new ScrollFrame(StartInstance->findChild("GeneralUI background"));
	settingsAnimationScroll->Size = { 0, 0.85 };
	settingsAnimationScroll->SizeOFFSET.x = 350;
	settingsAnimationScroll->BackgroundTransparency = 1;
	settingsAnimationScroll->ZIndex = 15;
	settingsAnimationScroll->ScrollEnabled = false;
	settingsAnimationScroll->ScrollEnabled = false;
	settingsAnimationScroll->SliderTransparency = 1;
	settingsAnimationScroll->Direction = 'X';
	settingsAnimationScroll->AnchorPosition = { 0.5,0.5 };
	settingsAnimationScroll->Position = { 0.5,0.5 };
	settingsAnimationScroll->Active = true;
	settingsAnimationScroll->Name = "settingsAnimationScroll";

	settingsFrame = new Object2D(settingsAnimationScroll);
	settingsFrame->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 1.2);
	settingsFrame->Roundness = 0.1;
	settingsFrame->BorderColor = DEFAULT_BACKGROUND;
	settingsFrame->BorderThickness = 4;
	settingsFrame->Name = "SettingsFrame";
	settingsFrame->Position = { -0.5, 0.5 };
	settingsFrame->AnchorPosition = { 0.5,0.5 };
	settingsFrame->Size = { 0.97, 0.97 };

	ScrollFrame* SettingsScroll = new ScrollFrame(settingsFrame);
	SettingsScroll->Size = { 1,1 };
	SettingsScroll->BackgroundTransparency = 1;
	SettingsScroll->CanvasSize.y = 2;

	return 0;
}

int generalUI() {
	Object2D* Background = new Object2D(StartInstance);
	Background->Name = "GeneralUI background";
	Background->Visible = false;
	Background->Size = { 1,1 };
	Background->SizeOFFSET = { 0,-BAR_SIZE };
	Background->PositionOFFSET = { 0,BAR_SIZE };
	Background->BackgroundColor = DEFAULT_BACKGROUND;
	Background->ZIndex = 10;

	ImageLabel* ChatBackground = new ImageLabel(Background);
	ChatBackground->BackgroundColor = DEFAULT_BACKGROUND;
	ChatBackground->Size.y = 1;

	CurrentChatScroll = new ScrollFrame(ChatBackground);
	CurrentChatScroll->Active = true;
	CurrentChatScroll->Animated = true;
	CurrentChatScroll->Size.y = 1;
	CurrentChatScroll->Size.x = 1;
	CurrentChatScroll->SizeOFFSET.y = -90;
	CurrentChatScroll->BackgroundTransparency = 1;

	TextLabel* UpperChatName = new TextLabel(ChatBackground);
	UpperChatName->TextColor = DEFAULT_TEXT;
	UpperChatName->font = "SegoeB";
	UpperChatName->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 0.9);
	UpperChatName->TextAnchor = TextAnchorEnum::W;
	UpperChatName->Text = "  Chat idk";
	UpperChatName->SizeOFFSET = { 0, 30 };
	UpperChatName->Size.x = 1;

	Object2D* LeftMenu = new Object2D(Background);
	LeftMenu->Active = true;
	LeftMenu->ZIndex = 15;
	LeftMenu->Size = { 0, 1 };
	LeftMenu->SizeOFFSET = { 350, 0 };
	LeftMenu->PositionOFFSET = { -350, 0 };
	LeftMenu->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 0.9);
	LeftMenu->Name = "LeftMenu";

	ImageLabel* ProfileImage = new ImageLabel(LeftMenu);
	ProfileImage->SizeOFFSET = { 80, 80 };
	ProfileImage->AnchorPosition = { 0.5,0 };
	ProfileImage->Position = { 0.5,0 };
	ProfileImage->BackgroundTransparency = 1;
	ProfileImage->setImage(getImage("defaultProfileImage"));
	ProfileImage->Roundness = 1;
	ProfileImage->RoundImage = true;
	ProfileImage->PositionOFFSET.y = 20;
	ProfileImage->BorderColor = mulColor(DEFAULT_BACKGROUND, 1.2);
	ProfileImage->BorderThickness = 5;

	TextLabel* ProfileName = new TextLabel(LeftMenu);
	ProfileName->AnchorPosition = { 0.5,0 };
	ProfileName->Position = { 0.5,0 };
	ProfileName->SizeOFFSET = {350, 50};
	ProfileName->BackgroundTransparency = 1;
	ProfileName->TextColor = mulColor(DEFAULT_TEXT, 0.7);
	ProfileName->PositionOFFSET.y = 110;
	ProfileName->Text = "Profile name";
	ProfileName->font = "SegoeB";

	TextLabel* UserID = new TextLabel(LeftMenu);
	UserID->BackgroundTransparency = 1;
	UserID->TextColor = mulColor(DEFAULT_TEXT, 0.5);
	UserID->Size = { 1, 0 };
	UserID->SizeOFFSET.y = 30;
	UserID->Position.y = 1;
	UserID->PositionOFFSET.y = -30;
	UserID->font = "SegoeB";
	UserID->Text = "UserID";

	{ // PROFILE BUTTON
		Object2D* ProfileButtonFull = new Object2D(LeftMenu);
		ProfileButtonFull->BackgroundTransparency = 1;
		ProfileButtonFull->BackgroundColor = { 255,255,255,255 };
		ProfileButtonFull->Size.x = 1;
		ProfileButtonFull->SizeOFFSET.y = 30;
		ProfileButtonFull->Active = true;
		ProfileButtonFull->PositionOFFSET.y = 180;
		ProfileButtonFull->AnchorPosition.x = 0.5;
		ProfileButtonFull->Position.x = 0.5;
		ProfileButtonFull->SetMouseEnter([ProfileButtonFull](Object2D* t) {
			Animate::Create(&ProfileButtonFull->BackgroundTransparency, 0.125, 0.85);
		});
		ProfileButtonFull->SetMouseLeave([ProfileButtonFull](Object2D* t) {
			Animate::Create(&ProfileButtonFull->BackgroundTransparency, 0.125, 1);
		});
		ProfileButtonFull->SetMouse1HoldEnd([](Object2D* t) {
			GlobalStates::UIstate = PROFILE;
		});

		ImageLabel* ProfileButtonImage = new ImageLabel(ProfileButtonFull);
		ProfileButtonImage->BackgroundTransparency = 1;
		ProfileButtonImage->setImage(getImage("profile"));
		ProfileButtonImage->ImageColor = mulColor(DEFAULT_TEXT, 0.7);
		ProfileButtonImage->Size = { 0.2, 0.8 };
		ProfileButtonImage->Position.y = 0.1;

		TextLabel* ProfileButton = new TextLabel(ProfileButtonFull);
		ProfileButton->TextColor = mulColor(DEFAULT_TEXT, 0.7);
		ProfileButton->Text = "My profile";
		ProfileButton->BackgroundTransparency = 1;
		ProfileButton->Size = { 0.8, 1 };
		ProfileButton->Position.x = 0.2;
		ProfileButton->font = "SegoeB";
		ProfileButton->TextAnchor = TextAnchorEnum::W;
	}

	{ // SETTINGS BUTTON
		Object2D* SettingsButtonFull = new Object2D(LeftMenu);
		SettingsButtonFull->BackgroundTransparency = 1;
		SettingsButtonFull->BackgroundColor = { 255,255,255,255 };
		SettingsButtonFull->Size.x = 1;
		SettingsButtonFull->SizeOFFSET.y = 30;
		SettingsButtonFull->Active = true;
		SettingsButtonFull->PositionOFFSET.y = 220;
		SettingsButtonFull->AnchorPosition.x = 0.5;
		SettingsButtonFull->Position.x = 0.5;
		SettingsButtonFull->SetMouseEnter([SettingsButtonFull](Object2D* t) {
			Animate::Create(&SettingsButtonFull->BackgroundTransparency, 0.125, 0.85);
		});
		SettingsButtonFull->SetMouseLeave([SettingsButtonFull](Object2D* t) {
			Animate::Create(&SettingsButtonFull->BackgroundTransparency, 0.125, 1);
		});
		SettingsButtonFull->SetMouse1HoldEnd([](Object2D* t) {
			GlobalStates::UIstate = SETTINGS;
		});

		ImageLabel* SettingsButtonImage = new ImageLabel(SettingsButtonFull);
		SettingsButtonImage->BackgroundTransparency = 1;
		SettingsButtonImage->setImage(getImage("settings"));
		SettingsButtonImage->ImageColor = mulColor(DEFAULT_TEXT, 0.7);
		SettingsButtonImage->Size = { 0.2, 0.8 };
		SettingsButtonImage->Position.y = 0.1;

		TextLabel* SettingsButton = new TextLabel(SettingsButtonFull);
		SettingsButton->TextColor = mulColor(DEFAULT_TEXT, 0.7);
		SettingsButton->Text = "Settings";
		SettingsButton->BackgroundTransparency = 1;
		SettingsButton->Size = { 0.8, 1 };
		SettingsButton->Position.x = 0.2;
		SettingsButton->font = "SegoeB";
		SettingsButton->TextAnchor = TextAnchorEnum::W;
	}

	Object2D* BackToGeneral = new Object2D(Background);
	BackToGeneral->Size = { 1,1 };
	BackToGeneral->BackgroundTransparency = 1;
	BackToGeneral->ZIndex = 14;
	BackToGeneral->Active = true;
	BackToGeneral->SetMouse1HoldStart([](Object2D* t) {
		GlobalStates::UIstate = GENERAL;
	});

	ImageLabel* ClientProfileButton = new ImageLabel(Background);
	ClientProfileButton->BackgroundTransparency = 1;
	ClientProfileButton->BackgroundColor = DEFAULT_TEXT;
	ClientProfileButton->SizeOFFSET = { 30,30 };
	ClientProfileButton->PositionOFFSET = {30,20};
	ClientProfileButton->Active = true;
	ClientProfileButton->AnchorPosition = { 0.5, 0.5 };
	ClientProfileButton->setImage(getImage("list"));
	ClientProfileButton->ImageColor = mulColor(DEFAULT_TEXT, 0.8);
	ClientProfileButton->Roundness = 0.2;
	ClientProfileButton->SetMouseEnter([ClientProfileButton](Object2D* t) {
		Animate::Create(&ClientProfileButton->SizeOFFSET, 0.125, { 35, 35 });
		Animate::Create(&ClientProfileButton->ImageColor, 0.125, DEFAULT_TEXT);
		Animate::Create(&ClientProfileButton->BackgroundTransparency, 0.125, 0.95);
	});
	ClientProfileButton->SetMouseLeave([ClientProfileButton](Object2D* t) {
		Animate::Create(&ClientProfileButton->SizeOFFSET, 0.125, { 30, 30 });
		Animate::Create(&ClientProfileButton->ImageColor, 0.125, mulColor(DEFAULT_TEXT, 0.8));
		Animate::Create(&ClientProfileButton->BackgroundTransparency, 0.125, 1);
	});
	ClientProfileButton->SetMouse1HoldEnd([](Object2D* t) {
		GlobalStates::UIstate = LEFT_MENU;
	});

	ScrollFrame* ChatScroll = new ScrollFrame(Background);
	ChatScroll->Size.y = 1;
	ChatScroll->PositionOFFSET.y = 40;
	ChatScroll->SizeOFFSET.y = -40;
	ChatScroll->SizeOFFSET.x = 350;
	ChatScroll->Active = true;
	ChatScroll->Animated = true;
	ChatScroll->ZIndex = 5;
	ChatScroll->BackgroundColor = mulColor(DEFAULT_BACKGROUND, 0.8);
	ChatScroll->SliderColor = { 255,255,255,255 };
	ChatScroll->SliderSize = 3;
	ChatScroll->SliderTransparency = 1;
	ChatScroll->ScrollSpeed = 0.25;
	ChatScroll->CanBeEnteredIfNotHigher = true;
	ChatScroll->SetMouseEnter([ChatScroll](Object2D* t) {
		Animate::Create(&ChatScroll->SliderTransparency, 0.125, 0.5);
	});
	ChatScroll->SetMouseLeave([ChatScroll](Object2D* t) {
		Animate::Create(&ChatScroll->SliderTransparency, 0.125, 1);
	});

	static bool resizing = false;
	static int max = 1000;
	static int min = 200;
	static bool isMinimal = false;

	static auto setFullScrollMode = [ChatScroll]() {
		isMinimal = false;
	};

	static auto setMinimalScrollMode = [ChatScroll]() {
		isMinimal = true;
	};

	static int backOffsetX = 0;

	ChatScroll->SetForTick([ChatScroll]() {
		if (FocusedTextBox == nullptr and GlobalStates::UIstate == GENERAL and ChatScroll->pointInObject(GetMousePosition())) {
			static bool key1Down = false;
			static bool key2Down = false;

			if (IsKeyPressed(KEY_DOWN)) {
				key1Down = true;
			}
			
			if (IsKeyPressed(KEY_UP)) {
				key2Down = true;
			}

			if (IsKeyUp(KEY_DOWN)) {
				key1Down = false;
			}

			if (IsKeyUp(KEY_UP)) {
				key2Down = false;
			}

			if (key1Down) {
				ChatScroll->CanvasPosition.y += CHAT_SIZE / ChatScroll->RealSize.y * dt * 8;
			} else if (key2Down) {
				ChatScroll->CanvasPosition.y -= CHAT_SIZE / ChatScroll->RealSize.y * dt * 8;
			}
		}

		Vector2 mousePos = ChatScroll->getMousePosition();
		if (ChatScroll->RealSize.x - mousePos.x * ChatScroll->RealSize.x < 7 and mousePos.x <= 1 and (ChatScroll->RealSize.y - mousePos.y * ChatScroll->RealSize.y) > 10 and (mousePos.y * ChatScroll->RealSize.y) > BAR_SIZE and GlobalStates::UIstate == GENERAL) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				backOffsetX = ChatScroll->RealSize.x - mousePos.x * ChatScroll->RealSize.x;
				resizing = true;
			}

			Resizing_EW = true;
		} else if (!resizing) {
			Resizing_EW = false;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			Resizing_EW = false;
			resizing = false;
		}
		
		if (ChatScroll->SizeOFFSET.x + 200 > winWidth) {
			ChatScroll->SizeOFFSET.x = winWidth - 200;
		}

		if (resizing) {
			int newPos = ChatScroll->RealSize.x * mousePos.x + backOffsetX;
			
			if (newPos > 1000) newPos = 1000;
			if (newPos < 200 and newPos > 100) newPos = 200;
			if (newPos + 200 > winWidth) {
				newPos = winWidth - 200;
			}
			
			if (newPos <= 100) {
				setMinimalScrollMode();
				ChatScroll->SizeOFFSET.x = 60;
			} else {
				if (isMinimal) {
					setFullScrollMode();
				}
				ChatScroll->SizeOFFSET.x = newPos;
			}
		}
	});

	auto updateChatScroll = [ChatBackground, ChatScroll]() {
		ChatBackground->SizeOFFSET.x = winWidth - ChatScroll->SizeOFFSET.x;
		ChatBackground->PositionOFFSET.x = ChatScroll->SizeOFFSET.x;
	};

	new ChangedSignal(ChatScroll->SizeOFFSET.x, updateChatScroll);
	new ChangedSignal(winWidth, updateChatScroll);

	Folder* ChatsFolder = new Folder(ChatScroll);
	ChatsFolder->Name = "ChatsFolder";

	ChatTemplate = new Object2D(ChatScroll);
	ChatTemplate->BackgroundTransparency = 1;
	ChatTemplate->Name = "ChatTemplate";
	ChatTemplate->BackgroundColor = { 255,255,255,255 };
	ChatTemplate->SizeOFFSET = { -5, CHAT_SIZE };
	ChatTemplate->Size = { 1, 0 };
	ChatTemplate->PositionOFFSET = { 0, 0 * CHAT_SIZE*3 };
	ChatTemplate->Active = true;
	ChatTemplate->Visible = false;

	ImageLabel* ChatIcon = new ImageLabel(ChatTemplate);
	ChatIcon->Name = "Icon";
	ChatIcon->SizeOFFSET = { CHAT_SIZE * 0.8, CHAT_SIZE * 0.8 };
	ChatIcon->BackgroundTransparency = 1;
	ChatIcon->Roundness = 1;
	ChatIcon->RoundImage = true;
	ChatIcon->PositionOFFSET = { CHAT_SIZE * 0.5, CHAT_SIZE * 0.5 };
	ChatIcon->AnchorPosition = { 0.5, 0.5 };
	ChatIcon->setImage(getImage("app_icon"));
	ChatIcon->Name = "ChatIcon";

	TextLabel* ChatName = new TextLabel(ChatTemplate);
	ChatName->Name = "Name";
	ChatName->PositionOFFSET = { CHAT_SIZE + 20, 2 };
	ChatName->SizeOFFSET = { 1000 - CHAT_SIZE - 20, 0 };
	ChatName->Size = { 0, 0.5 };
	ChatName->BackgroundTransparency = 1;
	ChatName->Name = "ChatName";
	ChatName->font = "SegoeB";
	ChatName->MaxVisibleSymbols = 70;
	ChatName->TextAnchor = TextAnchorEnum::W;
	ChatName->TextColor = mulColor(DEFAULT_TEXT, 0.8);
	ChatName->Text = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";

	TextLabel* ChatLastMsg = new TextLabel(ChatTemplate);
	ChatLastMsg->Name = "LastMsg";
	ChatLastMsg->PositionOFFSET = { CHAT_SIZE + 20, 2 + CHAT_SIZE * 0.55 };
	ChatLastMsg->SizeOFFSET = { 1000 - CHAT_SIZE - 20, 0 };
	ChatLastMsg->Size = { 0, 0.3 };
	ChatLastMsg->BackgroundTransparency = 1;
	ChatLastMsg->Name = "ChatLastMsg";
	ChatLastMsg->font = "SegoeB";
	ChatLastMsg->MaxVisibleSymbols = 110;
	ChatLastMsg->TextAnchor = TextAnchorEnum::W;
	ChatLastMsg->TextColor = mulColor(DEFAULT_TEXT, 0.8);
	ChatLastMsg->Text = "абвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжиабвгдеёзжи";
	
	ChatTemplate->SetMouseEnter([](Object2D* t) {
		ImageLabel* ChatIcon = static_cast<ImageLabel*>(t->findChild("ChatIcon"));
		TextLabel* ChatName = static_cast<TextLabel*>(t->findChild("ChatName"));

		Animate::Create(&t->BackgroundTransparency, 0.125, 0.9);
		Animate::Create(&ChatIcon->SizeOFFSET, 0.055, { CHAT_SIZE * 0.95, CHAT_SIZE * 0.95 });
		Animate::Create(&ChatName->TextColor, 0.055, { mulColor(DEFAULT_TEXT, 1) });
	});
	ChatTemplate->SetMouseLeave([](Object2D* t) {
		ImageLabel* ChatIcon = static_cast<ImageLabel*>(t->findChild("ChatIcon"));
		TextLabel* ChatName = static_cast<TextLabel*>(t->findChild("ChatName"));

		Animate::Create(&t->BackgroundTransparency, 0.125, 1);
		Animate::Create(&ChatIcon->SizeOFFSET, 0.055, { CHAT_SIZE * 0.8, CHAT_SIZE * 0.8 });
		Animate::Create(&ChatName->TextColor, 0.055, { mulColor(DEFAULT_TEXT, 0.8) });
	});
	
	new ChangedSignal<bool>(Authenticated, [&]() {
		if (Authenticated) {
			dynamic_cast<Object2D*>(StartInstance->findChild("GeneralUI background"))->Visible = true;
			Object2D* authFrame = dynamic_cast<Object2D*>(StartInstance->findChild("Auth Frame"));
			authFrame->Visible = true;
			authFrame->Position = { 0, 0 };
			GlobalStates::UIstate = GENERAL;

			Animate::Animation* anim = Animate::Create(&authFrame->Position.x, 0.125, 1);
			anim->Completed = [authFrame]() {
				authFrame->Visible = false;
			};
		} else {
			Object2D* generalFrame = dynamic_cast<Object2D*>(StartInstance->findChild("GeneralUI background"));
			generalFrame->Visible = true;
			Object2D* authFrame = dynamic_cast<Object2D*>(StartInstance->findChild("Auth Frame"));
			authFrame->Visible = true;
			authFrame->Position = { -1, 0 };
			GlobalStates::UIstate = AUTH;

			Animate::Animation* anim = Animate::Create(&authFrame->Position.x, 0.125, 0);
			anim->Completed = [generalFrame]() {
				generalFrame->Visible = false;
			};
		}
	});

	new ChangedSignal<bool>(ChatsUpdated, [ChatsFolder, ChatScroll]() {
		if (ChatsUpdated) {
			loadChatsMutex.lock();

			ChatsUpdated = false;
			ChatsFolder->deleteAllChildren();

			int i = 0;
			for (auto& [id, chat] : LoadedChats) {
				Object2D* newChat = ChatTemplate->Clone();
				newChat->setParent(ChatScroll);
				newChat->Name = long_to_string(chat->ChatID);
				ImageLabel* icon = static_cast<ImageLabel*>(newChat->findChild("ChatIcon"));
				TextLabel* name = static_cast<TextLabel*>(newChat->findChild("ChatName"));
				TextLabel* lastMsg = static_cast<TextLabel*>(newChat->findChild("ChatLastMsg"));

				if (name) {
					name->Text = chat->Name;
				}

				if (lastMsg) {
					lastMsg->Text = "Last Message";
				}

				newChat->Visible = true;
				newChat->PositionOFFSET.y = CHAT_SIZE * i++;
			}
			loadChatsMutex.unlock();
		}
	});

	new ChangedSignal<Client*>(*getClientPtr(), [ProfileName, UserID, ProfileImage](){
		if (*getClientPtr()) {
			(*getClientPtr())->asyncDataMutex.lock();
			ProfileName->Text = (*getClientPtr())->userName;
			UserID->Text = "UID: " + std::to_string((*getClientPtr())->userID);
		
			if ((*getClientPtr())->avatar.size() <= 1) {
				ProfileImage->setImage(getImage("defaultProfileImage"));
			} else {
				std::vector<unsigned char> icon((*getClientPtr())->avatar.getIcon().second);
				memcpy(icon.data(), (*getClientPtr())->avatar.getIcon().first, (*getClientPtr())->avatar.getIcon().second);
				ProfileImage->UpdateWithType(".jpg", icon);
			}

			/*
			std::cout << (*getClientPtr())->userName << std::endl;
			std::cout << (*getClientPtr())->login << std::endl;
			std::cout << (*getClientPtr())->lastActivity << std::endl;
			std::cout << (*getClientPtr())->userID << std::endl;
			*/
			(*getClientPtr())->asyncDataMutex.unlock();
		}
	});

	new ChangedSignal<WhereOnUI>(GlobalStates::UIstate, [BackToGeneral, LeftMenu]() {
		constexpr static float AnimationTime = 0.1;
		constexpr static Animate::Function SettingsAnimationType = Animate::Function::Exponential;

		switch (GlobalStates::UIstate) {
			case GENERAL: {
				settingsAnimationScroll->Active = false;
				BackToGeneral->Active = false;
				LeftMenu->Active = false;
				Animate::Create(&BackToGeneral->BackgroundTransparency, AnimationTime, 1);
				Animate::Create(&LeftMenu->PositionOFFSET.x, AnimationTime, -LeftMenu->SizeOFFSET.x);
				Animate::Create(&settingsFrame->Position.x, 0.1, -0.5, SettingsAnimationType);
				break;
			}
			case LEFT_MENU: {
				settingsAnimationScroll->Active = false;
				BackToGeneral->Active = true;
				LeftMenu->Active = true;
				Animate::Create(&BackToGeneral->BackgroundTransparency, AnimationTime, 0.7);
				Animate::Create(&LeftMenu->PositionOFFSET.x, AnimationTime, 0);
				Animate::Create(&settingsFrame->Position.x, 0.1, -0.5, SettingsAnimationType);
				break;
			}
			case AUTH: {
				settingsAnimationScroll->Active = false;
				BackToGeneral->Active = false;
				LeftMenu->Active = false;
				Animate::Create(&BackToGeneral->BackgroundTransparency, AnimationTime, 1);
				Animate::Create(&LeftMenu->PositionOFFSET.x, AnimationTime, -LeftMenu->SizeOFFSET.x);
				Animate::Create(&settingsFrame->Position.x, 0.1, -0.5, SettingsAnimationType);
				break;
			}
			case PROFILE: {
				settingsAnimationScroll->Active = false;
				BackToGeneral->Active = true;
				LeftMenu->Active = false;
				Animate::Create(&BackToGeneral->BackgroundTransparency, AnimationTime, 0.7);
				Animate::Create(&LeftMenu->PositionOFFSET.x, AnimationTime, -LeftMenu->SizeOFFSET.x);
				Animate::Create(&settingsFrame->Position.x, 0.1, -0.5, SettingsAnimationType);
				break;
			}
			case SETTINGS: {
				settingsAnimationScroll->Active = true;
				BackToGeneral->Active = true;
				LeftMenu->Active = false;
				Animate::Create(&BackToGeneral->BackgroundTransparency, AnimationTime, 0.7);
				Animate::Create(&LeftMenu->PositionOFFSET.x, AnimationTime, -LeftMenu->SizeOFFSET.x);
				settingsFrame->Position.x = 1.5;
				Animate::Create(&settingsFrame->Position.x, 0.1, 0.5, SettingsAnimationType);
				break;
			}
		}
	});

	return 0;
}

void initialInterface() {
	serverDataFunction([](std::pair<networkHeader, std::pair<char*, size_t>> data) {
		std::cout << "Server data: " << data.second.first << " " << data.second.second << std::endl;
	});

	StartInstance = new Instance(true);
	StartInstance->Name = "Root";
	addFontToQueqe("Segoe", "Fonts/segoeui.ttf", 100);
	addFontToQueqe("SegoeB", "Fonts/segoeuib.ttf", 100);

	loadImage("textbox_eye", "textures/textbox_eye.png");
	loadImage("app_icon", "textures/icon.png");
	loadImage("auth_background", "textures/auth_background.png");
	loadImage("exit", "textures/exit.png");
	loadImage("window", "textures/window.png");
	loadImage("hide", "textures/hide.png");
	loadImage("list", "textures/list.png");
	loadImage("defaultProfileImage", "textures/cat.jpg");
	loadImage("profile", "textures/profile.png");
	loadImage("settings", "textures/settings.png");

	int barReturn = initUpperBar();
	if (barReturn) {
		std::cerr << "Error in initUpperBar: " << barReturn << std::endl;
		exit(barReturn);
	}

	int authReturn = initAuth();
	if (authReturn) {
		std::cerr << "Error in initAuth: " << authReturn << std::endl;
		exit(authReturn);
	}

	int generalReturn = generalUI();
	if (generalReturn) {
		std::cerr << "Error in generalUI: " << generalReturn << std::endl;
		exit(generalReturn);
	}

	int settingsReturn = settingsUI();
	if (settingsReturn) {
		std::cerr << "Error in settingsUI: " << settingsReturn << std::endl;
		exit(settingsReturn);
	}

	start(*StartInstance, { 1200, 800, 0 }, "Nexora", "textures/icon.png", FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE);
}