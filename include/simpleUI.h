//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																												//
// simpleUI by Ishakao | https://github.com/Ishakao/simpleUI													//
// Current Version 1.0.1																						//
//																												//
// Description:																									//
// simpleUI is a library for simple creating a beautiful and fast user interfaces.								//
// Based on raylib v5.5 by raysan5 (www.raylib.com | https://github.com/raysan5)								//
// simpleUI interfaces are based on a hierarchical structure of objects for convinient objects management		//	
//																												//
//      ! IF YOU NEED SOME RAYLIB FUNCTIONS/STRUCTURES USE RAYLIB_FUNCTIONAL:: NAMESPACE TO USE THEM !          //
//																												//
// Changed Logs:																								//
// Better child event system																					//
// Text class for management TextLabel and TextBox (TEXT_CHANGED events optimization)							//
// Additional events for objects (like TEXT_CHANGED on text-objects)											//
// Textures RAM & VRAM optimization																				//
// A few CPU optimizations																						//
// Spacial Grid optimization for ScrollFrame (millions of objects with thousands of FPS)					    //
//																												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifdef _WIN32
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#define _CRT_SECURE_NO_WARNINGS
#include <stb_image_write.h>

namespace RAYLIB_FUNCTIONAL {
#include <raylib.h>
}

using RAYLIB_FUNCTIONAL::Vector2;
using RAYLIB_FUNCTIONAL::Shader;
using RAYLIB_FUNCTIONAL::Image;
using RAYLIB_FUNCTIONAL::Texture;
using RAYLIB_FUNCTIONAL::Color;
using RAYLIB_FUNCTIONAL::Font;
using RAYLIB_FUNCTIONAL::Rectangle;
using RAYLIB_FUNCTIONAL::Vector3;
using RAYLIB_FUNCTIONAL::RenderTexture2D;
using RAYLIB_FUNCTIONAL::Texture2D;

using RAYLIB_FUNCTIONAL::SetMouseCursor;
using RAYLIB_FUNCTIONAL::SetTargetFPS;
using RAYLIB_FUNCTIONAL::SetWindowPosition;
using RAYLIB_FUNCTIONAL::SetExitKey;
using RAYLIB_FUNCTIONAL::SetWindowMinSize;
using RAYLIB_FUNCTIONAL::SetConfigFlags;
using RAYLIB_FUNCTIONAL::SetTraceLogLevel;
using RAYLIB_FUNCTIONAL::SetWindowSize;
using RAYLIB_FUNCTIONAL::InitWindow;
using RAYLIB_FUNCTIONAL::CloseWindow;
using RAYLIB_FUNCTIONAL::GetWindowPosition;
using RAYLIB_FUNCTIONAL::GetScreenWidth;
using RAYLIB_FUNCTIONAL::GetScreenHeight;
using RAYLIB_FUNCTIONAL::GetFrameTime;
using RAYLIB_FUNCTIONAL::GetMousePosition;
using RAYLIB_FUNCTIONAL::GetMonitorRefreshRate;
using RAYLIB_FUNCTIONAL::GetCurrentMonitor;
using RAYLIB_FUNCTIONAL::GetMouseWheelMove;
using RAYLIB_FUNCTIONAL::IsKeyDown;
using RAYLIB_FUNCTIONAL::IsKeyPressed;
using RAYLIB_FUNCTIONAL::IsMouseButtonPressed;
using RAYLIB_FUNCTIONAL::IsMouseButtonReleased;
using RAYLIB_FUNCTIONAL::IsWindowMaximized;
using RAYLIB_FUNCTIONAL::IsWindowReady;
using RAYLIB_FUNCTIONAL::IsWindowFullscreen;
using RAYLIB_FUNCTIONAL::WindowShouldClose;
using RAYLIB_FUNCTIONAL::ToggleFullscreen;
using RAYLIB_FUNCTIONAL::MaximizeWindow;
using RAYLIB_FUNCTIONAL::MinimizeWindow;
using RAYLIB_FUNCTIONAL::RestoreWindow;

using RAYLIB_FUNCTIONAL::GenTextureMipmaps;
using RAYLIB_FUNCTIONAL::SetTextureFilter;
using RAYLIB_FUNCTIONAL::SetTextureWrap;

using RAYLIB_FUNCTIONAL::LoadImage;
using RAYLIB_FUNCTIONAL::LoadImageFromMemory;
using RAYLIB_FUNCTIONAL::LoadTextureFromImage;
using RAYLIB_FUNCTIONAL::LoadShader;
using RAYLIB_FUNCTIONAL::LoadCodepoints;
using RAYLIB_FUNCTIONAL::LoadFontEx;
using RAYLIB_FUNCTIONAL::LoadRenderTexture;

using RAYLIB_FUNCTIONAL::DrawRectangleRounded;
using RAYLIB_FUNCTIONAL::DrawRectangleRoundedLinesEx;
using RAYLIB_FUNCTIONAL::DrawTexturePro;
using RAYLIB_FUNCTIONAL::DrawLineEx;
using RAYLIB_FUNCTIONAL::ClearBackground;
using RAYLIB_FUNCTIONAL::BeginScissorMode;
using RAYLIB_FUNCTIONAL::EndScissorMode;
using RAYLIB_FUNCTIONAL::BeginTextureMode;
using RAYLIB_FUNCTIONAL::EndTextureMode;
using RAYLIB_FUNCTIONAL::BeginShaderMode;
using RAYLIB_FUNCTIONAL::EndShaderMode;
using RAYLIB_FUNCTIONAL::BeginDrawing;
using RAYLIB_FUNCTIONAL::EndDrawing;

using RAYLIB_FUNCTIONAL::TEXTURE_FILTER_TRILINEAR;
using RAYLIB_FUNCTIONAL::TEXTURE_WRAP_CLAMP;

using RAYLIB_FUNCTIONAL::FLAG_WINDOW_UNDECORATED;
using RAYLIB_FUNCTIONAL::FLAG_WINDOW_RESIZABLE;

using RAYLIB_FUNCTIONAL::PIXELFORMAT_UNCOMPRESSED_R8G8B8;
using RAYLIB_FUNCTIONAL::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

using RAYLIB_FUNCTIONAL::MOUSE_BUTTON_LEFT;
using RAYLIB_FUNCTIONAL::MOUSE_BUTTON_RIGHT;
using RAYLIB_FUNCTIONAL::MOUSE_BUTTON_MIDDLE;

using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_DEFAULT;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_ARROW;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_CROSSHAIR;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_POINTING_HAND;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_RESIZE_ALL;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_NOT_ALLOWED;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_RESIZE_EW;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_RESIZE_NWSE;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_RESIZE_NESW;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_RESIZE_NS;
using RAYLIB_FUNCTIONAL::MOUSE_CURSOR_IBEAM;

using RAYLIB_FUNCTIONAL::LOG_NONE;

using RAYLIB_FUNCTIONAL::KeyboardKey;
using RAYLIB_FUNCTIONAL::KEY_LEFT_SHIFT;
using RAYLIB_FUNCTIONAL::KEY_ONE;
using RAYLIB_FUNCTIONAL::KEY_TWO;
using RAYLIB_FUNCTIONAL::KEY_THREE;
using RAYLIB_FUNCTIONAL::KEY_FOUR;
using RAYLIB_FUNCTIONAL::KEY_FIVE;
using RAYLIB_FUNCTIONAL::KEY_SIX;
using RAYLIB_FUNCTIONAL::KEY_SEVEN;
using RAYLIB_FUNCTIONAL::KEY_EIGHT;
using RAYLIB_FUNCTIONAL::KEY_NINE;
using RAYLIB_FUNCTIONAL::KEY_ZERO;
using RAYLIB_FUNCTIONAL::KEY_Q;
using RAYLIB_FUNCTIONAL::KEY_W;
using RAYLIB_FUNCTIONAL::KEY_E;
using RAYLIB_FUNCTIONAL::KEY_R;
using RAYLIB_FUNCTIONAL::KEY_T;
using RAYLIB_FUNCTIONAL::KEY_Y;
using RAYLIB_FUNCTIONAL::KEY_U;
using RAYLIB_FUNCTIONAL::KEY_I;
using RAYLIB_FUNCTIONAL::KEY_O;
using RAYLIB_FUNCTIONAL::KEY_P;
using RAYLIB_FUNCTIONAL::KEY_A;
using RAYLIB_FUNCTIONAL::KEY_S;
using RAYLIB_FUNCTIONAL::KEY_D;
using RAYLIB_FUNCTIONAL::KEY_F;
using RAYLIB_FUNCTIONAL::KEY_G;
using RAYLIB_FUNCTIONAL::KEY_H;
using RAYLIB_FUNCTIONAL::KEY_J;
using RAYLIB_FUNCTIONAL::KEY_K;
using RAYLIB_FUNCTIONAL::KEY_L;
using RAYLIB_FUNCTIONAL::KEY_SEMICOLON;
using RAYLIB_FUNCTIONAL::KEY_APOSTROPHE;
using RAYLIB_FUNCTIONAL::KEY_Z;
using RAYLIB_FUNCTIONAL::KEY_X;
using RAYLIB_FUNCTIONAL::KEY_C;
using RAYLIB_FUNCTIONAL::KEY_V;
using RAYLIB_FUNCTIONAL::KEY_B;
using RAYLIB_FUNCTIONAL::KEY_N;
using RAYLIB_FUNCTIONAL::KEY_M;
using RAYLIB_FUNCTIONAL::KEY_COMMA;
using RAYLIB_FUNCTIONAL::KEY_PERIOD;
using RAYLIB_FUNCTIONAL::KEY_SLASH;
using RAYLIB_FUNCTIONAL::KEY_SPACE;
using RAYLIB_FUNCTIONAL::KEY_MINUS;
using RAYLIB_FUNCTIONAL::KEY_EQUAL;
using RAYLIB_FUNCTIONAL::KEY_LEFT_BRACKET;
using RAYLIB_FUNCTIONAL::KEY_RIGHT_BRACKET;
using RAYLIB_FUNCTIONAL::KEY_BACKSLASH;
using RAYLIB_FUNCTIONAL::KEY_GRAVE;
using RAYLIB_FUNCTIONAL::KEY_BACKSPACE;
using RAYLIB_FUNCTIONAL::KEY_LEFT_CONTROL;
using RAYLIB_FUNCTIONAL::KEY_DELETE;
using RAYLIB_FUNCTIONAL::KEY_LEFT;
using RAYLIB_FUNCTIONAL::KEY_RIGHT;
using RAYLIB_FUNCTIONAL::KEY_DOWN;
using RAYLIB_FUNCTIONAL::KEY_UP;
using RAYLIB_FUNCTIONAL::KEY_NULL;
using RAYLIB_FUNCTIONAL::KEY_F1;
using RAYLIB_FUNCTIONAL::KEY_F2;
using RAYLIB_FUNCTIONAL::KEY_F3;

using RAYLIB_FUNCTIONAL::SHADER_UNIFORM_FLOAT;
using RAYLIB_FUNCTIONAL::SHADER_UNIFORM_VEC4;

#include "SUIutils.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <set>
#include <tuple>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <mutex>

class Object2D;

void updateObject2DVector(Object2D*);

struct SpecialVector2 {
	template <size_t Index>
	struct num {
		float n{};

		SpecialVector2* getOwner() {
			size_t offset = (Index == 0) ? offsetof(SpecialVector2, x) : offsetof(SpecialVector2, y);
			return reinterpret_cast<SpecialVector2*>(reinterpret_cast<char*>(this) - offset);
		}

		operator float() const { return n; }

		template <typename T>
		num& operator=(T val) {
			float new_val = static_cast<float>(val);
			if (n != new_val) {
				n = new_val;
				SpecialVector2* owner = getOwner();
				if (owner->alarmWhenChanged and owner->parentalObj) {
					updateObject2DVector(owner->parentalObj);
				}
			}
			return *this;
		}

		num& operator=(const num& other) {
			return *this = other.n;
		}

		num& operator+=(float val) { return *this = (n + val); }
		num& operator-=(float val) { return *this = (n - val); }
		num& operator*=(float val) { return *this = (n * val); }
		num& operator/=(float val) { return *this = (n / val); }
	};

	using num_x = num<0>;
	using num_y = num<1>;

	Object2D* parentalObj = nullptr;
	bool alarmWhenChanged = true;

	num_x x;
	num_y y;

	SpecialVector2() = default;

	SpecialVector2(float x_val, float y_val, Object2D* parental = nullptr) : parentalObj(parental), x{ x_val }, y{ y_val } {}

	SpecialVector2(const Vector2& other) : x{ other.x }, y{ other.y } {}

	operator Vector2() const {
		return { x.n, y.n };
	}

	SpecialVector2& operator=(const Vector2& other) {
		if (alarmWhenChanged and (x.n != other.x or y.n != other.y)) {
			alarmWhenChanged = false;
			x = other.x;
			y = other.y;
			alarmWhenChanged = true;

			if (parentalObj) {
				updateObject2DVector(parentalObj);
			}
		}
		else {
			x.n = other.x;
			y.n = other.y;
		}
		return *this;
	}
};

inline int winWidth = 0;
inline int winHeight = 0;
inline int defaultSpacing = 0;
inline float dt = 0;
inline SpecialVector2 changeWindowSize = { 0,0 };
inline bool changeWindowSizeB = false;
inline int accurateFPS = 0;
inline bool programRunning = true;
SpecialVector2 mousePosition;
SpecialVector2 mouseScreenPosition;
SpecialVector2 windowPosition;
inline constexpr const char* BASIC_FONT_NAME = "Arial";
inline constexpr const char* DEBUG_MENU_FONT_NAME = "rog";
inline std::unordered_map<std::string, Shader> Shaders;
inline long currentUniqueObjectID = 0;
bool sceneDirty = false; // true in frame where any object size or position changed

inline std::mutex ImagesLoadingMtx;
inline std::unordered_map<std::string, std::pair<Image, Texture>> loadedImages;
inline std::unordered_map<std::string, Image> pendingImages;
inline void loadImage(const std::string& name, const std::string& path) {
	ImagesLoadingMtx.lock();

	if (pendingImages.find(name) != pendingImages.end()) {
		ImagesLoadingMtx.unlock();
		std::cout << "Image: " << name << " already exists" << std::endl; 
		return; 
	}

	Image img = LoadImage(path.c_str());
	if (!img.data) {
		ImagesLoadingMtx.unlock();
		std::cout << "Image: " << name << " error while loading" << std::endl;
		return;
	}

	pendingImages.insert({ name, img });
	ImagesLoadingMtx.unlock();
}

inline void unloadImage(const std::string& name) {
	ImagesLoadingMtx.lock();

	auto it = loadedImages.find(name);
	if (it != loadedImages.end()) {
		UnloadImage(it->second.first);
		UnloadTexture(it->second.second);
		loadedImages.erase(it);
	}

	auto it1 = pendingImages.find(name);
	if (it1 != pendingImages.end()) {
		UnloadImage(it1->second);
		pendingImages.erase(it1);
	}

	ImagesLoadingMtx.unlock();
}

inline std::pair<Image, Texture> getImage(const std::string& name) {
	if (name == "") { return {}; }

	ImagesLoadingMtx.lock();
	auto it = loadedImages.find(name);
	if (it != loadedImages.end()) {
		ImagesLoadingMtx.unlock();
		return it->second;
	}

	auto it1 = pendingImages.find(name);
	if (it1 != pendingImages.end()) {
		ImagesLoadingMtx.unlock();
		return { it1->second, Texture{} };
	}
	ImagesLoadingMtx.unlock();

	std::cout << "Image " << name << " was not found" << std::endl;
	return {};
}

inline void loadNewShader(const std::string& name, const std::string& vs, const std::string& fs) {
	auto it = Shaders.find(name);
	if (it != Shaders.end()) {
		std::cout << "Shader: " << name << " already exists" << std::endl;
		return;
	}

	Shaders.emplace(name, LoadShader(vs.c_str(), fs.c_str()));
}

inline Shader getShader(const std::string& name) {
	auto it = Shaders.find(name);
	if (it == Shaders.end()) {
		std::cout << "Shader: " << name << " was not found" << std::endl;
		return Shaders.find("TextureRoundness")->second;
	}
	return it->second;
}

inline Vector2 GetMouseScreenPosition() {
	return { GetMouseScreenPositionX(), GetMouseScreenPositionY() };
}

inline Color mulColor(Color other, float t) {
	return Color{
		(unsigned char)(other.r * t),
		(unsigned char)(other.g * t),
		(unsigned char)(other.b * t),
		other.a
	};
}

inline float sui_lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

inline std::unordered_map<std::string, Font> Fonts;
inline std::vector<std::tuple<const char*, std::string, int>> queuedFonts;

inline void addFontToQueqe(const char* name, std::string path, int size) {
	queuedFonts.emplace_back(name, path, size);
}

inline void createFont(const char* name, std::string path, int size) {
	static int codepointsCount = 0;
	static int* codepoints = LoadCodepoints(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ЁАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯёабвгдежзийклмнопрстуфхцчшщъыьэюя", &codepointsCount);
	Font ft = LoadFontEx(path.c_str(), size, codepoints, codepointsCount);
	if (ft.texture.id) {
		GenTextureMipmaps(&ft.texture);
		SetTextureFilter(ft.texture, TEXTURE_FILTER_TRILINEAR);
		Fonts.emplace(name, ft);
	} else {
		UnloadFont(ft);
	}
}

class SUI_Text {
	std::string text;
	bool changed = false;
public:
	bool isChanged() const {
		return changed;
	}

	const std::string& operator!() const {
		return text;
	}

	size_t size() const {
		return text.size();
	}

	void restate() {
		changed = false;
	}

	bool empty() const {
		return text.empty();
	}

	const char* c_str() const {
		return text.c_str();
	}

	std::string substr(size_t from, size_t count = ((size_t)0 - 1)) const {
		return text.substr(from, count);
	}

	char operator[](size_t other) const {
		return text[other];
	}

	const SUI_Text& operator=(const std::string& other) {
		if (text.size() != other.size()) {
			changed = true;
		} else {
			changed = text != other;
		}

		text = other;
		return other;
	}

	const SUI_Text& operator=(const char* other) {
		std::string st = other;

		if (text.size() != st.size()) {
			changed = true;
		} else {
			changed = text != st;
		}

		text = st;
		return other;
	}

	const SUI_Text& operator=(const SUI_Text& other) {
		if (text.size() != other.size()) {
			changed = true;
		} else {
			changed = text != !other;
		}

		text = !other;
		return other;
	}

	void operator+=(const std::string& other) {
		text += other;

		if (other.size()) {
			changed = true;
		}
	}

	bool operator==(const std::string& other) {
		if (text.size() != other.size()) {
			return false;
		} else {
			return text == other;
		}
	}

	bool operator==(const char* other) {
		return !strcmp(text.c_str(), other);
	}

	bool operator!=(const char* other) {
		return strcmp(text.c_str(), other);
	}

	bool operator!=(const std::string& other) {
		if (text.size() == other.size()) {
			return text != other;
		} else {
			return true;
		}
	}

	bool operator==(const SUI_Text& other) {
		if (text.size() != other.size()) {
			return false;
		} else {
			return text == !other;
		}
	}

	operator std::string() {
		return text;
	}

	SUI_Text(const SUI_Text& other) {
		text = !other;
		changed = true;
	}

	SUI_Text(const std::string& other) {
		text = other;
		changed = true;
	}

	SUI_Text(const char* other) {
		text = other;
		changed = true;
	}
};

namespace Tasks {
	class Task;
	std::mutex TasksMutex;
	std::vector<Task*> ActiveTasks;

	class Task {
	public:
		float TimeLeft{};
		std::function<void(void)> Callback{};

		void Cancel() {
			TasksMutex.lock();
			auto obj = find(ActiveTasks.begin(), ActiveTasks.end(), this);
			if (obj != ActiveTasks.end()) {
				ActiveTasks.erase(obj);
			}
			TasksMutex.unlock();
			delete this;
		}

		Task(float TimeInSeconds, std::function<void(void)> f) : TimeLeft(TimeInSeconds), Callback(f) {  }
		~Task() {}
	};

	Task* Create(float TimeInSeconds, std::function<void(void)> f) {
		Task* t = new Task(TimeInSeconds, f);

		TasksMutex.lock();
		ActiveTasks.push_back(t);
		TasksMutex.unlock();

		return t;
	}

	void UpdateTasks(float dt) {
		TasksMutex.lock();
		for (int i = 0; i < ActiveTasks.size();) {
			if (ActiveTasks[i]->TimeLeft <= 0) {
				ActiveTasks[i]->Callback();
				delete ActiveTasks[i];

				ActiveTasks.erase(ActiveTasks.begin() + i);
			} else {
				ActiveTasks[i]->TimeLeft -= dt;
				i++;
			}
		}
		TasksMutex.unlock();
	}
}

struct IChangedSignal {
	virtual ~IChangedSignal() = default;
	virtual void Update() = 0;
};

inline std::vector<IChangedSignal*> ActiveSignals;
template <typename T>
class ChangedSignal : IChangedSignal {
public:
	std::string SignalClass = "~";

	void Update() {
		if (*SignalPTR != *LastValue) {
			Callback();
			delete LastValue;
			LastValue = new T(*SignalPTR);
		}
	}
private:
	T* SignalPTR = nullptr;
	T* LastValue;
	std::function<void(void)> Callback;
public:
	void Disconnect() {
		auto z = find(ActiveSignals.begin(), ActiveSignals.end(), this);
		if (z != ActiveSignals.end()) ActiveSignals.erase(z);
		delete this;
	}

	ChangedSignal() = delete;
	ChangedSignal(T& p, std::function<void(void)> func) : SignalPTR(&p), Callback(func), SignalClass(typeid(T).name()) {
		ActiveSignals.push_back(this);
		LastValue = new T(p);
	}
	~ChangedSignal() {
		if (!LastValue) return;
		delete LastValue;
	}
};

enum EventType {
	TICK = 0,

	MOUSE_ENTER = 10,
	MOUSE_LEAVE = 11,
	MOUSE_CLICK = 12,
	MOUSE_HOLD_START = 13,
	MOUSE_HOLD_END = 14,

	CHILD_ADDED = 20,
	CHILD_REMOVED = 21,

	TEXT_CHANGED = 30
};

enum MouseButtonType {
	NONE = -1,
	LEFT = MOUSE_BUTTON_LEFT,
	RIGHT = MOUSE_BUTTON_RIGHT,
	MIDDLE = MOUSE_BUTTON_MIDDLE
};

namespace Animate {
	enum Function {
		Linear,

		Smooth,

		Quad,
		Cube,
		Quart,
		Quint,

		Sine,
		Circular,
		Exponential,

		Back,
		Bounce
	};

	enum Ease {
		In = 0,
		Out,
	};

	float getTime(Function f, Ease e, float t) {
		t = std::clamp(t, 0.0f, 1.0f);

		if (f == Linear) { return t; }

		if (f == Quad) {
			if (e == In) { return t * t; }
			else { float a = 1.0f - t; return 1.0f - a * a; }
		}

		if (f == Cube) {
			if (e == In) { return t * t * t; }
			else { float a = 1.0f - t; return 1.0f - a * a * a; }
		}

		if (f == Exponential) {
			const float k = 3.0f;

			if (e == In) {
				return (expf(k * t) - 1.0f) / (expf(k) - 1.0f);
			} else {
				return 1.0f - (expf(k * (1.0f - t)) - 1.0f) / (expf(k) - 1.0f);
			}
		}

		if (f == Sine) {
			if (e == In) { return 1.0f - cosf((PI * t) / 2.0f); }
			else { return sinf((PI * t) / 2.0f); }
		}

		if (f == Circular) {
			if (e == In) { return 1.0f - sqrtf(1.0f - t * t); }
			else { float a = t - 1.0f; return sqrtf(1.0f - a * a); }
		}

		if (f == Bounce) {
			auto bounceOut = [](float x) -> float {
				const float n1 = 7.5625f;
				const float d1 = 2.75f;

				if (x < 1.0f / d1) return n1 * x * x;
				else if (x < 2.0f / d1) { x -= 1.5f / d1; return n1 * x * x + 0.75f; }
				else if (x < 2.5f / d1) { x -= 2.25f / d1; return n1 * x * x + 0.9375f; }
				else { x -= 2.625f / d1; return n1 * x * x + 0.984375f; }
			};

			if (e == In) {
				return 1.0f - bounceOut(1.0f - t);
			} else {
				return bounceOut(t);
			}
		}

		return t;
	}

	class Animation;
	std::unordered_map<void*, Animation*> ActiveAnimations;

	void deleteCurrent(void* ptr) {
		auto an = ActiveAnimations.find(ptr);
		if (an != ActiveAnimations.end()) {
			ActiveAnimations.erase(an);
		}
	}

	class Animation {
		void* ptr = nullptr;
		Function func = Linear;
		Ease ease = In;
		float currentTime = 0.0f;
		float endTime = 0.0f;

		int startValueI{};
		int endValueI{};
		float startValueF{};
		float endValueF{};
		Color startValueC{};
		Color endValueC{};
		SpecialVector2::num_x startValueNX{};
		SpecialVector2::num_x startValueNY{};
		SpecialVector2::num_y endValueNX{};
		SpecialVector2::num_y endValueNY{};
		SpecialVector2 startValueV{};
		SpecialVector2 endValueV{};

		const char* type = "int";
	public:
		std::function<void(void)> Completed = []() {};

		bool Update() {
			currentTime += dt;
			if (currentTime >= endTime) {
				if (type == "int") { *(int*)ptr = endValueI; }
				else if (type == "float") { *(float*)ptr = endValueF; }
				else if (type == "color") { *(Color*)ptr = endValueC; }
				else if (type == "vector2") { *(SpecialVector2*)ptr = endValueV; }
				else if (type == "numx") { *(SpecialVector2::num_x*)ptr = endValueNX; }
				else if (type == "numy") { *(SpecialVector2::num_y*)ptr = endValueNY; }
				return true;
			}
			if (type == "int") { *(int*)ptr = sui_lerp(startValueI, endValueI, getTime(func, ease, currentTime / endTime)); }
			else if (type == "float") { *(float*)ptr = sui_lerp(startValueF, endValueF, getTime(func, ease, currentTime / endTime)); }
			else if (type == "color") { *(Color*)ptr = ColorLerp(startValueC, endValueC, getTime(func, ease, currentTime / endTime)); }
			else if (type == "vector2") { *(SpecialVector2*)ptr = SpecialVector2{ sui_lerp(startValueV.x, endValueV.x, getTime(func, ease, currentTime / endTime)), sui_lerp(startValueV.y, endValueV.y, getTime(func, ease, currentTime / endTime)) }; }
			else if (type == "numx") { *(SpecialVector2::num_x*)ptr = sui_lerp(startValueNX, endValueNX, getTime(func, ease, currentTime / endTime));}
			else if (type == "numy") { *(SpecialVector2::num_y*)ptr = sui_lerp(startValueNY, endValueNY, getTime(func, ease, currentTime / endTime));}
			return false;
		}

		Animation() = delete;
		Animation(int* ptr, float time, int endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueI(*ptr), endValueI(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(float* ptr, float time, float endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueF(*ptr), endValueF(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(Color* ptr, float time, Color endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueC(*ptr), endValueC(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(SpecialVector2* ptr, float time, SpecialVector2 endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueV(*ptr), endValueV(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(SpecialVector2::num_x* ptr, float time, float endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueNX(*ptr), endValueNX(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(SpecialVector2::num_y* ptr, float time, float endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueNY(*ptr), endValueNY(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
	};

	Animation* Create(int* ptr, float time, int endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "int", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}
	Animation* Create(float* ptr, float time, float endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "float", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}
	Animation* Create(Color* ptr, float time, Color endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "color", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}
	Animation* Create(SpecialVector2* ptr, float time, SpecialVector2 endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "vector2", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}
	Animation* Create(SpecialVector2::num_x* ptr, float time, float endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "numx", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}
	Animation* Create(SpecialVector2::num_y* ptr, float time, float endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "numy", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}

	void UpdateAnimations(float t) {
		for (auto it = ActiveAnimations.begin(); it != ActiveAnimations.end();) {
			if (it->second->Update()) {
				Animation* sas = it->second;
				it = ActiveAnimations.erase(it);
				sas->Completed();
				delete sas;
			}
			else {
				it++;
			}
		}
	}
};

enum class TextAnchorEnum {
	N = 0,
	NE = 1,
	E = 2,
	SE = 3,
	S = 4,
	SW = 5,
	W = 6,
	NW = 7,
	CENTER = 8,
};

inline SpecialVector2 getTextOffset(TextAnchorEnum anchor) {
	float offsetX{};
	float offsetY{};

	switch (anchor) {
	case TextAnchorEnum::N: { offsetX = 0.5; offsetY = 0; break; }
	case TextAnchorEnum::NE: { offsetX = 1; offsetY = 0; break; }
	case TextAnchorEnum::E: { offsetX = 1; offsetY = 0.5; break; }
	case TextAnchorEnum::SE: { offsetX = 1; offsetY = 1; break; }
	case TextAnchorEnum::S: { offsetX = 0.5; offsetY = 1; break; }
	case TextAnchorEnum::SW: { offsetX = 0; offsetY = 1; break; }
	case TextAnchorEnum::W: { offsetX = 0; offsetY = 0.5; break; }
	case TextAnchorEnum::NW: { offsetX = 0; offsetY = 0; break; }
	default: { offsetX = 0.5; offsetY = 0.5; };
	}

	return { offsetX, offsetY };
}

inline Vector3 getTextCFrame(const char* text, Font font, Rectangle rec, TextAnchorEnum anchor, int maxTextSize, int Spacing) {
	if (maxTextSize < 0 or maxTextSize > rec.height) maxTextSize = rec.height;

	int endX{};
	int endY{};
	float endSize = 1;
	float sizeMax = maxTextSize;
	SpecialVector2 textSize{};

	while (endSize < sizeMax) {
		float middle = (endSize + sizeMax + 1) / 2;
		textSize = MeasureTextEx(font, text, middle, Spacing);
		if (textSize.x <= rec.width and textSize.y <= rec.height) endSize = middle;
		else sizeMax = middle - 1;
	}

	if (endSize > maxTextSize) endSize = maxTextSize;
	SpecialVector2 ofst = getTextOffset(anchor);
	float offsetX = ofst.x;
	float offsetY = ofst.y;

	endX = offsetX * (rec.width - textSize.x); if (endX < 0) endX = 0;
	endY = offsetY * (rec.height - textSize.y);  if (endX < 0) endX = 0;

	return { (float)endX, (float)endY, (float)endSize };
}

class TextLabel;
class Instance;
class Object2D;
class TextBox;
class ImageLabel;
class ScrollFrame;
class TextureLabel;
class LineEx;

inline TextBox* FocusedTextBox = nullptr;
inline Object2D* PreviousHigherObject = nullptr;
inline Object2D* higherObject = nullptr;

inline std::unordered_map<long, Instance*> deletedObjectsByID;
inline std::unordered_map<Instance*, long> deletedObjectsByPtr;

enum InstanceType {
	INSTANCE = 0,
	OBJECT2D,
	TEXTLABEL,
	TEXTBOX,
	IMAGELABEL,
	SCROLLFRAME,
	TEXTURELABEL,
	LINEEX,

	STRING_VALUE,
	INT_VALUE,
	BOOL_VALUE,
	FLOAT_VALUE,
	OBJECT_VALUE,
	ADDRESS_VALUE,
	VECTOR2_VALUE,
	COLOR_VALUE,

	FOLDER
};

Instance* getAncestorWhichParentIsScrollFrame(Instance* ptr);

template<typename Z>
inline void Delete(Z* ptr) {
	if (!ptr) return;

	if (ptr->Parent) {
		ptr->Parent->childsRemovedInFrame.insert({ ptr->uniqueID, ptr});
		deletedObjectsByID.insert({ptr->uniqueID, ptr});
		deletedObjectsByPtr.insert({ptr, ptr->uniqueID});

		auto it = ptr->Parent->childsAddedInFrame.find(ptr->uniqueID);
		if (it != ptr->Parent->childsAddedInFrame.end()) {
			ptr->Parent->childsAddedInFrame.erase(it);
		}

		Instance* scrollChild = getAncestorWhichParentIsScrollFrame(ptr);

		if (scrollChild) {
			static_cast<ScrollFrame*>(scrollChild->Parent)->UpdateSectors(scrollChild);
		}
	}

	std::vector<Instance*> z = ptr->Children;
	for (int i = 0; i < z.size(); i++) {
		Instance* child = z[i];
		Delete(child);
	}

	ptr->setParent(nullptr);
	ptr->Children.clear();
	z.clear();

	delete ptr;
	ptr = nullptr;
}

inline void updateChildren(Instance*);

struct InstanceCallback {
	std::function<void(Instance*, Instance*)> func;

	InstanceCallback() = default;

	template<typename F, typename=std::enable_if_t< !std::is_same_v<std::decay_t<F>, InstanceCallback>>>
	InstanceCallback(F&& f) {
		if constexpr (std::is_invocable_v<F, Instance*, Instance*>) {
			func = std::forward<F>(f);
		} else if constexpr (std::is_invocable_v<F, Instance*>) {
			func = [f = std::forward<F>(f)](Instance* a, Instance*) mutable {
				f(a);
			};
		}
	}

	void operator()(Instance* a, Instance* b = nullptr) const {
		if (func) {
			func(a, b);
		}
	}
};

size_t framesSinceStart = 0;

class Instance {
protected:
	size_t lastUpdateFrame = 0;
public:
	const long uniqueID = -1;
	std::unordered_map<long, Instance*> childsAddedInFrame;
	std::unordered_map<long, Instance*> childsRemovedInFrame;
private:
	std::vector<std::pair<EventType, InstanceCallback>> events;

	void AddEvent(EventType t, InstanceCallback f, MouseButtonType m);
public:
	bool hasEvent(EventType t) const {
		for (auto& [type, _] : events) {
			if (type == t) {
				return true;
			}
		}

		return false;
	}

	bool updateChildrenZIndex = true;

	Instance* Parent = nullptr;
	std::vector<Instance*> Children;

	std::string Name = "Instance";
	InstanceType Class;

	bool __ParentObject{};

	Instance(bool a) : __ParentObject(true), uniqueID(currentUniqueObjectID++) {};
	Instance(Instance* p) : Parent(p), uniqueID(currentUniqueObjectID++) {
		if (p) { 
			p->Children.push_back(this); 
			p->childsAddedInFrame.insert({ p->uniqueID, this }); 
			p->updateChildrenZIndex = true; 
		}
	}
	Instance() = delete;

	virtual ~Instance() {}

	void setParent(Instance* ptr) {
		if (ptr == this) return;

		sceneDirty = true;

		if (Parent != nullptr) {
			std::vector<Instance*> arr;
			for (Instance* obj : Parent->Children) {
				arr.push_back(obj);
			}
			for (int i = 0; i < arr.size(); i++) {
				if (arr[i] == this) {
					Parent->Children.erase(Parent->Children.begin() + i);
					break;
				}
			}

			if (Parent->childsRemovedInFrame.find(this->uniqueID) == Parent->childsRemovedInFrame.end()) {
				Parent->childsRemovedInFrame.insert({ this->uniqueID, ptr });
			}
		}

		Parent = ptr;
		if (ptr) {
			ptr->Children.push_back(this);
			ptr->updateChildrenZIndex = true;
			ptr->childsAddedInFrame.insert({ this->uniqueID, this });
		}
	}

	Instance* findChild(const std::string& name) const {
		for (auto obj : Children) {
			if (obj->Name == name) {
				return obj;
			}
		}

		return nullptr;
	}

	Instance* findChildOfClass(InstanceType cls) {
		for (Instance*& obj : Children) {
			if (obj->Class == cls) {
				return obj;
			}
		}

		return nullptr;
	}

	bool isAncestorOf(Instance* other) const {
		Instance* ptr = other;

		while (ptr->Parent != nullptr and !ptr->__ParentObject) {
			if (ptr->Parent == this) return true;
			ptr = ptr->Parent;
		}

		return false;
	}

	Instance* findFirstAncestorOfClass(InstanceType cls) {
		Instance* ptr = this;

		while (ptr->Parent != nullptr and !ptr->__ParentObject) {
			if (ptr->Parent->Class == cls) return ptr->Parent;
			ptr = ptr->Parent;
		}

		return nullptr;
	}

	Instance* findFirstDescendantOfClass(InstanceType cls) {
		std::function<Instance* (Instance*)> l = [=](Instance* ptr) -> Instance* {
			for (Instance* child : ptr->Children) {
				if (child->Class == cls) {
					return child;
				}

				Instance* res = l(child);

				if (res) {
					return res;
				}
			}

			return nullptr;
			};

		return l(this);
	}

	Instance* findFirstDescendant(const std::string& name) {
		std::function<Instance* (Instance*)> l;

		l = [&](Instance* ptr) -> Instance* {
			for (Instance* child : ptr->Children) {
				if (child->Name == name) {
					return child;
				}

				Instance* res = l(child);

				if (res) {
					return res;
				}
			}

			return nullptr;
			};

		return l(this);
	}

	std::vector<Instance*> getDescendants(const std::function<bool(Instance*)>& condition = [](Instance* _) { return true; }) {
		std::vector<Instance*> out;

		std::function<void(Instance*)> l = [&](Instance* ptr) {
			for (Instance* child : ptr->Children) {
				if (condition(child)) {
					out.push_back(child);
				}

				l(child);
			}
		};

		l(this);

		return out;
	}

	void deleteAllChildren() {
		while (Children.size() > 0) {
			Delete(Children[0]);
		}
	}

	bool isDescendantOf(Instance* maybeAncestor) const {
		const Instance* ptr = this;

		while (ptr->Parent != nullptr and !ptr->__ParentObject) {
			if (ptr->Parent == maybeAncestor) return true;
			ptr = ptr->Parent;
		}

		return false;
	}

	virtual void eventHandler() {
		for (const auto& [type, func] : events) {
			if (type == TICK) {
				func(this);
			} else if (type == CHILD_ADDED) {
				for (auto& [id, ptr] : childsAddedInFrame) {
					if (childsRemovedInFrame.contains(id)) continue;
					func(this, ptr);
				}
			} else if (type == CHILD_REMOVED) {
				for (auto& [id, ptr] : childsRemovedInFrame) {
					if (childsAddedInFrame.contains(id)) continue;
					func(this, ptr);
				}
			}
		}

		childsAddedInFrame.clear();
		childsRemovedInFrame.clear();
	}

	virtual void Update() {
		if (lastUpdateFrame == framesSinceStart) return;
		lastUpdateFrame = framesSinceStart;

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		eventHandler();

		for (int i = 0; i < Children.size(); i++) {
			Instance* child = Children[i];
			child->Update();
		}
	}

	virtual Instance* Clone() const {
		Instance* i = new Instance(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		return i;
	}
};

Instance* getAncestorWhichParentIsScrollFrame(Instance* ptr) {
	while (ptr->Parent != nullptr and !ptr->__ParentObject) {
		if (ptr->Parent->Class == SCROLLFRAME) return ptr;
		ptr = ptr->Parent;
	}

	return nullptr;
}

bool Is2DInheritor(Instance* obj) {
	if (obj->Class == INSTANCE or
		obj->Class == LINEEX or
		obj->Class == STRING_VALUE or
		obj->Class == BOOL_VALUE or
		obj->Class == VECTOR2_VALUE or
		obj->Class == INT_VALUE or
		obj->Class == FLOAT_VALUE or
		obj->Class == OBJECT_VALUE or
		obj->Class == ADDRESS_VALUE or
		obj->Class == COLOR_VALUE or
		obj->Class == FOLDER
		) {
		return false;
	}

	return true;
}

bool Is2DInheritor(InstanceType type) {
	if (type == INSTANCE or
		type == LINEEX or
		type == STRING_VALUE or
		type == BOOL_VALUE or
		type == VECTOR2_VALUE or
		type == INT_VALUE or
		type == FLOAT_VALUE or
		type == OBJECT_VALUE or
		type == ADDRESS_VALUE or
		type == COLOR_VALUE or
		type == FOLDER
		) {
		return false;
	}

	return true;
}

class StringValue : public Instance {
	constexpr static const char* DefaultName = "StringValue";
	constexpr static InstanceType DefaultClass = STRING_VALUE;
public:
	std::string Value = "";

	StringValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	StringValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	StringValue() = delete;
};

class ObjectValue : public Instance {
	constexpr static const char* DefaultName = "ObjectValue";
	constexpr static InstanceType DefaultClass = OBJECT_VALUE;
public:
	Instance* Value = nullptr;

	ObjectValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	ObjectValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	ObjectValue() = delete;
};

template<typename T>
class AddressValue : public Instance {
	constexpr static const char* DefaultName = "AddressValue";
	constexpr static InstanceType DefaultClass = ADDRESS_VALUE;
public:
	T* Value = nullptr;

	AddressValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	AddressValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	AddressValue() = delete;
};

class BoolValue : public Instance {
	constexpr static const char* DefaultName = "BoolValue";
	constexpr static InstanceType DefaultClass = BOOL_VALUE;
public:
	bool Value = 0;

	BoolValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	BoolValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	BoolValue() = delete;
};

class IntValue : public Instance {
	constexpr static const char* DefaultName = "IntValue";
	constexpr static InstanceType DefaultClass = INT_VALUE;
public:
	int Value = 0;

	IntValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	IntValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	IntValue() = delete;
};

class FloatValue : public Instance {
	constexpr static const char* DefaultName = "FloatValue";
	constexpr static InstanceType DefaultClass = FLOAT_VALUE;
public:
	float Value = 0.0f;

	FloatValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	FloatValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	FloatValue() = delete;
};

class Vector2Value : public Instance {
	constexpr static const char* DefaultName = "Vector2Value";
	constexpr static InstanceType DefaultClass = VECTOR2_VALUE;
public:
	SpecialVector2 Value = { 0,0 };

	Vector2Value(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	Vector2Value(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	Vector2Value() = delete;
};

class ColorValue : public Instance {
	constexpr static const char* DefaultName = "ColorValue";
	constexpr static InstanceType DefaultClass = COLOR_VALUE;
public:
	Color Value = { 255,255,255,255 };

	ColorValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	ColorValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	ColorValue() = delete;
};

class Folder : public Instance {
	constexpr static const char* DefaultName = "Folder";
	constexpr static InstanceType DefaultClass = FOLDER;
public:

	Folder(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	Folder(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	Folder() = delete;
};

inline SpecialVector2 getCanvasRealPos(Object2D*);
inline SpecialVector2 getScrollFrameRS(Instance*);
inline SpecialVector2 getScrollFrameRP(Instance*);
inline bool isScrollFrameCropping(Instance*);

enum SUI_EEC {
	EEC_DEFAULT = 0,
	EEC_EVERY_ENTER,
	EEC_IF_DESCENDANT_HIGHER
};

class Object2D : public Instance {
	constexpr static const char* DefaultName = "Object2D";
	constexpr static InstanceType DefaultClass = OBJECT2D;

	bool startedOnObject1 = false;
	bool startedOnObject2 = false;
	bool startedOnObject3 = false;

	bool lastActive = Active;
	int lastZIndex = ZIndex;
protected:
	SpecialVector2 RelativePosition{};
	SpecialVector2 RelativeSize{};
	std::vector<std::tuple<EventType, InstanceCallback, MouseButtonType>> events;

	void SameUpdate() {
		if (Active != lastActive or lastZIndex != ZIndex) {
			lastActive = Active;
			lastZIndex = ZIndex;
			
			if (Parent) {
				Parent->updateChildrenZIndex = true;
			}
		}

		childsRemovedInFrame.clear();
		childsAddedInFrame.clear();
	} 

	void eventHandler();
	void PosOrSizeChanged();
	void updateAncestorWhichParentIsScroll();
public:
	bool RelativePCalculated = false;
	bool RelativeSCalculated = false;
	void VectorChanged() {
		PosOrSizeChanged();
	}

	bool hasEvent(EventType t) const {
		for (auto& [type, _, __] : events) {
			if (type == t) {
				return true;
			}
		}

		return false;
	}

	SpecialVector2 RealSize{0,0,this}; // Absolute size in pixels (not for changing from somewhere)
	SpecialVector2 RealPos{}; // Absolute position in pixels (not for changing from somewhere)
	SUI_EEC EnterEventCondition = SUI_EEC::EEC_DEFAULT;
	SpecialVector2 PositionOFFSET = { 0,0,this };
	SpecialVector2 SizeOFFSET = {};
	SpecialVector2 AnchorPositionOFFSET = { 0,0,this };
	SpecialVector2 Position{ 0,0,this };
	SpecialVector2 Size{};
	SpecialVector2 AnchorPosition{ 0,0,this };

	float BackgroundTransparency{};
	Color BackgroundColor = { 0,0,0,255 };
	bool Visible = true;

	float Roundness = 0.0f;
	short Segments = 5;

	short BorderThickness{};
	float BorderTransparency{};
	Color BorderColor{};

	int ZIndex = 0;
	bool Active = false;

	void getRealObject2Dsize() {
		SpecialVector2 sizePx = {};
		Object2D* self = this;
		Instance* current = Parent;
		Object2D* parent2D = nullptr;

		while (current) {
			if (!Is2DInheritor(current)) {
				if (current->Parent) { current = current->Parent; continue; }
				parent2D = nullptr;
				break;
			}
			parent2D = static_cast<Object2D*>(current);
			if (parent2D->__ParentObject) { parent2D = nullptr; break; }
			break;
		}

		SpecialVector2 parentSizePx = parent2D ? parent2D->RealSize : SpecialVector2{ static_cast<float>(winWidth), static_cast<float>(winHeight) };

		sizePx.x = parentSizePx.x * self->Size.x + self->SizeOFFSET.x;
		sizePx.y = parentSizePx.y * self->Size.y + self->SizeOFFSET.y;

		RelativeSCalculated = true;
		RealSize = sizePx;
		RelativeSize = SpecialVector2{ sizePx.x / winWidth, sizePx.y / winHeight };
	}

	void getRealObject2Dposition() {
		if (!RelativeSCalculated) getRealObject2Dsize();

		SpecialVector2 posPx = { 0.0f, 0.0f };
		SpecialVector2 sizePx = RealSize;

		SpecialVector2 anchorPx = {
			sizePx.x * AnchorPosition.x + AnchorPositionOFFSET.x,
			sizePx.y * AnchorPosition.y + AnchorPositionOFFSET.y
		};

		SpecialVector2 localPx = {
			0.0f,
			0.0f
		};

		Instance* current = Parent;
		while (current) {
			if (!Is2DInheritor(current)) { current = current->Parent; continue; }

			Object2D* obj = static_cast<Object2D*>(current);
			if (!obj->RelativeSCalculated) obj->getRealObject2Dsize();
			if (!obj->RelativePCalculated) obj->getRealObject2Dposition();

			SpecialVector2 parentSizePx = obj->RealSize;

			SpecialVector2 parentAnchorPx = {
				obj->RealSize.x * obj->AnchorPosition.x + obj->AnchorPositionOFFSET.x,
				obj->RealSize.y * obj->AnchorPosition.y + obj->AnchorPositionOFFSET.y
			};

			SpecialVector2 parentLocalPx = {
				obj->Position.x * parentSizePx.x + obj->PositionOFFSET.x - parentAnchorPx.x,
				obj->Position.y * parentSizePx.y + obj->PositionOFFSET.y - parentAnchorPx.y
			};

			SpecialVector2 parentPosPx = obj->RealPos;
			if (!obj->RelativePCalculated) parentPosPx = parentLocalPx;

			SpecialVector2 myLocalPx = {
				parentSizePx.x * Position.x + PositionOFFSET.x - anchorPx.x,
				parentSizePx.y * Position.y + PositionOFFSET.y - anchorPx.y
			};

			if (obj->Class == SCROLLFRAME) {
				SpecialVector2 canvasPx = getCanvasRealPos(obj);
				posPx.x = parentPosPx.x + myLocalPx.x - canvasPx.x;
				posPx.y = parentPosPx.y + myLocalPx.y - canvasPx.y;
			} else {
				posPx.x = parentPosPx.x + myLocalPx.x;
				posPx.y = parentPosPx.y + myLocalPx.y;
			}

			RelativePosition = SpecialVector2{ posPx.x / winWidth, posPx.y / winHeight };
			RealPos = posPx;
			RelativePCalculated = true;
			return;
		}

		SpecialVector2 rootSizePx = { (float)winWidth, (float)winHeight };
		SpecialVector2 rootLocalPx = {
			rootSizePx.x * Position.x + PositionOFFSET.x - anchorPx.x,
			rootSizePx.y * Position.y + PositionOFFSET.y - anchorPx.y
		};

		RealPos = rootLocalPx;
		RelativePosition = SpecialVector2{ RealPos.x / winWidth, RealPos.y / winHeight };
		RelativePCalculated = true;
	}

	SpecialVector2 getMousePosition() {
		SpecialVector2 mousePos = mousePosition;
		return { (mousePos.x - RealPos.x) / RealSize.x, (mousePos.y - RealPos.y) / RealSize.y };
	}

	virtual void Draw() {
		if (Visible) {
			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			if (BackgroundTransparency != 1) {
				DrawRectangleRounded({ RealPos.x, RealPos.y, RealSize.x, RealSize.y }, Roundness, Segments, { BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, (unsigned char)(BackgroundColor.a * (1 - BackgroundTransparency)) });
			}

			if (BorderThickness > 0) {
				DrawRectangleRoundedLinesEx({ RealPos.x, RealPos.y, RealSize.x, RealSize.y }, Roundness, Segments, BorderThickness, { BorderColor.r, BorderColor.g, BorderColor.b, (unsigned char)(BorderColor.a * (1 - BorderTransparency)) });
			}
		}
	}

	bool pointInObject(SpecialVector2 pos) {
		SpecialVector2 mouse = mouseScreenPosition;
		SpecialVector2 windowPos = windowPosition;

		int width = winWidth;
		int height = winHeight;

		if (!(mouse.x >= windowPos.x and
			mouse.x <= windowPos.x + width and
			mouse.y >= windowPos.y and
			mouse.y <= windowPos.y + height)) return false;
		if (Parent and Parent->Class == SCROLLFRAME) {
			SpecialVector2 scrRS = getScrollFrameRS(Parent);
			SpecialVector2 scrRP = getScrollFrameRP(Parent);
			bool cropping = isScrollFrameCropping(Parent);
			if (cropping) {
				if (scrRP.x > pos.x or scrRP.x + scrRS.x < pos.x or
					scrRP.y > pos.y or scrRP.y + scrRS.y < pos.y) {
					return false;
				}
			} else {
				if (pos.x >= RealPos.x and pos.x <= RealPos.x + RealSize.x and pos.y >= RealPos.y and pos.y <= RealPos.y + RealSize.y) return true;
			}
		}

		if (pos.x >= RealPos.x and pos.x <= RealPos.x + RealSize.x and pos.y >= RealPos.y and pos.y <= RealPos.y + RealSize.y) return true;

		return false;
	}

	bool MouseEntered = false;

	void AddEvent(EventType t, InstanceCallback f, MouseButtonType m = NONE);

	void Update() override {
		if (lastUpdateFrame == framesSinceStart) return;
		lastUpdateFrame = framesSinceStart;

		RelativeSCalculated = false;
		RelativePCalculated = false;
		if (!Visible) return;

		SameUpdate();

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		getRealObject2Dsize();
		getRealObject2Dposition();
		eventHandler();
		Draw();

		for (int i = 0; i < Children.size(); i++) {
			Instance* child = Children[i];
			child->Update();
		}
	}

	Object2D* Clone() const override {
		Object2D* i = new Object2D(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		return i;
	}

	Object2D(bool a) : Instance(a) { 
		Name = DefaultName; 
		Class = DefaultClass;

		updateAncestorWhichParentIsScroll();
	};

	Object2D(Instance* p) : Instance(p) { 
		Name = DefaultName; 
		Class = DefaultClass;

		updateAncestorWhichParentIsScroll();
	}

	Object2D() = delete;
};

void updateObject2DVector(Object2D* o) {
	o->VectorChanged();
}

class LineEx : public Instance { // it cannot contain Object2D inheritors inside itself  |  only necessary for drawing lines  | Unstable
	constexpr static const char* DefaultName = "LineEx";
	constexpr static InstanceType DefaultClass = LINEEX;

	std::pair<SpecialVector2, SpecialVector2> getRealObject2Dposition() {
		SpecialVector2 pos1 = { Position1.x, Position1.y };
		SpecialVector2 pos2 = { Position2.x, Position2.y };
		Instance* current = Parent;

		while (current) {
			Object2D* obj = dynamic_cast<Object2D*>(current);
			if (!obj) {
				if (current->Parent) {
					current = current->Parent;
					continue;
				}
				break;
			}

			if (obj->__ParentObject) break;

			SpecialVector2 parentPos = {
				obj->Position.x - obj->AnchorPosition.x * obj->Size.x,
				obj->Position.y - obj->AnchorPosition.y * obj->Size.y
			};

			if (obj->Class == SCROLLFRAME) {
				SpecialVector2 CanvasPosition = getCanvasRealPos(obj);

				pos1.x = parentPos.x + (pos1.x - CanvasPosition.x);
				pos1.y = parentPos.y + (pos1.y - CanvasPosition.y);

				pos2.x = parentPos.x + (pos2.x - CanvasPosition.x);
				pos2.y = parentPos.y + (pos2.y - CanvasPosition.y);
			}
			else {
				pos1.x = parentPos.x + pos1.x * obj->Size.x;
				pos1.y = parentPos.y + pos1.y * obj->Size.y;

				pos2.x = parentPos.x + pos2.x * obj->Size.x;
				pos2.y = parentPos.y + pos2.y * obj->Size.y;
			}

			current = obj->Parent;
		}

		return { {pos1.x * winWidth, pos1.y * winHeight}, {pos2.x * winWidth, pos2.y * winHeight} };
	}

public:
	SpecialVector2 Position1{};
	SpecialVector2 Position2{};
	Color LineColor{};
	int Thickness = 5;
	int ZIndex = 0;
	bool Visible = true;

	void Draw() {
		if (Visible and Thickness) {
			auto [pos1, pos2] = getRealObject2Dposition();
			DrawLineEx(pos1, pos2, Thickness, LineColor);
		}
	}

	void Update() override {
		if (lastUpdateFrame == framesSinceStart) return;
		lastUpdateFrame = framesSinceStart;

		eventHandler();
		Draw();
	}

	LineEx* Clone() const override {
		LineEx* i = new LineEx(*this);
		i->Parent = nullptr;
		i->Children.clear();

		return i;
	}

	LineEx(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	LineEx(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	LineEx() = delete;
};

inline void updateChildren(Instance* parent) {
	if (!parent) return;
	parent->updateChildrenZIndex = false;

	std::sort(parent->Children.begin(), parent->Children.end(), [](Instance* a, Instance* b) {
		auto az = dynamic_cast<Object2D*>(a);
		auto bz = dynamic_cast<Object2D*>(b);

		LineEx* az2 = nullptr;
		LineEx* bz2 = nullptr;

		if (!az and a->Class == LINEEX)
			az2 = dynamic_cast<LineEx*>(a);
		if (!bz and b->Class == LINEEX)
			bz2 = dynamic_cast<LineEx*>(b);

		if (az and bz) return az->ZIndex < bz->ZIndex;
		if (az2 and bz2) return az2->ZIndex < bz2->ZIndex;
		if (az2 and bz) return az2->ZIndex < bz->ZIndex;
		if (az and bz2) return az->ZIndex < bz2->ZIndex;
		if (az) return true;
		if (bz) return false;
		return true;
	});
}

inline Font getFont(const std::string& name) {
	auto it = Fonts.find(name);
	if (it != Fonts.end())
		return it->second;

	return Fonts.find(BASIC_FONT_NAME)->second;
}

struct Clip {
	int x, y, w, h;
};
inline std::vector<Clip> clipStack;

inline Clip Intersect(const Clip& a, const Clip& b) {
	int x1 = std::max(a.x, b.x);
	int y1 = std::max(a.y, b.y);
	int x2 = std::min(a.x + a.w, b.x + b.w);
	int y2 = std::min(a.y + a.h, b.y + b.h);
	if (x2 <= x1 or y2 <= y1) return { 0, 0, 0, 0 };
	return { x1, y1, x2 - x1, y2 - y1 };
}

inline void PushClip(Clip last) {
	if (!clipStack.empty())
		last = Intersect(clipStack.back(), last);

	clipStack.push_back(last);
	BeginScissorMode(last.x, last.y, last.w, last.h);
}

inline void PopClip() {
	EndScissorMode();
	clipStack.pop_back();
	if (!clipStack.empty()) {
		Clip last = clipStack.back();
		BeginScissorMode(last.x, last.y, last.w, last.h);
	}
}

class ScrollFrame : public Object2D {
	constexpr static const char* DefaultName = "ScrollFrame";
	constexpr static InstanceType DefaultClass = SCROLLFRAME;
	constexpr static unsigned int GridSectorSize = 512;

	struct ScrollSector {
		int X = 0;
		int Y = 0;
		std::unordered_map<long, Instance*> Objects;
	};

	std::unordered_map<int, std::unordered_map<int, ScrollSector*>> Grid;
	std::unordered_map<long, std::vector<ScrollSector*>> SectorsOnObject;

	std::vector<Instance*> Tick;
	std::unordered_map<long, Instance*> isTick;
public:
	std::vector<ScrollSector*> sectorsOnView;
private:
	void SectorsAddChild(Instance* child) {
		if (!child) return;

		UpdateSectors(child);

		if (child->hasEvent(TICK)) {
			Tick.push_back(child);
			isTick.insert({ child->uniqueID, child });
		}
	}

	void SectorsRemoveChild(long childID) {
		if (childID == -1) return;

		auto it4 = toUpdateSectors.find(childID);
		if (it4 != toUpdateSectors.end()) {
			toUpdateSectors.erase(it4);
		}

		auto it = SectorsOnObject.find(childID);
		if (it != SectorsOnObject.end()) {
			for (ScrollSector* sector : it->second) {
				auto it2 = sector->Objects.find(childID);
				if (it2 != sector->Objects.end()) {
					sector->Objects.erase(it2);
				}
			}
			SectorsOnObject.erase(it);
		}

		auto it2 = isTick.find(childID);
		if (it2 != isTick.end()) {
			Instance* ptr = it2->second;
			isTick.erase(it2);
			for (int i = 0; i < Tick.size(); i++) {
				if (Tick[i] == ptr) {
					Tick.erase(Tick.begin() + i);
					break;
				}
			}
		}
	}

	std::vector<std::pair<int, int>> getSectors(Instance* generalObj) const {
		std::vector<std::pair<int, int>> sectors;

		static std::function<void(Instance*, std::vector<std::pair<int, int>>&)> sectorsCalculate = [](Instance* obj, std::vector<std::pair<int, int>>& sect) {
			if (Is2DInheritor(obj)) {
				Object2D* casted = static_cast<Object2D*>(obj);
				if (!casted->RelativeSCalculated) {
					casted->getRealObject2Dsize();
				}

				SpecialVector2 pos = { casted->Position.x * casted->Size.x + casted->PositionOFFSET.x - (casted->BorderTransparency != 1 ? casted->BorderThickness : 0), casted->Position.y * casted->Size.y + casted->PositionOFFSET.y - (casted->BorderTransparency != 1 ? casted->BorderThickness : 0) };
				SpecialVector2 lastpos = { pos.x + casted->RealSize.x + (casted->BorderTransparency != 1 ? casted->BorderThickness*2 : 0), pos.y + casted->RealSize.y + (casted->BorderTransparency != 1 ? casted->BorderThickness * 2 : 0) };


				for (int i = pos.x / GridSectorSize; i <= lastpos.x / GridSectorSize; i++) {
					for (int j = pos.y / GridSectorSize; j <= lastpos.y / GridSectorSize; j++) {
						sect.push_back({ i, j });
					}
				}
			}

			for (Instance* child : obj->Children) {
				sectorsCalculate(child, sect);
			}
		};

		sectorsCalculate(generalObj, sectors);

		return sectors;
	}

	void addObjToSector(Instance* obj, int x, int y) {
		ScrollSector* sector = nullptr;
		auto it1 = Grid.find(x);
		bool founded = false;

		if (it1 != Grid.end()) {
			auto it2 = it1->second.find(y);
			if (it2 != it1->second.end()) {
				sector = it2->second;
				founded = true;
			}
		} else {
			Grid[x] = {};
		}

		if (not founded) {
			sector = new ScrollSector();
			sector->X = x;
			sector->Y = y;

			Grid[x][y] = sector;
		}

		sector->Objects.insert({obj->uniqueID, obj});

		auto it3 = SectorsOnObject.find(obj->uniqueID);

		if (it3 == SectorsOnObject.end()) {
			SectorsOnObject[obj->uniqueID] = { sector };
		} else {
			SectorsOnObject[obj->uniqueID].push_back(sector);
		}
	}
private:
	SpecialVector2 lastFullSize{};
	SpecialVector2 lastCanvasFullPosition{};

	void checkAndUpdateCurrentSectors(bool force = false) {
		SpecialVector2 fullSize = RealSize;
		SpecialVector2 fullPos = { CanvasPosition.x * RealSize.x + CanvasPositionOFFSET.x, CanvasPosition.y * RealSize.y + CanvasPositionOFFSET.y }; 
		
		if (force or lastFullSize.x != fullSize.x or lastFullSize.y != fullSize.y or
			lastCanvasFullPosition.x != fullPos.x or lastCanvasFullPosition.y != fullPos.y) {
			lastFullSize = fullSize;
			lastCanvasFullPosition = fullPos;
			sectorsOnView.clear();
			SpecialVector2 pos = fullPos;
			SpecialVector2 lastpos = { fullPos.x + fullSize.x, fullPos.y + fullSize.y };

			Vector2 start = {
				std::floor(pos.x / GridSectorSize),
				std::floor(pos.y / GridSectorSize)
			};

			Vector2 end = {
				std::floor(lastpos.x / GridSectorSize),
				std::floor(lastpos.y / GridSectorSize)
			};

			for (int i = start.x; i <= end.x; i++) {
				for (int j = start.y; j <= end.y; j++) {
					ScrollSector* s = nullptr;
					auto f = Grid.find(i);
					if (f == Grid.end()) {
						continue;
					}

					auto f2 = f->second.find(j);

					if (f2 == f->second.end()) {
						continue;
					} else {
						s = f2->second;
					}

					sectorsOnView.push_back(s);
				}
			}
		}
	}

	std::unordered_map<long, Instance*> toUpdateSectors;
	void secUpd(Instance* child) {
		if (!child) return;
		auto checkIt = SectorsOnObject.find(child->uniqueID);
		if (checkIt == SectorsOnObject.end()) { // new object in Scroll
			SectorsOnObject.insert({ child->uniqueID, {} });
			std::vector<std::pair<int, int>> sectors = getSectors(child);

			for (auto& [x, y] : sectors) {
				addObjToSector(child, x, y);
			}
		} else { // updating current sector
			for (ScrollSector* sector : checkIt->second) {
				auto it2 = sector->Objects.find(child->uniqueID);
				if (it2 != sector->Objects.end()) {
					sector->Objects.erase(it2);
				}
			}
			checkIt->second.clear();

			std::vector<std::pair<int, int>> sectors = getSectors(child);

			for (auto& [x, y] : sectors) {
				addObjToSector(child, x, y);
			}
		}
	}
public:
	void UpdateSectors(Instance* child) {
		toUpdateSectors.insert({ child->uniqueID, child});
	}

	void UpdateObjectTickState(Instance* child) {
		auto it = isTick.find(child->uniqueID);
		bool hasTick = child->hasEvent(TICK);

		if (hasTick and it == isTick.end()) {
			isTick.insert({child->uniqueID, child});
		} else if (!hasTick and it != isTick.end()) {
			isTick.erase(it);
		}
	}

	SpecialVector2 CanvasSize = { 0,0 };
	SpecialVector2 CanvasPosition = { 0,0 };
	SpecialVector2 CanvasSizeOFFSET = { 0,0 };
	SpecialVector2 CanvasPositionOFFSET = { 0,0 };
	SpecialVector2 CanvasAbsoluteSize = {0,0};
	SpecialVector2 CanvasAbsolutePosition = {0,0};
	float ScrollSpeed = 0.25;
	float ScrollSpeedOFFSET = 0;
	bool CropDescendants = true;
	Color SliderColor = { 15,15,15,255 };
	float SliderTransparency = 0.5;
	unsigned int SliderSize = 5;
	char Direction = 'Y';
	bool ScrollEnabled = true;
	bool Animated = false;

	void Draw(bool force=false) {
		Object2D::Draw();

		bool pushed = false;
		if (CropDescendants) {
			PushClip({ (int)RealPos.x, (int)RealPos.y, (int)RealSize.x, (int)RealSize.y });
			pushed = true;
		}

		for (auto& [id, ptr] : toUpdateSectors) {
			secUpd(ptr);
		}

		toUpdateSectors.clear();

		for (ScrollSector* s : sectorsOnView) {
			for (auto& [id, ptr] : s->Objects) {
				ptr->Update();
			}
		}

		for (Instance* s : Tick) {
			s->Update();
		}

		if (force) {
			checkAndUpdateCurrentSectors(force);
		}

		if (pushed) PopClip();

		if (SliderTransparency != 1 and SliderSize != 0) {
			if (CanvasSize.y > 1 or CanvasSizeOFFSET.y > RealSize.y) {
				if (Direction == 'Y' or Direction == 'B') {
					float totalContentHeight = CanvasSizeOFFSET.y + CanvasSize.y * RealSize.y;
					if (totalContentHeight < RealSize.y) totalContentHeight = RealSize.y;

					float sliderHeight = RealSize.y * (RealSize.y / totalContentHeight);
					if (sliderHeight > RealSize.y) sliderHeight = RealSize.y;

					float maxScrollY = totalContentHeight - RealSize.y;
					float currentScrollY = (RealSize.y * CanvasPosition.y) + CanvasPositionOFFSET.y;

					float sliderY = RealPos.y;
					if (maxScrollY > 0) {
						sliderY += (RealSize.y - sliderHeight) * (currentScrollY / maxScrollY);
					}

					SpecialVector2 firstPoint = { RealPos.x + RealSize.x - SliderSize * 0.6f, sliderY };
					SpecialVector2 secondPoint = { firstPoint.x, sliderY + sliderHeight };
					DrawLineEx(firstPoint, secondPoint, SliderSize, { SliderColor.r, SliderColor.g, SliderColor.b, (unsigned char)(SliderColor.a * (1 - SliderTransparency)) });
				}
			}

			if (CanvasSize.x > 1 or CanvasSizeOFFSET.x > RealSize.x) {
				if (Direction == 'X' or Direction == 'B') {
					float totalContentWidth = CanvasSizeOFFSET.x + CanvasSize.x * RealSize.x;
					if (totalContentWidth < RealSize.x) totalContentWidth = RealSize.x;

					float sliderWidth = RealSize.x * (RealSize.x / totalContentWidth);
					if (sliderWidth > RealSize.x) sliderWidth = RealSize.x;

					float maxScrollX = totalContentWidth - RealSize.x;
					float currentScrollX = (RealSize.x * CanvasPosition.x) + CanvasPositionOFFSET.x;

					float sliderX = RealPos.x;
					if (maxScrollX > 0) {
						sliderX += (RealSize.x - sliderWidth) * (currentScrollX / maxScrollX);
					}

					SpecialVector2 firstPoint = { sliderX, RealPos.y + RealSize.y - SliderSize * 0.6f };
					SpecialVector2 secondPoint = { sliderX + sliderWidth, firstPoint.y };
					DrawLineEx(firstPoint, secondPoint, SliderSize, { SliderColor.r, SliderColor.g, SliderColor.b, (unsigned char)(SliderColor.a * (1 - SliderTransparency)) });
				}
			}
		}
	}

	void Update() override {
		if (lastUpdateFrame == framesSinceStart) return;
		lastUpdateFrame = framesSinceStart;

		if (!Visible) return;
		if (CanvasSize.x < 0) CanvasSize.x = 0; if (CanvasSize.y < 0) CanvasSize.y = 0;
		if (Direction != 'X' and Direction != 'Y' and Direction != 'B') {
			Direction = 'Y';
		}

		getRealObject2Dsize();
		getRealObject2Dposition();
		eventHandler();

		bool force = false;

		for (auto& [id, ptr] : childsAddedInFrame) {
			SectorsAddChild(ptr);
			force = true;
		}

		for (auto& [id, ptr] : childsRemovedInFrame) {
			SectorsRemoveChild(id);
			force = true;
		}

		SameUpdate();

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		float maxScrollX = std::max(0.0f, (float)(CanvasSize.x * RealSize.x + CanvasSizeOFFSET.x - RealSize.x));
		float maxScrollY = std::max(0.0f, (float)(CanvasSize.y * RealSize.y + CanvasSizeOFFSET.y - RealSize.y));

		CanvasPositionOFFSET.x = std::max(0.0f, (float)CanvasPositionOFFSET.x);
		CanvasPositionOFFSET.y = std::max(0.0f, (float)CanvasPositionOFFSET.y);
		CanvasPosition.x = std::max(0.0f, (float)CanvasPosition.x);
		CanvasPosition.y = std::max(0.0f, (float)CanvasPosition.y);

		float currentScrollX = (RealSize.x * CanvasPosition.x) + CanvasPositionOFFSET.x;
		if (currentScrollX > maxScrollX) {
			CanvasPosition.x = std::floor(maxScrollX / RealSize.x);
			CanvasPositionOFFSET.x = maxScrollX - (RealSize.x * CanvasPosition.x);
		}

		float currentScrollY = (RealSize.y * CanvasPosition.y) + CanvasPositionOFFSET.y;
		if (currentScrollY > maxScrollY) {
			CanvasPosition.y = std::floor(maxScrollY / RealSize.y);
			CanvasPositionOFFSET.y = maxScrollY - (RealSize.y * CanvasPosition.y);
		}

		bool entered = false;

		bool enterAllowed = (
			EnterEventCondition == SUI_EEC::EEC_DEFAULT ? this == higherObject :
			(EnterEventCondition == SUI_EEC::EEC_EVERY_ENTER ? true :
				EnterEventCondition == SUI_EEC::EEC_IF_DESCENDANT_HIGHER ? ((higherObject == this and higherObject != nullptr) or (higherObject and higherObject != this and higherObject->isDescendantOf(this))) : false)
			);

		if (Visible and ((higherObject == this and PreviousHigherObject != this) or enterAllowed)) {
			entered = true;
		}

		if (entered and ScrollEnabled) {
			float WheelMove = GetMouseWheelMove();
			if (WheelMove != 0) {
				bool isY = (Direction == 'Y' or (!IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B'));
				bool isX = (Direction == 'X' or (IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B'));

				if (isY) {
					float currentY = (RealSize.y * CanvasPosition.y) + CanvasPositionOFFSET.y;
					float totalStep = (RealSize.y * ScrollSpeed) + ScrollSpeedOFFSET;
					float newTotalY = currentY - (WheelMove * totalStep);

					newTotalY = std::clamp(newTotalY, 0.0f, maxScrollY);

					float newY1 = std::floor(newTotalY / RealSize.y);
					float newY = newTotalY - (RealSize.y * newY1);

					if (Animated) {
						Animate::Create(&CanvasPositionOFFSET.y, 0.125, newY);
						Animate::Create(&CanvasPosition.y, 0.125, newY1);
					} else {
						CanvasPositionOFFSET.y = newY;
						CanvasPosition.y = newY1;
					}
				} else if (isX) {
					float currentX = (RealSize.x * CanvasPosition.x) + CanvasPositionOFFSET.x;
					float totalStep = (RealSize.x * ScrollSpeed) + ScrollSpeedOFFSET;
					float newTotalX = currentX - (WheelMove * totalStep);

					newTotalX = std::clamp(newTotalX, 0.0f, maxScrollX);

					float newX1 = std::floor(newTotalX / RealSize.x);
					float newX = newTotalX - (RealSize.x * newX1);

					if (Animated) {
						Animate::Create(&CanvasPositionOFFSET.x, 0.125, newX);
						Animate::Create(&CanvasPosition.x, 0.125, newX1);
					} else {
						CanvasPositionOFFSET.x = newX;
						CanvasPosition.x = newX1;
					}
				}
			}
		}

		checkAndUpdateCurrentSectors();
		Draw(force);
	}

	ScrollFrame* Clone() const override {
		ScrollFrame* i = new ScrollFrame(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		return i;
	}
	
	~ScrollFrame() {
		for (auto& _ : Grid) {
			for (auto& [_, s] : _.second) {
				delete s;
			}
		}
	}

	ScrollFrame(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; EnterEventCondition = EEC_IF_DESCENDANT_HIGHER; Active = true;  };
	ScrollFrame(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; EnterEventCondition = EEC_IF_DESCENDANT_HIGHER; Active = true; }

	ScrollFrame() = delete;
};

inline void Object2D::updateAncestorWhichParentIsScroll() {
	Instance* scrollChild = getAncestorWhichParentIsScrollFrame(this);

	if (scrollChild) {
		static_cast<ScrollFrame*>(scrollChild->Parent)->UpdateSectors(scrollChild);
	}
}

inline SpecialVector2 getCanvasRealPos(Object2D* obj) {
	if (obj->Class == SCROLLFRAME) {
		ScrollFrame* scra = static_cast<ScrollFrame*>(obj);
		return
		{
			scra->CanvasPosition.x * scra->RealSize.x + scra->CanvasPositionOFFSET.x,
			scra->CanvasPosition.y * scra->RealSize.y + scra->CanvasPositionOFFSET.y
		};
	}
	return { 0,0 };
}

inline SpecialVector2 getScrollFrameRS(Instance* sc) {
	if (sc->Class == SCROLLFRAME) {
		ScrollFrame* scra = static_cast<ScrollFrame*>(sc);
		return scra->RealSize;
	}

	return { 0,0 };
}
inline SpecialVector2 getScrollFrameRP(Instance* sc) {
	if (sc->Class == SCROLLFRAME) {
		ScrollFrame* scra = static_cast<ScrollFrame*>(sc);
		return scra->RealPos;
	}

	return { 0,0 };
}
inline bool isScrollFrameCropping(Instance* sc) {
	if (sc->Class == SCROLLFRAME) {
		ScrollFrame* scra = static_cast<ScrollFrame*>(sc);
		return scra->CropDescendants;
	}

	return false;
}

class TextLabel : public Object2D {
	constexpr static float TextTextureUpdateAspect = 1.1;
	constexpr static const char* DefaultName = "TextLabel";
	constexpr static InstanceType DefaultClass = TEXTLABEL;

	std::string visibleText = "";
	int lastMaxVisible = -1;
	Vector3 textParams{};
	SpecialVector2 lastRealSize{};
	RenderTexture2D cachedText{};
	SpecialVector2 newSize{};
	SpecialVector2 lastNewSize{};
	Vector3 lastParams = Vector3{};
	std::vector<int> charOffsets;

	void updateCharOffsets() {
		charOffsets.clear();
		for (int i = 0; i < Text.size();) {
			charOffsets.push_back(i);
			unsigned char c = Text[i];
			if (c < 0x80) i += 1;
			else if ((c & 0xE0) == 0xC0) i += 2;
			else if ((c & 0xF0) == 0xE0) i += 3;
			else if ((c & 0xF8) == 0xF0) i += 4;
			else i += 1;
		}
		charOffsets.push_back(Text.size());
	}

	void updateTexture() {
		updateCharOffsets();

		if (MaxVisibleSymbols > 0 and MaxVisibleSymbols < charOffsets.size()) {
			if (MaxVisibleRight) {
				size_t idx = charOffsets[std::max(3, (int)(charOffsets.size() - MaxVisibleSymbols)) - 3];
				visibleText = "...";
				visibleText += Text.substr(idx);
			} else {
				size_t idx = charOffsets[std::max(3, MaxVisibleSymbols) - 3];
				visibleText = Text.substr(0, idx);
				visibleText += "...";
			}
		} else {
			visibleText = !Text;
		}

		textParams = getTextCFrame(visibleText.c_str(), getFont(!FontFace), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
		lastRealSize = RealSize;
		lastParams = textParams;
		lastMaxVisible = MaxVisibleSymbols;

		newSize = MeasureTextEx(getFont(!FontFace), visibleText.c_str(), textParams.z, Spacing);

		if (cachedText.id == 0 or lastNewSize.x < newSize.x or lastNewSize.y < newSize.y) {
			if (cachedText.id != 0) {
				UnloadRenderTexture(cachedText);
			}
			
			if (Text.size()) {
				cachedText = LoadRenderTexture(newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect);
				lastNewSize = SpecialVector2{ newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect };
			}
		}

		if (cachedText.id) {
			bool hadClip = !clipStack.empty();
			Clip current;
			if (hadClip) current = clipStack.back();

			if (hadClip) EndScissorMode();

			BeginTextureMode(cachedText);
			ClearBackground(BLANK);
			DrawTextEx(getFont(!FontFace), visibleText.c_str(), { 0,0 }, textParams.z, Spacing, { 255,255,255,255 });
			EndTextureMode();
			SetTextureWrap(cachedText.texture, TEXTURE_WRAP_CLAMP);

			if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
		}
	}
public:
	SUI_Text Text = "";
	SUI_Text FontFace = BASIC_FONT_NAME;
	float TextTransparency = 0.0f;
	TextAnchorEnum TextAnchor = TextAnchorEnum::CENTER;
	Color TextColor = { 0,0,0,255 };
	int TextSize = -1;
	int Spacing = defaultSpacing;
	int MaxVisibleSymbols = -1;
	bool MaxVisibleRight = false;

	const SUI_Text& GetText() const {
		return Text;
	}

	void SetText(const std::string& T) {
		Text = T;
	}

	const SUI_Text& GetFont() const {
		return FontFace;
	}

	void SetFont(const std::string& F) {
		FontFace = F;
	}

	void Draw() override {
		if (Visible) {
			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			ScrollFrame* ancestor = nullptr;
			Instance* c = findFirstAncestorOfClass(SCROLLFRAME);
			if (c) ancestor = static_cast<ScrollFrame*>(c);
			if (ancestor and ancestor->CropDescendants) {
				if (RealPos.x + RealSize.x + BorderThickness < ancestor->RealPos.x or
					RealPos.y + RealSize.y + BorderThickness < ancestor->RealPos.y or
					RealPos.x + BorderThickness > ancestor->RealPos.x + ancestor->RealSize.x or
					RealPos.y + BorderThickness > ancestor->RealPos.y + ancestor->RealSize.y) {
					return;
				}
			}

			Object2D::Draw();

			bool dirtyCondition = Text.isChanged() or FontFace.isChanged();
			Text.restate();
			FontFace.restate();

			if (lastParams.z != textParams.z or (cachedText.id == 0 and Text.size()) or dirtyCondition or lastMaxVisible != MaxVisibleSymbols) {
				updateTexture();
			} else {
				if (std::fabsf(lastRealSize.x - RealSize.x) >= TextTextureUpdateAspect or std::fabsf(lastRealSize.y - RealSize.y) >= TextTextureUpdateAspect) {
					lastRealSize = RealSize;
					newSize = MeasureTextEx(getFont(!FontFace), visibleText.c_str(), textParams.z, Spacing);
					textParams = getTextCFrame(visibleText.c_str(), getFont(!FontFace), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
				}
			}

			if (textParams.z > 1) {
				if (cachedText.id == 0) {
					updateTexture();
				}
				Rectangle sourceRec = { 0.0f, (float)(cachedText.texture.height - newSize.y), (float)newSize.x, -(float)newSize.y };
				Rectangle destRec = { RealPos.x + textParams.x, RealPos.y + textParams.y, (float)newSize.x, (float)newSize.y };
				SpecialVector2 origin = { 0, 0 };

				DrawTexturePro(cachedText.texture, sourceRec, destRec, origin, 0, { TextColor.r, TextColor.g, TextColor.b, (unsigned char)(TextColor.a * (1 - TextTransparency)) });
			}
		}
	}
	
	TextLabel* Clone() const override {
		TextLabel* i = new TextLabel(*this);
		i->Parent = nullptr;
		i->Children.clear();

		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		i->cachedText.id = 0;
		i->cachedText.texture.id = 0;
		i->updateTexture();

		return i;
	}

	TextLabel(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	TextLabel(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }
	~TextLabel() {
		if (cachedText.id != 0) {
			UnloadRenderTexture(cachedText);
		}
	}
	TextLabel() = delete;
};

struct KeyMapping {
	KeyboardKey key;
	const char* defaultEN;
	const char* shiftEN;
	const char* defaultRU;
	const char* shiftRU;
};

inline constexpr KeyMapping KeysMapping[49] = { // in future will be replaced by OS API input or file with more languages support
	{ KEY_ONE,   "1", "!", "1", "!" },
	{ KEY_TWO,   "2", "@", "2", "\"" },
	{ KEY_THREE, "3", "#", "3", "№" },
	{ KEY_FOUR,  "4", "$", "4", ";" },
	{ KEY_FIVE,  "5", "%", "5", ":" },
	{ KEY_SIX,   "6", "^", "6", "?" },
	{ KEY_SEVEN, "7", "&", "7", "?" },
	{ KEY_EIGHT, "8", "*", "8", "*" },
	{ KEY_NINE,  "9", "(", "9", "(" },
	{ KEY_ZERO,  "0", ")", "0", ")" },

	{ KEY_Q, "q", "Q", "й", "Й" },
	{ KEY_W, "w", "W", "ц", "Ц" },
	{ KEY_E, "e", "E", "у", "У" },
	{ KEY_R, "r", "R", "к", "К" },
	{ KEY_T, "t", "T", "е", "Е" },
	{ KEY_Y, "y", "Y", "н", "Н" },
	{ KEY_U, "u", "U", "г", "Г" },
	{ KEY_I, "i", "I", "ш", "Ш" },
	{ KEY_O, "o", "O", "щ", "Щ" },
	{ KEY_P, "p", "P", "з", "З" },

	{ KEY_A, "a", "A", "ф", "Ф" },
	{ KEY_S, "s", "S", "ы", "Ы" },
	{ KEY_D, "d", "D", "в", "В" },
	{ KEY_F, "f", "F", "а", "А" },
	{ KEY_G, "g", "G", "п", "П" },
	{ KEY_H, "h", "H", "р", "Р" },
	{ KEY_J, "j", "J", "о", "О" },
	{ KEY_K, "k", "K", "л", "Л" },
	{ KEY_L, "l", "L", "д", "Д" },
	{ KEY_SEMICOLON, ";", ":", "ж", "Ж" },
	{ KEY_APOSTROPHE, "'", "\"", "э", "Э" },

	{ KEY_Z, "z", "Z", "я", "Я" },
	{ KEY_X, "x", "X", "ч", "Ч" },
	{ KEY_C, "c", "C", "с", "С" },
	{ KEY_V, "v", "V", "м", "М" },
	{ KEY_B, "b", "B", "и", "И" },
	{ KEY_N, "n", "N", "т", "Т" },
	{ KEY_M, "m", "M", "ь", "Ь" },
	{ KEY_COMMA, ",", "<", "б", "Б" },
	{ KEY_PERIOD, ".", ">", "ю", "Ю" },
	{ KEY_SLASH, "/", "?", ".", "," },

	{ KEY_SPACE, " ", " ", " ", " " },
	{ KEY_MINUS, "-", "_", "-", "_" },
	{ KEY_EQUAL, "=", "+", "=", "+" },
	{ KEY_LEFT_BRACKET, "[", "{", "х", "Х" },
	{ KEY_RIGHT_BRACKET, "]", "}", "ъ", "Ъ" },
	{ KEY_BACKSLASH, "\\", "|", "\\", "/" },
	{ KEY_GRAVE, "`", "~", "ё", "Ё" },
	//{ KEY_ENTER, "\n", "\n", "\n", "\n"} // WIP
};

enum TextBoxType {
	TextResizing = 0,
	Viewported
};

class TextBox : public Object2D {
	constexpr static float TextTextureUpdateAspect = 1.3;
	constexpr static const char* DefaultName = "TextBox";
	constexpr static InstanceType DefaultClass = TEXTBOX;

	int CursorIndex = -1;
	float CursorCooldown = 0.5f;
	float CursorTime = 0.0f;
	bool CursorVisible = false;

	bool deleteText = false;

	void updateCharOffsets() {
		charOffsets.clear();
		for (int i = 0; i < Text.size();) {
			charOffsets.push_back(i);
			unsigned char c = Text[i];
			if (c < 0x80) i += 1;
			else if ((c & 0xE0) == 0xC0) i += 2;
			else if ((c & 0xF0) == 0xE0) i += 3;
			else if ((c & 0xF8) == 0xF0) i += 4;
			else i += 1;
		}
		charOffsets.push_back(Text.size());
	}

	std::vector<int> charOffsets;
	Vector3 textParams{};
	RenderTexture2D cachedText;
	TextBox* lastFocused = nullptr;
	SpecialVector2 newSize{};
	SpecialVector2 lastRealSize{};
	Vector3 lastParams = Vector3{};
	char lastHideText = '\0';
	SpecialVector2 lastNewSize{};
	TextBoxType lastType = TextResizing;
	int lastCursorIndex = -1;
	float viewportPosition = 0;

	void updateTextParams() {
		if (Type == Viewported) {
			textParams.y = 0;
			textParams.x = 0;
			textParams.z = RealSize.y;
		} else {
			if (Text != "") {
				textParams = getTextCFrame(Text.c_str(), getFont(!FontFace), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
			} else {
				textParams = getTextCFrame(PlaceholderText.c_str(), getFont(!FontFace), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
			}
		}
	}
	void updateTexture() {
		updateTextParams();
		lastFocused = FocusedTextBox;
		lastParams = textParams;
		lastRealSize = RealSize;
		lastHideText = HideText;
		lastType = Type;

		if (Text != "") {
			newSize = MeasureTextEx(getFont(!FontFace), Text.c_str(), textParams.z, Spacing);
		} else {
			if (CursorIndex == -1 or FocusedTextBox != this) {
				newSize = MeasureTextEx(getFont(!FontFace), PlaceholderText.c_str(), textParams.z, Spacing);
			}
		}

		if (lastNewSize.x < newSize.x or lastNewSize.y < newSize.y) {
			if (cachedText.id != 0) {
				UnloadRenderTexture(cachedText);
			}

			cachedText = LoadRenderTexture(newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect);
			lastNewSize = SpecialVector2{ newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect };
		}

		bool hadClip = !clipStack.empty();
		Clip current;
		if (hadClip) current = clipStack.back();

		if (hadClip) EndScissorMode();

		BeginTextureMode(cachedText);
		ClearBackground(BLANK);

		if (Text != "") {
			std::string t;
			if (HideText == '\0') {
				t = !Text;
			} else {
				for (int i = 0; i < Text.size(); i++) {
					t += HideText;
				}
			}

			DrawTextEx(getFont(!FontFace), t.c_str(), { 0,0 }, textParams.z, Spacing, { 255,255,255,255 });
		} else {
			if (CursorIndex == -1 or FocusedTextBox != this) {
				DrawTextEx(getFont(!FontFace), PlaceholderText.c_str(), {0,0}, textParams.z, Spacing, {255,255,255,255});
			}
		}

		EndTextureMode();
		SetTextureWrap(cachedText.texture, TEXTURE_WRAP_CLAMP);
		if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
	}
public:
	Color CursorColor = { 0,0,0,255 };
	SUI_Text Text = "";
	SUI_Text FontFace = BASIC_FONT_NAME;
	SUI_Text PlaceholderText = "PlaceholderText";
	Color PlaceholderTextColor = { 150, 150, 150, 255 };
	Color TextColor = { 0,0,0,255 };
	TextAnchorEnum TextAnchor = TextAnchorEnum::CENTER;
	int TextSize = -1;
	int maxSymbols = 20;
	float TextTransparency = 0;
	std::string AllowedSymbols = "";
	int Spacing = defaultSpacing;
	char HideText = '\0';
	bool ClearOnClick = true;
	int CursorSize = 3;
	TextBoxType Type = TextResizing;

	void Draw() override {
		if (!Visible) return;
		if (RealPos.x + RealSize.x + BorderThickness < 0
			or RealPos.x - RealSize.x - BorderThickness > winWidth
			or RealPos.y + RealSize.y + BorderThickness < 0
			or RealPos.y - RealSize.y - BorderThickness > winHeight) {
			return;
		}

		ScrollFrame* ancestor = nullptr; Instance* c = findFirstAncestorOfClass(SCROLLFRAME); if (c) ancestor = static_cast<ScrollFrame*>(c);
		if (ancestor and ancestor->CropDescendants) {
			if (RealPos.x + RealSize.x + BorderThickness < ancestor->RealPos.x or
				RealPos.y + RealSize.y + BorderThickness < ancestor->RealPos.y or
				RealPos.x + BorderThickness > ancestor->RealPos.x + ancestor->RealSize.x or
				RealPos.y + BorderThickness > ancestor->RealPos.y + ancestor->RealSize.y) {
				return;
			}
		}

		Object2D::Draw();

		bool updateCondition1 = PlaceholderText.isChanged() or FontFace.isChanged();

		if (updateCondition1 or lastType != Type or cachedText.id == 0 or lastHideText != HideText or lastParams.x != textParams.x or lastParams.y != textParams.y or lastParams.z != textParams.z or ((FocusedTextBox == this and lastFocused != this) or (lastFocused == this and FocusedTextBox != this))) {
			updateTexture();
		} else if (Text.isChanged()) {
			updateTexture();
		} else {
			if (lastRealSize.x != RealSize.x or lastRealSize.y != RealSize.y) {
				updateTextParams();
				if (Text == "") {
					newSize = MeasureTextEx(getFont(!FontFace), PlaceholderText.c_str(), textParams.z, Spacing);
				}
				else {
					newSize = MeasureTextEx(getFont(!FontFace), Text.c_str(), textParams.z, Spacing);
				}
			}
		}

		if (textParams.z > 1) {
			if (cachedText.id == 0) {
				updateTexture();
			}

			SpecialVector2 sizeToDraw = (Type == Viewported) ? RealSize : newSize;

			Rectangle sourceRec = { (Type == Viewported) ? viewportPosition : 0.0f, (cachedText.texture.height - sizeToDraw.y), sizeToDraw.x, -sizeToDraw.y };
			Rectangle destRec = { RealPos.x + textParams.x, RealPos.y + textParams.y, sizeToDraw.x, sizeToDraw.y };
			SpecialVector2 origin = { 0, 0 };

			Color clr;
			if (Text == "") {
				clr = { PlaceholderTextColor.r, PlaceholderTextColor.g, PlaceholderTextColor.b, (unsigned char)(PlaceholderTextColor.a * (1 - TextTransparency)) };
			}
			else {
				clr = { TextColor.r, TextColor.g, TextColor.b, (unsigned char)(TextColor.a * (1 - TextTransparency)) };
			}

			DrawTexturePro(cachedText.texture, sourceRec, destRec, origin, 0, clr);
		}

		if (Text == "") {
			if (CursorVisible and FocusedTextBox == this) {
				if (textParams.z > 3) {
					float sizeY = MeasureTextEx(getFont(!FontFace), " ", textParams.z, Spacing).y;
					DrawLineEx({ RealPos.x + getTextOffset(TextAnchor).x * RealSize.x - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + 2 }, { RealPos.x + getTextOffset(TextAnchor).x * RealSize.x - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + sizeY - 4 }, CursorSize, CursorColor);
				}
			}
		}

		if (CursorIndex >= 0 and CursorVisible and Text != "" and textParams.z > 3) {
			int bytePos = (CursorIndex < (int)charOffsets.size()) ? charOffsets[CursorIndex] : Text.size();
			std::string textBeforeCursor = Text.substr(0, bytePos);
			if (HideText != '\0') {
				textBeforeCursor = "";
				for (int i = 0; i < bytePos; i++) {
					textBeforeCursor += HideText;
				}
			}

			SpecialVector2 size = MeasureTextEx(getFont(!FontFace), textBeforeCursor.c_str(), textParams.z, Spacing);

			if (size.x == 0 and size.y == 0) {
				size.y = MeasureTextEx(getFont(!FontFace), "a", textParams.z, Spacing).y;
			}

			DrawLineEx({ RealPos.x + textParams.x + size.x + 2 - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + 2 }, { RealPos.x + textParams.x + size.x + 2 - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + size.y - 4 }, CursorSize, CursorColor);
		}
	}

	void inputHandler() {
		// CURSOR

		if (FocusedTextBox == this and deleteText and ClearOnClick) {
			Text = "";
			CursorIndex = 0;
			deleteText = false;
			updateCharOffsets();
		}

		CursorTime += dt;
		if (CursorTime >= CursorCooldown) { CursorVisible = !CursorVisible; CursorTime = 0.0f; }

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (pointInObject(mousePosition) and FocusedTextBox != this and higherObject == this and ClearOnClick) {
				Text = "";
			}
			if (higherObject != this and higherObject) {
				if (higherObject->Class == TEXTBOX) {
					FocusedTextBox = static_cast<TextBox*>(higherObject);
				}
				else {
					FocusedTextBox = nullptr;
				}
			}
			else if (not higherObject) {
				FocusedTextBox = nullptr;
			}
			else if (pointInObject(mousePosition) and higherObject == this) {
				CursorTime = 0.0f;
				CursorVisible = true;
				FocusedTextBox = this;

				std::string text = "";
				if (HideText != '\0') {
					for (int i = 0; i < Text.size(); i++) {
						text += HideText;
					}
				}
				else {
					text = !Text;
				}

				updateTextParams();
				std::string textBeforeCursor = "";
				if (HideText != '\0') {
					for (int i = 0; i < Text.size(); i++) {
						textBeforeCursor += HideText;
					}
				}
				else {
					textBeforeCursor = !Text;
				}

				float textStartX = RealPos.x + textParams.x;
				float clickX = mousePosition.x - textStartX + ((Type == Viewported) ? viewportPosition : 0.0f);

				CursorIndex = 0;
				if (!text.empty()) {
					for (int i = 1; i < charOffsets.size(); i++) {
						float widthPrev = MeasureTextEx(getFont(!FontFace), text.substr(0, charOffsets[i - 1]).c_str(), textParams.z, Spacing).x;
						float widthCurr = MeasureTextEx(getFont(!FontFace), text.substr(0, charOffsets[i]).c_str(), textParams.z, Spacing).x;
						if (clickX < (widthPrev + widthCurr) / 2.0f) {
							CursorIndex = i - 1;
							break;
						}
						CursorIndex = i;
					}
				}
				else {
					CursorIndex = 0;
				}
			}
		}

		// KEYBOARD INPUT

		if (FocusedTextBox == this and Visible) {
			if (maxSymbols >= charOffsets.size()) {
				const char* layout = getLayout();
				int index = (!strcmp(layout, "RU")) ? 2 : 0;
				if (IsKeyDown(KEY_LEFT_SHIFT) or capsLock()) index += 1;
				for (auto& key : KeysMapping) {
					if (maxSymbols < charOffsets.size()) break;
					if (IsKeyPressed(key.key)) {
						const char* keyValue = nullptr;
						switch (index) {
						case 0: keyValue = key.defaultEN; break;
						case 1: keyValue = key.shiftEN; break;
						case 2: keyValue = key.defaultRU; break;
						case 3: keyValue = key.shiftRU; break;
						default: keyValue = key.defaultEN;
						}

						if (AllowedSymbols != "") {
							bool allowed = false;
							for (char c : AllowedSymbols) {
								if (c == *keyValue) {
									allowed = true;
									break;
								}
							}
							if (!allowed) continue;
						}
						updateCharOffsets();
						int bytePos = (CursorIndex < (int)charOffsets.size()) ? charOffsets[CursorIndex] : Text.size();
						Text = Text.substr(0, bytePos) + std::string(keyValue) + Text.substr(bytePos);
						CursorIndex += 1;
						updateCharOffsets();
						CursorVisible = true; CursorTime = 0.0f;
					}
				}
			}
		}

		// UTILS (BACKSPACE | DEL | CTRL BACKSPACE | ARROWS)
		if (FocusedTextBox == this and Visible) {
			if (IsKeyPressed(KEY_BACKSPACE)) {
				if (IsKeyDown(KEY_LEFT_CONTROL)) {
					if (CursorIndex > 0) {
						int start = CursorIndex;
						while (start > 0) {
							unsigned char c = Text[charOffsets[start - 1]];

							if (c != ' ')
								break;

							start--;
						}

						if (start > 0) {
							unsigned char c = Text[charOffsets[start - 1]];

							if (c == '.' or c == ',' or c == ':' or
								c == ';' or c == '?' or c == '!' or
								c == '/' or c == '\\' or c == '\'' or
								c == '\"') {
								start--;
							}
							else {
								while (start > 0) {
									c = Text[charOffsets[start - 1]];

									if (c == ' ' or c == '.' or c == ',' or
										c == ':' or c == ';' or c == '?' or
										c == '!' or c == '/' or c == '\\' or
										c == '\'' or c == '\"')
									{
										break;
									}

									start--;
								}
							}
						}

						Text = Text.substr(0, charOffsets[start]) + Text.substr(charOffsets[CursorIndex]);
						CursorIndex = start;

						updateCharOffsets();
					}
				}
				else {
					if (CursorIndex > 0) {
						Text = Text.substr(0, charOffsets[CursorIndex - 1]) + Text.substr(charOffsets[CursorIndex]);
						CursorIndex--;

						updateCharOffsets();
					}
				}

				CursorVisible = true;
				CursorTime = 0.0f;
			}

			if (IsKeyPressed(KEY_DELETE)) {
				if (CursorIndex < (int)charOffsets.size() - 1) {
					Text = Text.substr(0, charOffsets[CursorIndex]) + Text.substr(charOffsets[CursorIndex + 1]);

					updateCharOffsets();
				}

				CursorVisible = true;
				CursorTime = 0.0f;
			}

			if (IsKeyPressed(KEY_LEFT)) {
				if (IsKeyDown(KEY_LEFT_CONTROL)) {
					while (CursorIndex > 0) {
						unsigned char c = Text[charOffsets[CursorIndex - 1]];

						if (c != ' ')
							break;

						CursorIndex--;
					}

					while (CursorIndex > 0) {
						unsigned char c = Text[charOffsets[CursorIndex - 1]];

						if (c == ' ')
							break;

						CursorIndex--;
					}
				}
				else {
					CursorIndex--;
				}

				if (CursorIndex < 0)
					CursorIndex = 0;

				CursorVisible = true;
				CursorTime = 0.0f;
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				int maxIndex = (int)charOffsets.size() - 1;

				if (IsKeyDown(KEY_LEFT_CONTROL)) {
					while (CursorIndex < maxIndex) {
						unsigned char c = Text[charOffsets[CursorIndex]];

						if (c == ' ')
							break;

						CursorIndex++;
					}

					while (CursorIndex < maxIndex) {
						unsigned char c = Text[charOffsets[CursorIndex]];

						if (c != ' ')
							break;

						CursorIndex++;
					}
				}
				else {
					CursorIndex++;
				}

				if (CursorIndex > maxIndex)
					CursorIndex = maxIndex;

				CursorVisible = true;
				CursorTime = 0.0f;
			}
		}
	}

	void Update() override {
		if (lastUpdateFrame == framesSinceStart) return;
		lastUpdateFrame = framesSinceStart;

		if (!Visible) { CursorIndex = -1; CursorVisible = false; Text = ""; return; }
		if (!(FocusedTextBox == this)) { CursorIndex = -1; CursorVisible = false; deleteText = true; }

		getRealObject2Dsize();
		getRealObject2Dposition();

		inputHandler();

		eventHandler();

		SameUpdate();

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		if (Type == Viewported) {
			if (lastCursorIndex != CursorIndex) {
				lastCursorIndex = CursorIndex;

				if (Text.empty() or CursorIndex == -1) {
					viewportPosition = 0.0f;
				}
				else {
					std::string textBeforeCursor = Text.substr(0, charOffsets[CursorIndex]);
					SpecialVector2 textSize = MeasureTextEx(getFont(!FontFace), textBeforeCursor.c_str(), textParams.z, Spacing);

					float currentX = textSize.x;

					if (currentX - viewportPosition >= RealSize.x) {
						viewportPosition = currentX - RealSize.x;
					}
					else if (currentX < viewportPosition) {
						viewportPosition = currentX;
					}

					if (viewportPosition < 0) viewportPosition = 0;
					if (viewportPosition > newSize.x) viewportPosition = newSize.x - RealSize.x;
				}
			}
		}

		Draw();

		Text.restate();
		FontFace.restate();
		PlaceholderText.restate();

		for (int i = 0; i < Children.size(); i++) {
			Instance* child = Children[i];
			child->Update();
		}
	};

	~TextBox() {
		if (cachedText.id != 0) {
			UnloadRenderTexture(cachedText);
		}
	}

	size_t size() {
		updateCharOffsets();
		return charOffsets.size();
	}

	void SetText(const std::string& t) {
		std::vector<int> offsets;
		for (int i = 0; i < t.size();) {
			unsigned char c = t[i];
			offsets.push_back(i);
			if (c < 0x80) i += 1;
			else if ((c & 0xE0) == 0xC0) i += 2;
			else if ((c & 0xF0) == 0xE0) i += 3;
			else if ((c & 0xF8) == 0xF0) i += 4;
			else i += 1;
		}

		if (offsets.size() > maxSymbols) {
			Text = t.substr(0, offsets[maxSymbols]);
			updateCharOffsets();
			CursorIndex = maxSymbols;
			return;
		}

		Text = t;
		updateCharOffsets();
		CursorIndex = t.size();
	}

	const SUI_Text& GetText() const {
		return Text;
	}

	TextBox* Clone() const override {
		TextBox* i = new TextBox(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		i->cachedText.id = 0;
		i->cachedText.texture.id = 0;
		i->updateTexture();

		return i;
	}

	TextBox(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; Active = true; }
	TextBox(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; Active = true; }

	TextBox() = delete;
};

enum ImageOverlayFormat {
	STRETCH = 0, // STRETCH ON FULL SIZE
	FIT = 1, // FIT WITH ASPECT SAVING
	CROP = 2, // CUT EXCESS
};

class ImageLabel : public Object2D {
	constexpr static const char* DefaultName = "ImageLabel";
	constexpr static InstanceType DefaultClass = IMAGELABEL;

	Texture2D tex{};
	std::string currentPair;
	bool isMemoryLoadedTex = false;

	/* Previous version of ImageLabel textures system
	
	void updateTexture() {
		if ((tex.id == 0 or lastId != tex.id) and image.data) {
			if (tex.id != 0) {
				UnloadTexture(tex);
			}

			tex = LoadTextureFromImage(image);
			lastId = tex.id;

			GenTextureMipmaps(&tex);
			SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
			SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
		}
	}
	
	*/
public:
	ImageOverlayFormat Overlay = FIT;
	float ImageTransparency = 0.0f;
	Color ImageColor = { 255,255,255,255 };
	bool RoundImage = false;
	float Rotation = 0;
	SpecialVector2 Origin = { 0, 0 };

	void setImage(const std::string& name = "") {
		if (isMemoryLoadedTex) {
			isMemoryLoadedTex = false;
			UnloadTexture(tex);
		}

		auto pair = getImage(name);
		tex = pair.second;
		currentPair = name;
	}

	void Draw() override {
		if (!Visible) return;
		Object2D::Draw();

		if (RealPos.x + RealSize.x + BorderThickness < 0
			or RealPos.x - RealSize.x - BorderThickness > winWidth
			or RealPos.y + RealSize.y + BorderThickness < 0
			or RealPos.y - RealSize.y - BorderThickness > winHeight) {
			return;
		}

		if (tex.id) {
			Rectangle destRec = { RealPos.x + Origin.x, RealPos.y + Origin.y, RealSize.x, RealSize.y };
			Rectangle srcRec = { 0, 0, tex.width, tex.height };

			if (Overlay == FIT) {
				float imageAspect = (float)tex.width / tex.height;
				float rectAspect = RealSize.x / RealSize.y;

				if (imageAspect > rectAspect) {
					float scaledHeight = RealSize.x / imageAspect;
					destRec.y += (RealSize.y - scaledHeight) / 2.0f;
					destRec.height = scaledHeight;
				} else {
					float scaledWidth = RealSize.y * imageAspect;
					destRec.x += (RealSize.x - scaledWidth) / 2.0f;
					destRec.width = scaledWidth;
				}
			} else if (Overlay == CROP) {
				float imageAspect = (float)tex.width / tex.height;
				float rectAspect = RealSize.x / RealSize.y;

				if (imageAspect > rectAspect) {
					float cropWidth = tex.height * rectAspect;
					srcRec.x = (tex.width - cropWidth) / 2.0f;
					srcRec.width = cropWidth;
				} else {
					float cropHeight = tex.width / rectAspect;
					srcRec.y = (tex.height - cropHeight) / 2.0f;
					srcRec.height = cropHeight;
				}
			}

			if (Roundness and RoundImage) {
				static bool roundShaderLoaded = false;
				static Shader shader;
				static float lastRoundness = 0;
				static Rectangle lastObjectData = {0,0,0,0};
				static Rectangle lastImageData = {0,0,0,0};
				if (!roundShaderLoaded) {
					shader = getShader("TextureRoundness");
					roundShaderLoaded = true;
				}

				if (Roundness != lastRoundness) {
					lastRoundness = Roundness;
					SetShaderValue(shader, GetShaderLocation(shader, "roundness"), &Roundness, SHADER_UNIFORM_FLOAT);
				}

				if (destRec.width != lastObjectData.width or destRec.height != lastObjectData.height) {
					lastObjectData = destRec;
					SetShaderValue(shader, GetShaderLocation(shader, "objectData"), &destRec, SHADER_UNIFORM_VEC4);
				}
				if (srcRec.x != lastImageData.x or srcRec.y != lastImageData.y or
					srcRec.width != lastImageData.width or srcRec.height != lastImageData.height) {
					lastImageData = srcRec;
					SetShaderValue(shader, GetShaderLocation(shader, "imageData"), &srcRec, SHADER_UNIFORM_VEC4);
				}

				BeginShaderMode(shader);
				DrawTexturePro(tex, srcRec, destRec, Origin, Rotation, { ImageColor.r, ImageColor.g, ImageColor.b, (unsigned char)(ImageColor.a * (1 - ImageTransparency)) });
				EndShaderMode();
			} else {
				DrawTexturePro(tex, srcRec, destRec, Origin, Rotation, { ImageColor.r, ImageColor.g, ImageColor.b, (unsigned char)(ImageColor.a * (1 - ImageTransparency)) });
			}
		} else {
			setImage(currentPair);
		}
	}

	void UpdateFromMemory(const std::string& type, const std::vector<unsigned char>& data) {
		Image image = LoadImageFromMemory(type.c_str(), data.data(), data.size());

		if (!image.data) {
			std::cout << "UpdateFromMemory FAILED" << std::endl;
		}

		if (tex.id != 0 and isMemoryLoadedTex) UnloadTexture(tex);
	
		isMemoryLoadedTex = true;
		tex = LoadTextureFromImage(image);
		GenTextureMipmaps(&tex);
		SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
		SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
		currentPair = "";
	}

	ImageLabel* Clone() const override {
		ImageLabel* i = new ImageLabel(*this);
		i->setParent(nullptr);
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		if (isMemoryLoadedTex) {
			i->isMemoryLoadedTex = false;
			i->tex.id = 0;
		}

		return i;
	}

	ImageLabel(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	ImageLabel(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }

	ImageLabel() = delete;

	~ImageLabel() {
		if (tex.id != 0 and isMemoryLoadedTex) UnloadTexture(tex);
	}
};

class TextureLabel : public Object2D {
	constexpr static const char* DefaultName = "TextureLabel";
	constexpr static InstanceType DefaultClass = TEXTURELABEL;

	Image img{};
	bool imageLoadedWhileNotReady = false;
	Texture texture{};
	bool owner = false;

	void SetSize(int w, int h) {
		if (texture.id == 0 or texture.width != w or texture.height != h or !owner) {
			owner = true;
			if (texture.id != 0) UnloadTexture(texture);

			Image img = GenImageColor(w, h, BLANK);
			texture = LoadTextureFromImage(img);
			UnloadImage(img);
		}
	}
public:
	float Rotation = 0;
	Color TextureColor = { 255,255,255,255 };
	SpecialVector2 Origin = { 0, 0 };

	void Draw() override {
		if (Visible) {
			Object2D::Draw();

			if (imageLoadedWhileNotReady) {
				imageLoadedWhileNotReady = false;

				if (img.data != nullptr) {
					if (texture.id != 0 and owner) UnloadTexture(texture);

					owner = true;
					texture = LoadTextureFromImage(img);
					GenTextureMipmaps(&texture);
					SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
					UnloadImage(img);
				}
			}

			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			if (texture.id == 0) {
				return;
			}

			DrawTexturePro(texture, { 0,0,(float)texture.width,(float)texture.height }, { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, Origin, Rotation, TextureColor);
		}
	}

	void UpdateWithType(const std::string& type, std::vector<unsigned char>& data) {
		if (!IsWindowReady()) {
			if (imageLoadedWhileNotReady) {
				UnloadImage(img);
			}
		}

		img = LoadImageFromMemory(type.c_str(), data.data(), data.size());

		if (img.data == nullptr) return;
		if (texture.id != 0 and owner) UnloadTexture(texture);

		if (IsWindowReady()) {
			imageLoadedWhileNotReady = false;

			owner = true;
			texture = LoadTextureFromImage(img);
			GenTextureMipmaps(&texture);
			SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
			UnloadImage(img);
		}
		else {
			imageLoadedWhileNotReady = true;
		}
	}

	void UpdateData(std::vector<char>& data, int w, int h) {
		SetSize(w, h);
		UpdateTexture(texture, data.data());
		owner = true;
	}

	TextureLabel* Clone() const override {
		TextureLabel* i = new TextureLabel(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		i->owner = false;

		return i;
	}

	TextureLabel(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	TextureLabel(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }

	TextureLabel() = delete;

	~TextureLabel() {
		if (texture.id != 0 and owner) UnloadTexture(texture);
	}
};

inline void Object2D::PosOrSizeChanged() {
	sceneDirty = true;
	Instance* scrollChild = getAncestorWhichParentIsScrollFrame(this);

	if (scrollChild) {
		static_cast<ScrollFrame*>(scrollChild->Parent)->UpdateSectors(scrollChild);
	}
}

inline void Object2D::AddEvent(EventType t, InstanceCallback f, MouseButtonType m) {
	events.push_back({ t, f, m });

	Instance* asc = findFirstAncestorOfClass(SCROLLFRAME);

	if (asc) {
		static_cast<ScrollFrame*>(asc)->UpdateObjectTickState(this);
	}
}

inline void Instance::AddEvent(EventType t, InstanceCallback f, MouseButtonType m = NONE) {
	events.push_back({ t, f });

	Instance* asc = findFirstAncestorOfClass(SCROLLFRAME);

	if (asc) {
		static_cast<ScrollFrame*>(asc)->UpdateObjectTickState(this);
	}
}

inline void Object2D::eventHandler() {
	bool mouseOnObject = pointInObject(mousePosition);
	bool hasStartHold1 = false;
	bool hasStartHold2 = false;
	bool hasStartHold3 = false;

	std::function<void(Instance*)> mouseReleased1;
	std::function<void(Instance*)> mouseReleased2;
	std::function<void(Instance*)> mouseReleased3;

	for (const auto& [type, func, mouse] : events) {
		switch (type) {
			case TICK: {
				func(this);
				break;
			} case MOUSE_ENTER: {
				bool entered = false;

				if (mouseOnObject) {
					bool enterAllowed = (
						EnterEventCondition == SUI_EEC::EEC_DEFAULT ? this == higherObject : 
						(EnterEventCondition == SUI_EEC::EEC_EVERY_ENTER ? true :
							EnterEventCondition == SUI_EEC::EEC_IF_DESCENDANT_HIGHER ? ((higherObject == this and higherObject != nullptr) or (higherObject and higherObject != this and higherObject->isDescendantOf(this))) : false)
					);
					
					if (Visible and ((higherObject == this and PreviousHigherObject != this) or enterAllowed)) {
						entered = true;
					}
				}

				if (entered and !MouseEntered) {
					MouseEntered = true;
					func(this);
				}
				break;
			} case MOUSE_LEAVE: {
				bool enterAllowed = (
					EnterEventCondition == SUI_EEC::EEC_DEFAULT ? this == higherObject :
					(EnterEventCondition == SUI_EEC::EEC_EVERY_ENTER ? true :
					EnterEventCondition == SUI_EEC::EEC_IF_DESCENDANT_HIGHER ? (higherObject == this or (higherObject and higherObject != this and higherObject->isDescendantOf(this))) : false)
				);

				if (MouseEntered and (!Visible or !mouseOnObject or !enterAllowed)) {
					MouseEntered = false;
					func(this);
				}
				break;
			} case MOUSE_CLICK: {
				if (IsMouseButtonPressed(mouse) and mouseOnObject and higherObject == this) {
					func(this);
				}
				break;
			} case MOUSE_HOLD_START: {
				if (IsMouseButtonPressed(mouse) and mouseOnObject and higherObject == this) {
					if (mouse == LEFT) {
						startedOnObject1 = true;
					}
					else if (mouse == RIGHT) {
						startedOnObject2 = true;
					}
					else if (mouse == MIDDLE) {
						startedOnObject3 = true;
					}
					func(this);
				}

				if (mouse == LEFT) {
					hasStartHold1 = true;
				}
				else if (mouse == RIGHT) {
					hasStartHold2 = true;
				}
				else if (mouse == MIDDLE) {
					hasStartHold3 = true;
				}

				break;
			} case MOUSE_HOLD_END: {
				if (IsMouseButtonReleased(mouse)) {
					if (mouse == LEFT) {
						mouseReleased1 = func;
					} else if (mouse == RIGHT) {
						mouseReleased2 = func;
					} else if (mouse == MIDDLE) {
						mouseReleased3 = func;
					}
				}
				break;
			} case CHILD_ADDED: {
				for (auto& [id, ptr] : childsAddedInFrame) {
					if (childsRemovedInFrame.contains(id)) continue;
					func(this, ptr);
				}
				break;
			} case CHILD_REMOVED: {
				for (auto& [id, ptr] : childsRemovedInFrame) {
					if (childsAddedInFrame.contains(id)) continue;
					func(this, ptr);
				}
				break;
			} case TEXT_CHANGED: {
				if (Class == TEXTLABEL) {
					if (static_cast<TextLabel*>(this)->Text.isChanged()) {
						func(this);
					}
				} else if (Class == TEXTBOX) {
					if (static_cast<TextBox*>(this)->Text.isChanged()) {
						func(this);
					}
				}
				break;
			}
		}
	}

	if (not hasStartHold1 and IsMouseButtonPressed(LEFT) and mouseOnObject and higherObject == this) {
		startedOnObject1 = true;
	}

	if (not hasStartHold2 and IsMouseButtonPressed(RIGHT) and mouseOnObject and higherObject == this) {
		startedOnObject2 = true;
	}

	if (not hasStartHold3 and IsMouseButtonPressed(MIDDLE) and mouseOnObject and higherObject == this) {
		startedOnObject3 = true;
	}

	if (mouseReleased1 and mouseOnObject and startedOnObject1) {
		mouseReleased1(this);
	}
	if (mouseReleased2 and mouseOnObject and startedOnObject2) {
		mouseReleased2(this);
	}
	if (mouseReleased3 and mouseOnObject and startedOnObject3) {
		mouseReleased3(this);
	}

	if (IsMouseButtonReleased(LEFT)) {
		startedOnObject1 = false;
	}
	if (IsMouseButtonReleased(RIGHT)) {
		startedOnObject2 = false;
	}
	if (IsMouseButtonReleased(MIDDLE)) {
		startedOnObject3 = false;
	}
}

inline std::vector<unsigned char> PngBytesToJpgBytes(const std::string& path, int quality = 60) {
    Image img = LoadImage(path.c_str());

    if (img.data == nullptr) {
        return {};
    }

    if (img.width > 1920) {
        int targetWidth = 1920;
        int targetHeight = (img.height * 1920) / img.width;
        ImageResize(&img, targetWidth, targetHeight);
    }

    Image background = GenImageColor(img.width, img.height, WHITE);

    ImageDraw(&background, img, 
              Rectangle{ 0, 0, (float)img.width, (float)img.height }, 
              Rectangle{ 0, 0, (float)img.width, (float)img.height }, 
              WHITE);

    ImageFormat(&background, PIXELFORMAT_UNCOMPRESSED_R8G8B8);

    int channels = 3; 
    if (background.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {
        channels = 4;
    } else if (background.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8) {
        channels = 3;
    } else {
        ImageFormat(&background, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        channels = 4;
    }

    std::vector<unsigned char> outBytes;

    stbi_write_jpg_to_func(
        [](void* context, void* data, int size) {
            auto* vec = static_cast<std::vector<unsigned char>*>(context);
            auto* bytes = static_cast<unsigned char*>(data);
            vec->insert(vec->end(), bytes, bytes + size);
        },
        &outBytes,
        background.width,
        background.height,
        channels,
        background.data,
        quality
    );

    UnloadImage(img);
    UnloadImage(background);

    return outBytes;
}

inline void DrawFrame(Instance* StartInstance) {
	BeginDrawing();
	ClearBackground({ 255,255,255,255 });
	StartInstance->Update();
	EndDrawing();
}

inline void toggleFPS(Instance* s, Color textColor = { 0,0,0,255 }) {
	static TextLabel* labelFPS = nullptr;
	if (!labelFPS) {
		labelFPS = new TextLabel(s);
		labelFPS->BackgroundTransparency = 1;
		labelFPS->TextSize = -1;
		new ChangedSignal(accurateFPS, []() {
			static int last = 0;
			if (last != accurateFPS) {
				labelFPS->SetText(std::to_string(accurateFPS) + " FPS");
			}
		});
		labelFPS->Name = "FPS_LABEL";
		labelFPS->Active = false;
		labelFPS->Size = SpecialVector2{ 0.15, 0.1 };
		labelFPS->Position = SpecialVector2{ 0.85, 0 };
		labelFPS->TextAnchor = TextAnchorEnum::NE;
		labelFPS->ZIndex = 1000;
		labelFPS->TextColor = { 0,0,0,255 };
		labelFPS->Visible = false;
		labelFPS->TextColor = textColor;
	}

	labelFPS->Visible = !labelFPS->Visible;
}

inline namespace debug {
	int typeFPS[4]{
		60,
		144,
		-1,
		0
	};
	Color DefaultDebugColor = { 153, 204, 255, 255 };
	Color typeColor[9] = {
		DefaultDebugColor,
		{255,255,255,255},
		{255,102,102,255},
		{204,102,0,255},
		{255,255,204,255},
		{153,255,153,255},
		{51,0,102,255},
		{153,153,255,255},
		{0,51,102,255}
	};
	int currentColor = 0;

	ScrollFrame* console = nullptr;
	std::vector<std::string> textQueue;

	void print(std::string text) {
		if (!console) { textQueue.push_back(text); return; }
		TextLabel* sas = new TextLabel(nullptr);
		sas->SetText(text);
		sas->BackgroundTransparency = 1;
		sas->TextColor = typeColor[currentColor]; sas->TextSize = -1;
		sas->SetFont(BASIC_FONT_NAME);
		sas->TextAnchor = TextAnchorEnum::W;
		sas->setParent(console);
	}

	Object2D* debugMenu = nullptr;
	bool Animations = true;
	int currentFPSindex = 3;
	bool lowGraphicsMode = false; // SOON

	Object2D* treeFrame = nullptr;
	Instance* currentInstance = nullptr;

	void initDebug(Instance* s) {
		if (debugMenu) return;
		debugMenu = new Object2D(s);
		debugMenu->Size = SpecialVector2{ 1,1 };
		debugMenu->BackgroundTransparency = 0.9;
		debugMenu->BackgroundColor = DefaultDebugColor;
		debugMenu->Visible = false;
		debugMenu->ZIndex = 100000;
		debugMenu->Name = "debugMenu";

		TextLabel* lowerName = new TextLabel(debugMenu);
		lowerName->Name = "debugName";
		lowerName->SetText("(F2) Debug Menu");
		lowerName->TextSize = -1;
		lowerName->TextColor = DefaultDebugColor;
		lowerName->Position = SpecialVector2{ 0.03f, 0.9f };
		lowerName->Size = SpecialVector2{ 0.24, 0.1 };
		lowerName->TextAnchor = TextAnchorEnum::SE;
		lowerName->BackgroundTransparency = 1;
		lowerName->SetFont(DEBUG_MENU_FONT_NAME);

		/************************
		*       Settings        *
		************************/

		Object2D* SettingsFrame = new Object2D(debugMenu);
		SettingsFrame->Size = SpecialVector2{ 0.4, 0.25 };
		SettingsFrame->Position = SpecialVector2{ 0.04, 0.03 };
		SettingsFrame->BackgroundTransparency = 0.2;
		SettingsFrame->BorderColor = DefaultDebugColor;
		SettingsFrame->BorderThickness = 3;
		SettingsFrame->Name = "SettingsFrame";

		TextLabel* SettingsName = new TextLabel(SettingsFrame);
		SettingsName->Name = "SettingsName";
		SettingsName->SetText("Settings");
		SettingsName->TextSize = -1;
		SettingsName->TextColor = DefaultDebugColor;
		SettingsName->Position = SpecialVector2{ 0.5, 0 };
		SettingsName->AnchorPosition = SpecialVector2{ 0.5, 0 };
		SettingsName->Size = SpecialVector2{ 0.8, 0.1 };
		SettingsName->TextAnchor = TextAnchorEnum::CENTER;
		SettingsName->BackgroundTransparency = 1;
		SettingsName->SetFont(DEBUG_MENU_FONT_NAME);

		TextLabel* AnimLabel = new TextLabel(SettingsFrame);
		AnimLabel->Size = SpecialVector2{ 0.7, 0.2 };
		AnimLabel->BackgroundTransparency = 1;
		AnimLabel->Position = SpecialVector2{ 0, 0.1 };
		AnimLabel->SetText(" Animations");
		AnimLabel->TextAnchor = TextAnchorEnum::W;
		AnimLabel->TextSize = -1;
		AnimLabel->TextColor = DefaultDebugColor;
		AnimLabel->SetFont(DEBUG_MENU_FONT_NAME);
		AnimLabel->Name = "animLabel";

		TextLabel* AnimButton = new TextLabel(SettingsFrame);
		AnimButton->Size = SpecialVector2{ 0.19, 0.15 };
		AnimButton->BackgroundColor = Animations ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 };
		AnimButton->Position = SpecialVector2{ 0.8, 0.125 };
		AnimButton->SetText(Animations ? " On " : " Off ");
		AnimButton->TextAnchor = TextAnchorEnum::W;
		AnimButton->TextSize = -1;
		AnimButton->TextColor = { 0,0,0,255 };
		AnimButton->SetFont(DEBUG_MENU_FONT_NAME);
		AnimButton->Name = "animButton";
		AnimButton->Active = true;
		AnimButton->AddEvent(MOUSE_CLICK, [](Instance* t) {Animations = !Animations; }, LEFT);
		AnimButton->Roundness = 0.3;

		TextLabel* LGMlabel = new TextLabel(SettingsFrame);
		LGMlabel->Size = SpecialVector2{ 0.7, 0.2 };
		LGMlabel->BackgroundTransparency = 1;
		LGMlabel->Position = SpecialVector2{ 0, 0.3 };
		LGMlabel->SetText(" Low Graphics Mode");
		LGMlabel->TextAnchor = TextAnchorEnum::W;
		LGMlabel->TextSize = -1;
		LGMlabel->TextColor = DefaultDebugColor;
		LGMlabel->SetFont(DEBUG_MENU_FONT_NAME);
		LGMlabel->Name = "LGMlabel";

		TextLabel* LGMbutton = new TextLabel(SettingsFrame);
		LGMbutton->Size = SpecialVector2{ 0.19, 0.15 };
		LGMbutton->BackgroundColor = lowGraphicsMode ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 };
		LGMbutton->Position = SpecialVector2{ 0.8, 0.325 };
		LGMbutton->SetText(lowGraphicsMode ? " On " : " Off ");
		LGMbutton->TextAnchor = TextAnchorEnum::W;
		LGMbutton->TextSize = -1;
		LGMbutton->TextColor = { 0,0,0,255 };
		LGMbutton->SetFont(DEBUG_MENU_FONT_NAME);
		LGMbutton->Name = "LGMbutton";
		LGMbutton->Active = true;
		LGMbutton->AddEvent(MOUSE_CLICK, [](Instance* t) {lowGraphicsMode = !lowGraphicsMode; }, LEFT);
		LGMbutton->Roundness = 0.3;

		TextLabel* FPSlabel = new TextLabel(SettingsFrame);
		FPSlabel->Size = SpecialVector2{ 0.65, 0.2 };
		FPSlabel->BackgroundTransparency = 1;
		FPSlabel->Position = SpecialVector2{ 0, 0.5 };
		FPSlabel->SetText(" FPS mode");
		FPSlabel->TextAnchor = TextAnchorEnum::W;
		FPSlabel->TextSize = -1;
		FPSlabel->TextColor = DefaultDebugColor;
		FPSlabel->SetFont(DEBUG_MENU_FONT_NAME);
		FPSlabel->Name = "FPSlabel";

		Object2D* FPSframe = new TextLabel(SettingsFrame);
		FPSframe->Size = SpecialVector2{ 0.3, 0.2 };
		FPSframe->BackgroundTransparency = 1;
		FPSframe->Roundness = 0.3;
		FPSframe->Position = SpecialVector2{ 0.7, 0.5 };
		FPSframe->Name = "FPSlabel";
		TextLabel* FPSleft = new TextLabel(FPSframe);
		FPSleft->Size = SpecialVector2{ 0.25, 0.6 };
		FPSleft->BackgroundTransparency = 1;
		FPSleft->Position = SpecialVector2{ 0.0, 0.2 };
		FPSleft->SetText("<");
		FPSleft->TextAnchor = TextAnchorEnum::CENTER;
		FPSleft->TextSize = -1;
		FPSleft->TextColor = DefaultDebugColor;
		FPSleft->SetFont(DEBUG_MENU_FONT_NAME);
		FPSleft->Name = "FPSleft";
		FPSleft->Active = true;
		FPSleft->AddEvent(MOUSE_CLICK, [](Instance* t) { currentFPSindex--; currentFPSindex += 4; currentFPSindex = currentFPSindex % 4; }, LEFT);
		TextLabel* FPSquantity = new TextLabel(FPSframe);
		FPSquantity->Size = SpecialVector2{ 0.5, 1 };
		FPSquantity->BackgroundTransparency = 1;
		FPSquantity->Position = SpecialVector2{ 0.25, 0 };
		std::ostringstream st; st << " " << typeFPS[currentFPSindex] << " "; 
		FPSquantity->SetText(currentFPSindex == 2 ? "FULL" : ((currentFPSindex == 3) ? "V-SYNC" : st.str()));
		FPSquantity->TextSize = -1;
		FPSquantity->TextColor = DefaultDebugColor;
		FPSquantity->SetFont(DEBUG_MENU_FONT_NAME);
		FPSquantity->Name = "FPSquantity";
		TextLabel* FPSright = new TextLabel(FPSframe);
		FPSright->Size = SpecialVector2{ 0.25, 0.6 };
		FPSright->BackgroundTransparency = 1;
		FPSright->Position = SpecialVector2{ 0.75, 0.2 };
		FPSright->SetText(">");
		FPSright->TextAnchor = TextAnchorEnum::CENTER;
		FPSright->TextSize = -1;
		FPSright->TextColor = DefaultDebugColor;
		FPSright->SetFont(DEBUG_MENU_FONT_NAME);
		FPSright->Name = "FPSright";
		FPSright->Active = true;
		FPSright->AddEvent(MOUSE_CLICK, [](Instance* t) { currentFPSindex++; currentFPSindex += 4; currentFPSindex = currentFPSindex % 4; }, LEFT);

		TextLabel* Colorlabel = new TextLabel(SettingsFrame);
		Colorlabel->Size = SpecialVector2{ 0.65, 0.2 };
		Colorlabel->BackgroundTransparency = 1;
		Colorlabel->Position = SpecialVector2{ 0, 0.7 };
		Colorlabel->SetText(" Menu color");
		Colorlabel->TextAnchor = TextAnchorEnum::W;
		Colorlabel->TextSize = -1;
		Colorlabel->TextColor = DefaultDebugColor;
		Colorlabel->SetFont(DEBUG_MENU_FONT_NAME);
		Colorlabel->Name = "Colorlabel";

		Object2D* Colorframe = new TextLabel(SettingsFrame);
		Colorframe->Size = SpecialVector2{ 0.3, 0.2 };
		Colorframe->BackgroundTransparency = 1;
		Colorframe->Roundness = 0.3;
		Colorframe->Position = SpecialVector2{ 0.7, 0.7 };
		Colorframe->Name = "Colorframe";
		TextLabel* Colorleft = new TextLabel(Colorframe);
		Colorleft->Size = SpecialVector2{ 0.25, 0.6 };
		Colorleft->BackgroundTransparency = 1;
		Colorleft->Position = SpecialVector2{ 0.0, 0.2 };
		Colorleft->SetText("<");
		Colorleft->TextAnchor = TextAnchorEnum::CENTER;
		Colorleft->TextSize = -1;
		Colorleft->TextColor = DefaultDebugColor;
		Colorleft->SetFont(DEBUG_MENU_FONT_NAME);
		Colorleft->Name = "Colorleft";
		Colorleft->Active = true;
		Colorleft->AddEvent(MOUSE_CLICK, [](Instance* t) { currentColor--; currentColor += 9; currentColor = currentColor % 9; }, LEFT);
		Object2D* ColorBlock = new TextLabel(Colorframe);
		ColorBlock->Size = SpecialVector2{ 0.5, 0.8 };
		ColorBlock->BackgroundColor = DefaultDebugColor;
		ColorBlock->Position = SpecialVector2{ 0.25, 0.1 };
		ColorBlock->Roundness = 0.3;
		ColorBlock->Name = "ColorBlock";
		TextLabel* Colorright = new TextLabel(Colorframe);
		Colorright->Size = SpecialVector2{ 0.25, 0.6 };
		Colorright->BackgroundTransparency = 1;
		Colorright->Position = SpecialVector2{ 0.75, 0.2 };
		Colorright->SetText(">");
		Colorright->TextAnchor = TextAnchorEnum::CENTER;
		Colorright->TextSize = -1;
		Colorright->TextColor = DefaultDebugColor;
		Colorright->SetFont(DEBUG_MENU_FONT_NAME);
		Colorright->Name = "Colorright";
		Colorright->Active = true;
		Colorright->AddEvent(MOUSE_CLICK, [](Instance* t) { currentColor++; currentColor += 9; currentColor = currentColor % 9; }, LEFT);

		/******************
		*       logs      *
		******************/

		Object2D* LogsFrame = new Object2D(debugMenu);
		LogsFrame->Size = SpecialVector2{ 0.4, 0.6 };
		LogsFrame->Position = SpecialVector2{ 0.04, 0.3 };
		LogsFrame->BackgroundTransparency = 0.2;
		LogsFrame->BorderColor = DefaultDebugColor;
		LogsFrame->BorderThickness = 3;
		LogsFrame->Name = "LogsFrame";

		TextLabel* LogsName = new TextLabel(LogsFrame);
		LogsName->Name = "LogsName";
		LogsName->SetText("Logs");
		LogsName->TextSize = -1;
		LogsName->TextColor = DefaultDebugColor;
		LogsName->Position = SpecialVector2{ 0.5, 0 };
		LogsName->AnchorPosition = SpecialVector2{ 0.5, 0 };
		LogsName->Size = SpecialVector2{ 0.8, 0.055 };
		LogsName->TextAnchor = TextAnchorEnum::CENTER;
		LogsName->BackgroundTransparency = 1;
		LogsName->SetFont(DEBUG_MENU_FONT_NAME);

		console = new ScrollFrame(LogsFrame);
		console->BackgroundColor = { 0,0,0,255 };
		console->BackgroundTransparency = 0.1;
		console->BorderThickness = 3;
		console->BorderColor = DefaultDebugColor;
		console->Size = SpecialVector2{ 1, 0.93 };
		console->Position = SpecialVector2{ 0, 0.07 };
		console->SliderColor = { 255,255,255,255 };
		console->Name = "consoleLogs";
		console->AddEvent(CHILD_ADDED, [](Instance* child) {
			int n = console->Children.size();
			std::ostringstream s; s << n;
			TextLabel* c = static_cast<TextLabel*>(child);
			c->Name = s.str();
			c->Size = SpecialVector2{ 1, 0.05 };
			c->Position = SpecialVector2{ 0, 0.05f * (n - 1) };
			console->CanvasSize.y += 0.05 - (n > 20 ? 0 : 0.05);
			console->CanvasPosition.y = console->CanvasSize.y - 1;
		});
		console->Active = true;
		print("Debug inited");
		for (int i = 0; i < textQueue.size(); i++) {
			print(textQueue[i]);
		}
		textQueue.clear();

		/********************
		* Objects hierarchy *
		********************/

		treeFrame = new Object2D(debugMenu);
		treeFrame->Size = SpecialVector2{ 0.49, 0.87 };
		treeFrame->Position = SpecialVector2{ 0.47, 0.03 };
		treeFrame->BackgroundTransparency = 0.2;
		treeFrame->BorderColor = DefaultDebugColor;
		treeFrame->BorderThickness = 3;
		treeFrame->Name = "treeFrame";
		treeFrame->Active = true;
		TextLabel* treeName = new TextLabel(treeFrame);
		treeName->Name = "treeName";
		treeName->SetText("Objects hierarchy");
		treeName->TextSize = -1;
		treeName->TextColor = DefaultDebugColor;
		treeName->Position = SpecialVector2{ 0.5, 0 };
		treeName->AnchorPosition = SpecialVector2{ 0.5, 0 };
		treeName->Size = SpecialVector2{ 0.8, 0.055 };
		treeName->TextAnchor = TextAnchorEnum::CENTER;
		treeName->BackgroundTransparency = 1;
		treeName->SetFont(DEBUG_MENU_FONT_NAME);
		Object2D* manageMenu = new Object2D(treeFrame);
		manageMenu->Name = "manageMenu";
		manageMenu->Position = SpecialVector2{ 0, 0.06 };
		manageMenu->Size = SpecialVector2{ 1, 0.05 };
		manageMenu->BorderThickness = 3;
		manageMenu->BackgroundTransparency = 1;
		manageMenu->BorderColor = DefaultDebugColor;
		ScrollFrame* way = new ScrollFrame(manageMenu);
		way->Name = "directory";
		way->BackgroundTransparency = 1;
		way->Position = SpecialVector2{ 0, 0 };
		way->Size = SpecialVector2{ 1, 1 };
		way->Direction = 'X';
		way->SliderColor = { 255,255,255,255 };
		ScrollFrame* treeScroll = new ScrollFrame(treeFrame);
		treeScroll->Name = "treeScroll";
		treeScroll->Position = SpecialVector2{ 0, 0.12 };
		treeScroll->Size = SpecialVector2{ 0.5, 0.88 };
		treeScroll->BackgroundTransparency = 1;
		treeScroll->SliderColor = { 255,255,255,255 };
		treeScroll->ScrollSpeed = 0.2;

		/********************
		*  Events Handler   *
		********************/

		new ChangedSignal<int>(currentColor, [treeScroll, way, manageMenu, treeName, LogsName, LogsFrame, lowerName, Colorright, ColorBlock, Colorleft, Colorlabel, FPSright, FPSquantity, FPSleft, FPSlabel, LGMlabel, AnimLabel, SettingsName, SettingsFrame]() {
			Colorright->TextColor = typeColor[currentColor];
			ColorBlock->BackgroundColor = typeColor[currentColor];
			Colorleft->TextColor = typeColor[currentColor];
			Colorlabel->TextColor = typeColor[currentColor];
			FPSright->TextColor = typeColor[currentColor];
			FPSquantity->TextColor = typeColor[currentColor];
			FPSleft->TextColor = typeColor[currentColor];
			FPSlabel->TextColor = typeColor[currentColor];
			LGMlabel->TextColor = typeColor[currentColor];
			AnimLabel->TextColor = typeColor[currentColor];
			SettingsName->TextColor = typeColor[currentColor];
			SettingsFrame->BorderColor = typeColor[currentColor];
			lowerName->TextColor = typeColor[currentColor];
			debugMenu->BackgroundColor = typeColor[currentColor];
			console->BorderColor = typeColor[currentColor];
			LogsFrame->BorderColor = typeColor[currentColor];
			LogsName->TextColor = typeColor[currentColor];
			treeFrame->BorderColor = typeColor[currentColor];
			treeName->TextColor = typeColor[currentColor];
			manageMenu->BorderColor = typeColor[currentColor];
			for (Instance* obj : console->Children) {
				if (obj->Class == TEXTLABEL) {
					TextLabel* t = static_cast<TextLabel*>(obj);
					if (t) {
						t->TextColor = typeColor[currentColor];
					}
				}
			}
			for (Instance* obj : way->Children) {
				if (obj->Class == TEXTLABEL) {
					TextLabel* t = static_cast<TextLabel*>(obj);
					if (t) {
						t->TextColor = typeColor[currentColor];
					}
				}
			}
			for (Instance* obj : treeScroll->Children) {
				if (obj->Class == TEXTLABEL) {
					TextLabel* t = static_cast<TextLabel*>(obj);
					if (t) {
						t->TextColor = typeColor[currentColor];
					}
				}
			}
		});

		new ChangedSignal<int>(currentFPSindex, [FPSquantity]() { SetTargetFPS((typeFPS[currentFPSindex] == 0) ? GetMonitorRefreshRate(GetCurrentMonitor()) : typeFPS[currentFPSindex]); std::ostringstream s; s << " " << typeFPS[currentFPSindex] << " "; FPSquantity->SetText(currentFPSindex == 2 ? "FULL" : ((currentFPSindex == 3) ? "V-SYNC" : s.str())); });
		new ChangedSignal<bool>(Animations, [AnimButton]() { AnimButton->BackgroundColor = Animations ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 }; AnimButton->SetText(Animations ? " On " : " Off ");});
		new ChangedSignal<bool>(lowGraphicsMode, [LGMbutton]() { LGMbutton->BackgroundColor = lowGraphicsMode ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 }; LGMbutton->SetText(lowGraphicsMode ? " On " : " Off "); });

		new ChangedSignal<Instance*>(currentInstance, [way, treeScroll]() {
			way->deleteAllChildren();
			treeScroll->deleteAllChildren();

			if (currentInstance) {
				Instance* obj = currentInstance;
				static std::vector<Instance*> objects;
				objects.clear();
				while (obj != nullptr) {
					objects.push_back(obj);

					if (obj->__ParentObject) break;
					obj = obj->Parent;
				}

				for (int i = objects.size() - 1; i >= 0; i--) {
					TextLabel* element = new TextLabel(way);
					element->Name = objects[i]->Name;
					element->BackgroundTransparency = 1;
					element->TextColor = DefaultDebugColor;
					element->Position = SpecialVector2{ (objects.size() - i - 1) * 0.25f, 0 };
					element->Size = SpecialVector2{ 0.2, 0.9 };
					element->SetText(objects[i]->Name);
					element->Active = true;

					if (i != 0) {
						TextLabel* element2 = new TextLabel(way);
						element2->Name = ">";
						element2->BackgroundTransparency = 1;
						element2->TextColor = DefaultDebugColor;
						element2->Position = SpecialVector2{ (objects.size() - i - 1) * 0.25f + 0.2f , 0 };
						element2->Size = SpecialVector2{ 0.05, 0.9 };
						element2->SetText(">");
					}

					element->AddEvent(MOUSE_CLICK, [i](Instance* t) { currentInstance = objects[i]; }, LEFT);
				}

				way->CanvasSize.x = objects.size() * 0.25 - 0.05;
				way->CanvasPosition.x = way->CanvasSize.x;

				static std::vector<Instance*> objects2;
				objects2.clear();

				bool dec = false;

				for (int i = 0; i < currentInstance->Children.size(); i++) {
					if (currentInstance->Children[i]->Name == "debugMenu") { dec = true; continue; }
					objects2.push_back(currentInstance->Children[i]);
					TextLabel* element = new TextLabel(treeScroll);
					element->Name = currentInstance->Children[i]->Name;
					element->BackgroundTransparency = 1;
					element->TextColor = DefaultDebugColor;
					element->Position = SpecialVector2{ 0, (i - dec) * 0.05f };
					element->Size = SpecialVector2{ 1, 0.05 };
					std::ostringstream pupupupu; pupupupu << " > " << currentInstance->Children[i]->Name;
					element->SetText(pupupupu.str());
					element->Active = true;
					element->TextAnchor = TextAnchorEnum::W;
					element->AddEvent(MOUSE_CLICK, [i, dec](Instance* t) { currentInstance = objects2[i - dec]; }, LEFT);
				}

				treeScroll->CanvasSize.y = (currentInstance->Children.size() - dec) * 0.05;
				treeScroll->CanvasPosition.y = 0;
			}
			});
		currentInstance = s;
	}

	void toggleDebug(Instance* s) {
		if (!debugMenu) {
			initDebug(s);
		}

		debugMenu->Visible = !debugMenu->Visible;
	}
};

inline void updateSignals() {
	for (auto obj : ActiveSignals) {
		if (!obj) continue;
		obj->Update();
	}
}

void SUI_SetWindowSize(int newW, int newH) {
	changeWindowSize = SpecialVector2{ (float)newW, (float)newH };
	changeWindowSizeB = true;
}

void SUI_SetWindowPosition(int newX, int newY) {
	SetWindowPosition(newX, newY);
}

inline SpecialVector2 windowMinimalSize = { 0,0 };

void SUI_SetMinimalWindowSize(int newX, int newY) {
	windowMinimalSize = SpecialVector2{ (float)newX, (float)newY };
}

bool ALLOW_DEBUG = true;
bool ALLOW_FPS = true;

void UpdateHigher(Instance* StartInstance) {
	Object2D* best = nullptr;
	int maxDepth = -1;

	std::function<bool(Instance*, int)> getTop = [&getTop, &best, &maxDepth](Instance* parent, int localDepth) -> bool {
		bool foundInThisBranch = false;

		if (parent->Class == SCROLLFRAME) {
			ScrollFrame* scroll = static_cast<ScrollFrame*>(parent);

			for (auto sector : scroll->sectorsOnView) {
				for (auto& [id, child] : sector->Objects) {
					int nextDepth = localDepth;
					bool isTarget = false;

					if (scroll->childsRemovedInFrame.contains(id)) continue;

					if (!Is2DInheritor(child)) { // experemental branch. if not working then delete
						if (getTop(child, nextDepth)) {
							foundInThisBranch = true;
						}

						if (foundInThisBranch) {
							return true;
						}
					} else {
						auto obj = static_cast<Object2D*>(child);

						if (obj) {
							if (!obj->Visible) continue;
							nextDepth = localDepth + 1;
							if (obj->Active and obj->pointInObject(mousePosition)) {
								isTarget = true;
							}
						}

						if (getTop(child, nextDepth)) {
							foundInThisBranch = true;
						}

						if (isTarget) {
							if (nextDepth > maxDepth or (nextDepth == maxDepth and (!best or obj->ZIndex > best->ZIndex))) {
								best = obj;
								maxDepth = nextDepth;
								foundInThisBranch = true;
							}
						}

						if (foundInThisBranch) {
							return true;
						}
					}
				}
			}
		} else {
			for (auto it = parent->Children.rbegin(); it != parent->Children.rend(); it++) {
				Instance* child = *it;

				int nextDepth = localDepth;
				bool isTarget = false;

				if (!Is2DInheritor(child)) { // experemental branch. if not working then delete
					if (getTop(child, nextDepth)) {
						foundInThisBranch = true;
					}

					if (foundInThisBranch) {
						return true;
					}
				}

				auto obj = static_cast<Object2D*>(child);

				if (obj) {
					if (!obj->Visible) continue;
					nextDepth = localDepth + 1;
					if (obj->Active and obj->pointInObject(mousePosition)) {
						isTarget = true;
					}
				}

				if (getTop(child, nextDepth)) {
					foundInThisBranch = true;
				}

				if (isTarget) {
					if (nextDepth > maxDepth or (nextDepth == maxDepth and (!best or obj->ZIndex > best->ZIndex))) {
						best = obj;
						maxDepth = nextDepth;
						foundInThisBranch = true;
					}
				}

				if (foundInThisBranch) {
					return true;
				}
			}
		}



		return foundInThisBranch;
	};

	getTop(StartInstance, 0);
	PreviousHigherObject = higherObject;
	higherObject = best;
}

void start(Instance& StartInstance, Vector3 inf, const char* name, const char* iconName = "", unsigned int flags = 4) {
	SetConfigFlags(flags);
	SetTraceLogLevel(LOG_NONE);

	winWidth = inf.x;
	winHeight = inf.y;
	
	InitWindow(inf.x, inf.y, name);
	if (windowMinimalSize.x != 0 and windowMinimalSize.y != 0) {
		SetWindowMinSize(windowMinimalSize.x, windowMinimalSize.y);
	}
	int targetFPS = (inf.z <= 0) ? GetMonitorRefreshRate(GetCurrentMonitor()) : inf.z;
	SetTargetFPS(targetFPS);
	if (iconName != "") SetWindowIcon(LoadImage(iconName));

	SetExitKey(KEY_NULL);

	createFont(BASIC_FONT_NAME, "Fonts/arial.ttf", 100); // Basic font 1
	createFont(DEBUG_MENU_FONT_NAME, "Fonts/rogFont.otf", 50); // Basic font 2
	loadNewShader("TextureRoundness", "", "include/simpleUI Shaders/texture_roundness.frag"); // Basic shader

	for (auto& tup : queuedFonts) {
		createFont(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup));
	}
	queuedFonts.clear();

	for (auto& pair : pendingImages) {
		Texture tex = LoadTextureFromImage(pair.second);
		GenTextureMipmaps(&tex);
		SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
		SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
		loadedImages.insert({ pair.first, {pair.second, tex} });
	}

	pendingImages.clear();

	while (programRunning and !WindowShouldClose()) {
		if (IsWindowFullscreen()) ToggleFullscreen();
		if (changeWindowSizeB) {
			SetWindowSize(changeWindowSize.x, changeWindowSize.y);
			changeWindowSizeB = false;
		}

		static Vector2 previousMousePosition = {};
		mousePosition = GetMousePosition();
		mouseScreenPosition = GetMouseScreenPosition();
		windowPosition = GetWindowPosition();
		winWidth = GetScreenWidth(); winHeight = GetScreenHeight();

		static long middleFPS = 0;
		middleFPS += 1 / dt;
		static double cd = 0;
		cd += dt;
		static int frames = 0;
		frames++;

		if (cd >= 0.1) {
			cd = 0;
			accurateFPS = middleFPS / frames;
			middleFPS = 0;
			frames = 0;
		}

		updateSignals();
		dt = GetFrameTime();
		Animate::UpdateAnimations(dt);
		Tasks::UpdateTasks(dt);

		if (previousMousePosition.x != mousePosition.x or previousMousePosition.y != mousePosition.y or sceneDirty) {
			previousMousePosition = mousePosition;
			UpdateHigher(&StartInstance);
		}

		if (IsKeyPressed(KEY_F1) and ALLOW_FPS) { toggleFPS(&StartInstance, {125, 180, 220, 255}); }
		if (IsKeyPressed(KEY_F2) and ALLOW_DEBUG) { debug::toggleDebug(&StartInstance); }
		if (IsKeyPressed(KEY_F3)) { std::cout << accurateFPS << std::endl; }

		framesSinceStart += 1;

		DrawFrame(&StartInstance);
	}

	/*
	
	for (int i = 0; i < StartInstance.Children.size();) {
		Instance* child = StartInstance.Children[i];
		Delete(child);
	}

	for (auto it : Fonts) {
		UnloadFont(it.second);
	}

	Fonts.clear();

	*/

	CloseWindow();
}