#pragma once
#ifdef _WIN32
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#define _CRT_SECURE_NO_WARNINGS
#include "stb_image_write.h"
#include <raylib.h>
#include "apiUTILS.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <cstdint>

std::unordered_map<std::string, Shader> Shaders;

void loadNewShader(const std::string& name, const std::string& vs, const std::string& fs) {
	Shaders.emplace(name, LoadShader(vs.c_str(), fs.c_str()));
}

Shader getShader(const std::string& name) {
	auto it = Shaders.find(name);
	if (it == Shaders.end()) {
		exit(99);
	}
	return it->second;
}

Vector2 GetMouseScreenPosition() {
	return { GetMouseScreenPositionX(), GetMouseScreenPositionY() };
}

Color mulColor(Color other, float t) {
	return Color{
		(unsigned char)(other.r * t),
		(unsigned char)(other.g * t),
		(unsigned char)(other.b * t),
		other.a
	};
}

float sui_lerp(float a, float b, float t) {
	return a + (b-a) * t;
}

int winWidth = 0;
int winHeight = 0;

Vector2 changeWindowSize = { 0,0 };
bool changeWindowSizeB = false;

int accurateFPS = 0;

bool programRunning = true;

std::unordered_map<std::string, Font> Fonts;
std::vector<std::tuple<const char*, const char*, int>> queuedFonts;

void addFontToQueqe(const char* name, const char* path, int size) {
	queuedFonts.emplace_back(name, path, size);
}

void createFont(const char* name, const char* path, int size) {
	static int codepointsCount = 0;
	static int* codepoints = LoadCodepoints(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ЁАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯёабвгдежзийклмнопрстуфхцчшщъыьэюя", &codepointsCount);
	Font ft = LoadFontEx(path, size, codepoints, codepointsCount);
	GenTextureMipmaps(&ft.texture);
	SetTextureFilter(ft.texture, TEXTURE_FILTER_TRILINEAR);
	Fonts.emplace(name, ft);
}

int defaultSpacing = 0;
float dt = 0;

namespace Tasks {
	class Task;

	std::vector<Task*> ActiveTasks;

	class Task {
	public:
		float TimeLeft{};
		std::function<void(void)> Callback{};

		void Cancel() {
			auto obj = find(ActiveTasks.begin(), ActiveTasks.end(), this);
			if (obj != ActiveTasks.end()) {
				ActiveTasks.erase(obj);
			}
			delete this;
		}

		Task(float TimeInSeconds, std::function<void(void)> f) : TimeLeft(TimeInSeconds), Callback(f) { ActiveTasks.push_back(this); }
		~Task() {
			auto obj = find(ActiveTasks.begin(), ActiveTasks.end(), this);
			if (obj != ActiveTasks.end()) {
				ActiveTasks.erase(obj);
			}
		}
	};

	void UpdateTasks(float dt) {
		std::vector<Task*> z = ActiveTasks;

		for (int i = 0; z.size() > 0;) {
			z[i]->TimeLeft -= dt;
			if (z[i]->TimeLeft <= 0) {
				z[i]->Callback();
				delete z[i];
			}
			z.erase(z.begin() + i);
		}
	}
}

struct IChangedSignal {
    virtual ~IChangedSignal() = default;
    virtual void Update() = 0;
};

std::vector<IChangedSignal*> ActiveSignals;
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

namespace Animate {
	enum Function {
		Linear = 0,
		Quad,
		Cube,
		Exponential,
		Sine,
		Circular
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
			else { return 1 - (1 - t) * (1 - t); }
		}
		if (f == Cube) {
			if (e == In) { return t * t * t; }
			else { return 1 - (1 - t) * (1 - t) * (1 - t); }
		}
		if (f == Exponential) {
			if (e == In) { return (t == 0) ? t : powf(2, 10 * (t - 1)); }
			else { return (t == 1) ? 1 : (1 - powf(2, -10 * t)); }
		}
		if (f == Sine) {
			if (e == In) { return 1 - cos((PI * t) / 2); }
			else { return sin((PI * t) / 2); }
		}
		if (f == Circular) {
			if (e == In) { return 1 - sqrt(1 - t * t); }
			else { return sqrt(1 - (t - 1) * (t - 1)); }
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
		Vector2 startValueV{};
		Vector2 endValueV{};

		const char* type = "int";
	public:
		std::function<void(void)> Completed = []() {};

		bool Update(float dt) {
			currentTime += dt;
			if (currentTime >= endTime) {
				if (type == "int") { *(int*)ptr = endValueI; }
				else if (type == "float") { *(float*)ptr = endValueF; }
				else if (type == "color") { *(Color*)ptr = endValueC; }
				else if (type == "vector2") { *(Vector2*)ptr = endValueV; }
				return true;
			}
			if (type == "int") { *(int*)ptr = sui_lerp(startValueI, endValueI, getTime(func, ease, currentTime / endTime)); }
			else if (type == "float") { *(float*)ptr = sui_lerp(startValueF, endValueF, getTime(func, ease, currentTime / endTime)); }
			else if (type == "color") { *(Color*)ptr = ColorLerp(startValueC, endValueC, getTime(func, ease, currentTime / endTime));; }
			else if (type == "vector2") { *(Vector2*)ptr = { sui_lerp(startValueV.x, endValueV.x, getTime(func, ease, currentTime / endTime)), sui_lerp(startValueV.y, endValueV.y, getTime(func, ease, currentTime / endTime)) }; }
			return false;
		}

		Animation() = delete;
		Animation(int* ptr, float time, int endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueI(*ptr), endValueI(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(float* ptr, float time, float endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueF(*ptr), endValueF(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(Color* ptr, float time, Color endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueC(*ptr), endValueC(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
		Animation(Vector2* ptr, float time, Vector2 endValue, const char* type, Function func = Linear, Ease ease = In) : type(type), startValueV(*ptr), endValueV(endValue), ptr(ptr), func(func), ease(ease), endTime(time) {}
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
	Animation* Create(Vector2* ptr, float time, Vector2 endValue, Function func = Linear, Ease ease = In) {
		deleteCurrent((void*)ptr);
		Animation* s = new Animation(ptr, time, endValue, "vector2", func, ease);
		ActiveAnimations.insert({ ptr, s });
		return s;
	}

	void UpdateAnimations(float t) {
		for (auto it = ActiveAnimations.begin(); it != ActiveAnimations.end();) {
			if (it->second->Update(t)) {
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

Vector2 getTextOffset(TextAnchorEnum anchor) {
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

Vector3 getTextCFrame(const char* text, Font font, Rectangle rec, TextAnchorEnum anchor, int maxTextSize, int Spacing) {
	if (maxTextSize < 0 or maxTextSize > rec.height) maxTextSize = rec.height;

	int endX{};
	int endY{};
	float endSize = 1;
	float sizeMax = maxTextSize;
	Vector2 textSize{};

	while (endSize < sizeMax) {
		float middle = (endSize + sizeMax + 1) / 2;
		textSize = MeasureTextEx(font, text, middle, Spacing);
		if (textSize.x <= rec.width and textSize.y <= rec.height) endSize = middle;
		else sizeMax = middle - 1;
	}

	if (endSize > maxTextSize) endSize = maxTextSize;
	Vector2 ofst = getTextOffset(anchor);
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
	VECTOR2_VALUE,
	COLOR_VALUE,
	FOLDER
};


TextBox* FocusedTextBox = nullptr;
Object2D* PreviousHigherObject = nullptr;
Object2D* higherObject = nullptr;

template<typename Z>
void Delete(Z* ptr) {
	if (!ptr) return;

	if (ptr->Class == IMAGELABEL) {
		static_cast<ImageLabel*>(ptr)->setImage("");
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

void updateChildren(Instance*);

class Instance {
	std::function<void(Instance*)> callbackChildAdded = [](Instance*) {};
	std::function<void(Instance*)> callbackChildRemoved = [](Instance*) {};
public:
	std::function<void()> functionForTick;
	void SetForTick(std::function<void()> f) { functionForTick = f; }

	bool updateChildrenZIndex = true;

	Instance* Parent = nullptr;
	std::vector<Instance*> Children;

	std::string Name = "Instance";
	InstanceType Class = INSTANCE;

	bool __ParentObject{};

	Instance(bool a) : __ParentObject(a) {};
	Instance(Instance* p) : Parent(p) { if (p) { p->Children.push_back(this); p->callbackChildAdded(this); p->updateChildrenZIndex = true; } }
	Instance() = delete;

	virtual ~Instance() {

	}

	void setParent(Instance* ptr) {
		if (ptr == this) return;

		if (Parent) {
			std::vector<Instance*> arr;
			for (Instance*& obj : static_cast<Instance*>(Parent)->Children) {
				arr.push_back(static_cast<Instance*>(obj));
			}
			for (int i = 0; i < arr.size(); i++) {
				if (arr[i] == this) {
					Parent->Children.erase(Parent->Children.begin() + i);
					break;
				}
			}

			Parent->callbackChildRemoved(this);
		}

		Parent = ptr;
		if (ptr) {
			ptr->Children.push_back(this);
			ptr->updateChildrenZIndex = true;
			ptr->callbackChildAdded(this);
		}
	}

	void OnChildAdded(std::function<void(Instance*)> callback) {
		callbackChildAdded = callback;
	}

	void OnChildRemoved(std::function<void(Instance*)> callback) {
		callbackChildRemoved = callback;
	}

	Instance* findChild(const std::string& name) {
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
		std::function<Instance*(Instance*)> l = [=](Instance* ptr) -> Instance* {
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

	std::vector<Instance*> getDescendants(const std::function<bool(Instance*)>& condition = [](Instance* _)  { return true; }) {
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

	virtual void Update() {
		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		if (functionForTick) functionForTick();

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

class StringValue : virtual public Instance {
	constexpr static const char* DefaultName = "StringValue";
	constexpr static InstanceType DefaultClass = STRING_VALUE;
public:
	std::string Value = "";

	StringValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	StringValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	StringValue() = delete;
};

class ObjectValue : virtual public Instance {
	constexpr static const char* DefaultName = "ObjectValue";
	constexpr static InstanceType DefaultClass = OBJECT_VALUE;
public:
	Instance* Value = nullptr;

	ObjectValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	ObjectValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	ObjectValue() = delete;
};

class BoolValue : virtual public Instance {
	constexpr static const char* DefaultName = "BoolValue";
	constexpr static InstanceType DefaultClass = BOOL_VALUE;
public:
	bool Value = 0;

	BoolValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	BoolValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	BoolValue() = delete;
};

class IntValue : virtual public Instance {
	constexpr static const char* DefaultName = "IntValue";
	constexpr static InstanceType DefaultClass = INT_VALUE;
public:
	int Value = 0;

	IntValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	IntValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	IntValue() = delete;
};

class FloatValue : virtual public Instance {
	constexpr static const char* DefaultName = "FloatValue";
	constexpr static InstanceType DefaultClass = FLOAT_VALUE;
public:
	float Value = 0.0f;

	FloatValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	FloatValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	FloatValue() = delete;
};

class Vector2Value : virtual public Instance {
	constexpr static const char* DefaultName = "Vector2Value";
	constexpr static InstanceType DefaultClass = VECTOR2_VALUE;
public:
	Vector2 Value = { 0,0 };

	Vector2Value(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	Vector2Value(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	Vector2Value() = delete;
};

class ColorValue : virtual public Instance {
	constexpr static const char* DefaultName = "ColorValue";
	constexpr static InstanceType DefaultClass = COLOR_VALUE;
public:
	Color Value = { 255,255,255,255 };

	ColorValue(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	ColorValue(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	ColorValue() = delete;
};

class Folder : virtual public Instance {
	constexpr static const char* DefaultName = "Folder";
	constexpr static InstanceType DefaultClass = FOLDER;
public:

	Folder(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	Folder(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	Folder() = delete;
};

Vector2 getCanvasPosition(Object2D*); 
Vector2 getScrollFrameRS(Instance*);
Vector2 getScrollFrameRP(Instance*);
bool isScrollFrameCropping(Instance*);

class Object2D : public Instance {
	constexpr static const char* DefaultName = "Object2D";
	constexpr static InstanceType DefaultClass = OBJECT2D;

	std::function<void(Object2D*)> functionForMouseEnter;
	std::function<void(Object2D*)> functionForMouseLeave;

	std::function<void(Object2D*)> functionForMouse1HoldStart;
	std::function<void(Object2D*)> functionForMouse1HoldEnd;
	std::function<void(Object2D*)> functionForMouse2HoldStart;
	std::function<void(Object2D*)> functionForMouse2HoldEnd;

	std::function<void(Object2D*)> functionForClick1;
	std::function<void(Object2D*)> functionForClick2;

	bool isMouseButton1Down = false;
	bool isMouseButton2Down = false;

	bool startedOnObject1 = false;
	bool startedOnObject2 = false;

protected:
	bool RelativePCalculated = false;
	bool RelativeSCalculated = false;
	Vector2 RelativePosition{};
	Vector2 RelativeSize{};

public:
	Vector2 RealSize{};
	Vector2 RealPos{};
	bool CanBeEnteredIfNotHigher = false;

	virtual void eventHandler() {
		Vector2 mousePosition = GetMousePosition();
		bool mouseOnObject = pointInObject(mousePosition);

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) or !Visible or higherObject != this) { isMouseButton1Down = false; }
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) or !Visible or higherObject != this) { isMouseButton2Down = false; }

		if (mouseOnObject) {
			if (startedOnObject1 and IsMouseButtonDown(MOUSE_BUTTON_LEFT) and Visible and higherObject == this) { isMouseButton1Down = true; }
			if (startedOnObject2 and IsMouseButtonDown(MOUSE_BUTTON_RIGHT) and Visible and higherObject == this) { isMouseButton2Down = true; }

			if (Visible and ((higherObject == this and PreviousHigherObject != this) or CanBeEnteredIfNotHigher)) {
				MouseEntered = true;
				if (functionForMouseEnter) functionForMouseEnter(this);
			}
		} else {
			if ((MouseEntered and !Visible) or ((higherObject != this) or CanBeEnteredIfNotHigher)) {
				MouseEntered = false;
				if (functionForMouseLeave) functionForMouseLeave(this);
			}

			isMouseButton1Down = false;
			isMouseButton2Down = false;
		}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and mouseOnObject and higherObject == this) {
			startedOnObject1 = true;
			if (functionForMouse1HoldStart) functionForMouse1HoldStart(this);
			if (functionForClick1 and Visible) functionForClick1(this);
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) and mouseOnObject and higherObject == this) {
			startedOnObject2 = true;
			if (functionForMouse2HoldStart) functionForMouse2HoldStart(this);
			if (functionForClick2 and Visible) functionForClick2(this);
		}
		if (startedOnObject1 and IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			if (functionForMouse1HoldEnd and pointInObject(GetMousePosition())) functionForMouse1HoldEnd(this);
			startedOnObject1 = false;
		}

		if (startedOnObject2 and IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
			if (functionForMouse2HoldEnd and pointInObject(GetMousePosition())) functionForMouse2HoldEnd(this);
			startedOnObject2 = false;
		}

		if (functionForTick) functionForTick();
	}
	
	Vector2 PositionOFFSET = {};
	Vector2 SizeOFFSET = {};
	Vector2 AnchorPositionOFFSET = {};
	Vector2 Position{};
	Vector2 Size{};
	Vector2 AnchorPosition{};

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
		Vector2 sizePx = {};

		Object2D* self = this;

		Instance* current = Parent;
		Object2D* parent2D = nullptr;

		while (current) {
			parent2D = dynamic_cast<Object2D*>(current);
			if (!parent2D) {
				if (current->Parent) { current = current->Parent; continue; }
				parent2D = nullptr;
				break;
			}
			if (parent2D->__ParentObject) { parent2D = nullptr; break; }
			break;
		}

		Vector2 parentSizePx = parent2D ? parent2D->RealSize : Vector2{ (float)winWidth, (float)winHeight };

		sizePx.x = parentSizePx.x * self->Size.x + self->SizeOFFSET.x;
		sizePx.y = parentSizePx.y * self->Size.y + self->SizeOFFSET.y;

		RelativeSCalculated = true;
		RealSize = sizePx;
		RelativeSize = { sizePx.x / winWidth, sizePx.y / winHeight };
	}

	void getRealObject2Dposition() {
		if (!RelativeSCalculated) getRealObject2Dsize();

		Vector2 posPx = { 0.0f, 0.0f };
		Vector2 sizePx = RealSize;

		Vector2 anchorPx = {
			sizePx.x * AnchorPosition.x + AnchorPositionOFFSET.x,
			sizePx.y * AnchorPosition.y + AnchorPositionOFFSET.y
		};

		Vector2 localPx = {
			0.0f,
			0.0f
		};

		Instance* current = Parent;
		while (current) {
			Object2D* obj = dynamic_cast<Object2D*>(current);
			if (!obj) { current = current->Parent; continue; }

			if (!obj->RelativeSCalculated) obj->getRealObject2Dsize();
			if (!obj->RelativePCalculated and obj->Class != SCROLLFRAME) obj->getRealObject2Dposition();

			Vector2 parentSizePx = obj->RealSize;

			Vector2 parentAnchorPx = {
				obj->RealSize.x * obj->AnchorPosition.x + obj->AnchorPositionOFFSET.x,
				obj->RealSize.y * obj->AnchorPosition.y + obj->AnchorPositionOFFSET.y
			};

			Vector2 parentLocalPx = {
				obj->Position.x * parentSizePx.x + obj->PositionOFFSET.x - parentAnchorPx.x,
				obj->Position.y * parentSizePx.y + obj->PositionOFFSET.y - parentAnchorPx.y
			};

			Vector2 parentPosPx = obj->RealPos;
			if (!obj->RelativePCalculated) parentPosPx = parentLocalPx;

			Vector2 myLocalPx = {
				parentSizePx.x * Position.x + PositionOFFSET.x - anchorPx.x,
				parentSizePx.y * Position.y + PositionOFFSET.y - anchorPx.y
			};

			if (obj->Class == SCROLLFRAME) {
				Vector2 canvasPx = getCanvasPosition(obj);
				posPx.x = parentPosPx.x + myLocalPx.x - canvasPx.x * dynamic_cast<Object2D*>(current)->RealSize.x;
				posPx.y = parentPosPx.y + myLocalPx.y - canvasPx.y * dynamic_cast<Object2D*>(current)->RealSize.y;
			} else {
				posPx.x = parentPosPx.x + myLocalPx.x;
				posPx.y = parentPosPx.y + myLocalPx.y;
			}

			RelativePosition = { posPx.x / winWidth, posPx.y / winHeight };
			RealPos = posPx;
			RelativePCalculated = true;
			return;
		}

		Vector2 rootSizePx = { (float)winWidth, (float)winHeight };
		Vector2 rootLocalPx = {
			rootSizePx.x * Position.x + PositionOFFSET.x - anchorPx.x,
			rootSizePx.y * Position.y + PositionOFFSET.y - anchorPx.y
		};

		RealPos = rootLocalPx;
		RelativePosition = { RealPos.x / winWidth, RealPos.y / winHeight };
		RelativePCalculated = true;
	}

	Vector2 getMousePosition() {
		Vector2 mousePos = GetMousePosition();
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
	
	bool pointInObject(Vector2 pos) {
		Vector2 mouse = GetMouseScreenPosition();
		Vector2 windowPos = GetWindowPosition();

		int width = GetScreenWidth();
		int height = GetScreenHeight();

		if (!(mouse.x >= windowPos.x and
			mouse.x <= windowPos.x + width and
			mouse.y >= windowPos.y and
			mouse.y <= windowPos.y + height)) return false;
		if (Parent and Parent->Class == SCROLLFRAME) {
			Vector2 scrRS = getScrollFrameRS(Parent);
			Vector2 scrRP = getScrollFrameRP(Parent);
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

	void SetMouseEnter(std::function<void(Object2D*)> f) { functionForMouseEnter = f; }
	void SetMouseLeave(std::function<void(Object2D*)> f) { functionForMouseLeave = f; }
	void SetMouseClick1(std::function<void(Object2D*)> f) { functionForClick1 = f; }
	void SetMouseClick2(std::function<void(Object2D*)> f) { functionForClick2 = f; }
	void SetMouse1HoldStart(std::function<void(Object2D*)> f) { functionForMouse1HoldStart = f; }
	void SetMouse1HoldEnd(std::function<void(Object2D*)> f) { functionForMouse1HoldEnd = f; }
	void SetMouse2HoldStart(std::function<void(Object2D*)> f) { functionForMouse2HoldStart = f; }
	void SetMouse2HoldEnd(std::function<void(Object2D*)> f) { functionForMouse2HoldEnd = f; }

	bool MouseEntered = false;

	void Update() override {
		RelativeSCalculated = false;
		RelativePCalculated = false;
		if (!Visible) return;

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

	Object2D(bool a) : Instance(a) { Name = DefaultName; Class = DefaultClass; };
	Object2D(Instance* p) : Instance(p) { Name = DefaultName; Class = DefaultClass; }

	Object2D() = delete;
};

class LineEx : public Instance { // it cannot contain Object2D inheritors inside itself  |  only necessary for drawing lines
	constexpr static const char* DefaultName = "LineEx";
	constexpr static InstanceType DefaultClass = LINEEX;

	std::pair<Vector2, Vector2> getRealObject2Dposition() {
		Vector2 pos1 = { Position1.x, Position1.y };
		Vector2 pos2 = { Position2.x, Position2.y };
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

			Vector2 parentPos = {
				obj->Position.x - obj->AnchorPosition.x * obj->Size.x,
				obj->Position.y - obj->AnchorPosition.y * obj->Size.y
			};

			if (obj->Class == SCROLLFRAME) {
				Vector2 CanvasPosition = getCanvasPosition(obj);

				pos1.x = parentPos.x + (pos1.x - CanvasPosition.x) * obj->Size.x;
				pos1.y = parentPos.y + (pos1.y - CanvasPosition.y) * obj->Size.y;

				pos2.x = parentPos.x + (pos2.x - CanvasPosition.x) * obj->Size.x;
				pos2.y = parentPos.y + (pos2.y - CanvasPosition.y) * obj->Size.y;
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
	Vector2 Position1{};
	Vector2 Position2{};
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
		if (functionForTick) functionForTick();
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

void updateChildren(Instance* parent) {
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

Font getFont(const std::string& name) {
	auto it = Fonts.find(name);
	if (it != Fonts.end())
		return it->second;

	throw "Font not exist";
}

struct Clip {
	int x, y, w, h;
};
std::vector<Clip> clipStack;

Clip Intersect(const Clip& a, const Clip& b) {
	int x1 = std::max(a.x, b.x);
	int y1 = std::max(a.y, b.y);
	int x2 = std::min(a.x + a.w, b.x + b.w);
	int y2 = std::min(a.y + a.h, b.y + b.h);
	if (x2 <= x1 or y2 <= y1) return { 0, 0, 0, 0 };
	return { x1, y1, x2 - x1, y2 - y1 };
}

void PushClip(Clip last) {
	if (!clipStack.empty())
		last = Intersect(clipStack.back(), last);

	clipStack.push_back(last);
	BeginScissorMode(last.x, last.y, last.w, last.h);
}

void PopClip() {
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
public:
	Vector2 CanvasSize = { 1,1 };
	Vector2 CanvasPosition = { 0,0 };
	float ScrollSpeed = 0.5;
	bool CropDescendants = true;
	Color SliderColor = { 15,15,15,255 };
	float SliderTransparency = 0.5;
	unsigned int SliderSize = 5;
	char Direction = 'Y';
	bool ScrollEnabled = true;
	bool Animated = false;

	void Draw() {
		Object2D::Draw();

		bool pushed = false;
		if (CropDescendants) {
			PushClip({ (int)RealPos.x, (int)RealPos.y, (int)RealSize.x, (int)RealSize.y });
			pushed = true;
		}

		for (int i = 0; i < Children.size(); i++) {
			Children[i]->Update();
		}

		if (pushed) PopClip();


		if (SliderTransparency != 1 and SliderSize != 0) {

			if (CanvasSize.y > 1) {
				if (Direction == 'Y' or Direction == 'B') {
					float sliderHeight = RealSize.y * (1.0f / CanvasSize.y);
					float sliderY = RealPos.y + (RealSize.y - sliderHeight) * (CanvasPosition.y / (CanvasSize.y - 1));
					Vector2 firstPoint = { RealPos.x + RealSize.x - SliderSize * 0.6f, sliderY };
					Vector2 secondPoint = { firstPoint.x, sliderY + sliderHeight };
					DrawLineEx(firstPoint, secondPoint, SliderSize, { SliderColor.r, SliderColor.g, SliderColor.b, (unsigned char)(SliderColor.a * (1 - SliderTransparency)) });
				}
			}

			if (CanvasSize.x > 1) {
				if (Direction == 'X' or Direction == 'B') {
					float sliderWidth = RealSize.x * (1.0f / CanvasSize.x);
					float sliderX = RealPos.x + (RealSize.x - sliderWidth) * (CanvasPosition.x / (CanvasSize.x - 1));
					Vector2 firstPoint = { sliderX, RealPos.y + RealSize.y - SliderSize * 0.6f };
					Vector2 secondPoint = { sliderX + sliderWidth, firstPoint.y };
					DrawLineEx(firstPoint, secondPoint, SliderSize, { SliderColor.r, SliderColor.g, SliderColor.b, (unsigned char)(SliderColor.a * (1 - SliderTransparency)) });
				}
			}
		}
	}

	void Update() override {
		if (!Visible) return;
		if (CanvasSize.x < 1) CanvasSize.x = 1; if (CanvasSize.y < 1) CanvasSize.y = 1;
		if (Direction != 'X' and Direction != 'Y' and Direction != 'B') {
			Direction = 'Y';
		}

		getRealObject2Dsize();
		getRealObject2Dposition();
		eventHandler();

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		if (CanvasPosition.x + 1 > CanvasSize.x) 
			CanvasPosition.x = (CanvasSize.x - 1 > 0 ? CanvasSize.x - 1 : 0);
		if (CanvasPosition.y + 1 > CanvasSize.y) 
			CanvasPosition.y = (CanvasSize.y - 1 > 0 ? CanvasSize.y - 1 : 0);
		
		if (CanvasPosition.x < 0) 
			CanvasPosition.x = 0;
		if (CanvasPosition.y < 0) 
			CanvasPosition.y = 0;

		if (pointInObject(GetMousePosition()) and ScrollEnabled) {
			float WheelMove = GetMouseWheelMove();
			if (WheelMove > 0) {
				if (Direction == 'Y' or (!IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B')) {
					float newY = CanvasPosition.y - ScrollSpeed; if (newY < 0) newY = 0;
					Animate::Create(&CanvasPosition.y, 0.125, newY);
				} else if (Direction == 'X' or (IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B')) {
					float newX = CanvasPosition.x - ScrollSpeed; if (newX < 0) newX = 0;
					Animate::Create(&CanvasPosition.x, 0.125, newX);
				}
			} else if (WheelMove < 0) {
				if (Direction == 'Y' or (!IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B')) {
					float newY = CanvasPosition.y + ScrollSpeed; if (newY -1 > CanvasSize.y) newY = 0;
					Animate::Create(&CanvasPosition.y, 0.125, newY);
				} else if (Direction == 'X' or (IsKeyDown(KEY_LEFT_SHIFT) and Direction == 'B')) {
					float newX = CanvasPosition.x + ScrollSpeed; if (newX - 1 > CanvasSize.x) newX = 0;
					Animate::Create(&CanvasPosition.x, 0.125, newX);
				}
			}
		}

		Draw();
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

	ScrollFrame(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	ScrollFrame(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }

	ScrollFrame() = delete;
};
Vector2 getCanvasPosition(Object2D* obj) {
	ScrollFrame* scra = dynamic_cast<ScrollFrame*>(obj);
	if (scra) return scra->CanvasPosition;
	return { 0,0 };
}

Vector2 getScrollFrameRS(Instance* sc) {
	ScrollFrame* scra = dynamic_cast<ScrollFrame*>(sc);
	if (scra) return scra->RealSize;
	return { 0,0 };
}
Vector2 getScrollFrameRP(Instance* sc) {
	ScrollFrame* scra = dynamic_cast<ScrollFrame*>(sc);
	if (scra) return scra->RealPos;
	return { 0,0 };
}
bool isScrollFrameCropping(Instance* sc) {
	ScrollFrame* scra = dynamic_cast<ScrollFrame*>(sc);
	if (scra) return scra->CropDescendants;
	return false;
}

class TextLabel : public Object2D {
	constexpr static float TextTextureUpdateAspect = 1.3;
	constexpr static const char* DefaultName = "TextLabel";
	constexpr static InstanceType DefaultClass = TEXTLABEL;

	std::string visibleText = "";
	int lastMaxVisible = -1;
	Vector3 textParams{};
	Vector2 lastRealSize{};
	std::string lastText{};
	std::string lastFont = "";
	RenderTexture2D cachedText{};
	Vector2 newSize{};
	Vector2 lastNewSize{};
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
			size_t idx = charOffsets[std::max(3, MaxVisibleSymbols) - 3];
			visibleText = Text.substr(0, idx);
			visibleText += "...";
		} else {
			visibleText = Text;
		}
		
		textParams = getTextCFrame(visibleText.c_str(), getFont(font), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
		lastRealSize = RealSize;
		lastText = Text;
		lastFont = font;
		lastParams = textParams;
		lastMaxVisible = MaxVisibleSymbols;
		
		newSize = MeasureTextEx(getFont(font), visibleText.c_str(), textParams.z, Spacing);

		if (cachedText.id == 0 or lastNewSize.x < newSize.x or lastNewSize.y < newSize.y) {
			if (cachedText.id != 0) {
				UnloadRenderTexture(cachedText);
			}
			cachedText = LoadRenderTexture(newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect);
			lastNewSize = { newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect };
		}
		
		bool hadClip = !clipStack.empty();
		Clip current;
		if (hadClip) current = clipStack.back();

		if (hadClip) EndScissorMode();
		
		BeginTextureMode(cachedText);
		ClearBackground(BLANK);
		DrawTextEx(getFont(font), visibleText.c_str(), { 0,0 }, textParams.z, Spacing, { 255,255,255,255 });
		EndTextureMode();
		SetTextureWrap(cachedText.texture, TEXTURE_WRAP_CLAMP);

		if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
	}
public:
	std::string Text = "";
	float TextTransparency = 0.0f;
	TextAnchorEnum TextAnchor = TextAnchorEnum::CENTER;
	Color TextColor = { 0,0,0,255 };
	int TextSize = -1;
	std::string font = "Arial";
	int Spacing = defaultSpacing;
	int MaxVisibleSymbols = -1;

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
			if (c) ancestor = dynamic_cast<ScrollFrame*>(c);
			if (ancestor and ancestor->CropDescendants) {
				if (RealPos.x + RealSize.x + BorderThickness < ancestor->RealPos.x or
					RealPos.y + RealSize.y + BorderThickness < ancestor->RealPos.y or
					RealPos.x + BorderThickness > ancestor->RealPos.x + ancestor->RealSize.x or
					RealPos.y + BorderThickness > ancestor->RealPos.y + ancestor->RealSize.y) {
					return;
				}
			}

			Object2D::Draw();

			if (lastParams.z != textParams.z or cachedText.id == 0 or lastText != Text or lastFont != font or lastMaxVisible != MaxVisibleSymbols) {
				updateTexture();
			} else {
				if (std::fabsf(lastRealSize.x - RealSize.x) >= TextTextureUpdateAspect or std::fabsf(lastRealSize.y - RealSize.y) >= TextTextureUpdateAspect) {
					lastRealSize = RealSize;
					newSize = MeasureTextEx(getFont(font), visibleText.c_str(), textParams.z, Spacing);
					textParams = getTextCFrame(visibleText.c_str(), getFont(font), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
				}
			}

			if (textParams.z > 1) {
				if (cachedText.id == 0) {
					updateTexture();
				}
				Rectangle sourceRec = { 0.0f, (float)(cachedText.texture.height - newSize.y), (float)newSize.x, -(float)newSize.y };
				Rectangle destRec = { RealPos.x + textParams.x, RealPos.y + textParams.y, (float)newSize.x, (float)newSize.y };
				Vector2 origin = { 0, 0 };

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

KeyMapping KeysMapping[49] = {
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
	// { KEY_ENTER, "", "\n", "", "\n"} // WIP
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
	std::string lastText{};
	RenderTexture2D cachedText;
	TextBox* lastFocused = nullptr;
	std::string lastPlaceholder;
	Vector2 newSize{};
	Vector2 lastRealSize{};
	std::string lastFont = "";
	Vector3 lastParams = Vector3{};
	char lastHideText = '\0';
	Vector2 lastNewSize{};
	TextBoxType lastType = TextResizing;
	int lastCursorIndex = -1;

	std::string Text = "";
	float viewportPosition = 0;

	std::function<void(Object2D*)> TextChanged;

	void updateTextParams() {
		if (Type == Viewported) {
			textParams.y = 0;
			textParams.x = 0;
			textParams.z = RealSize.y;
		}
		else {
			if (Text != "") {
				textParams = getTextCFrame(Text.c_str(), getFont(font), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
			}
			else {
				textParams = getTextCFrame(PlaceholderText.c_str(), getFont(font), { RealPos.x, RealPos.y, RealSize.x, RealSize.y }, TextAnchor, TextSize, Spacing);
			}
		}
	}
	void updateTexture() {
		updateTextParams();
		lastText = Text;
		lastPlaceholder = PlaceholderText;
		lastFocused = FocusedTextBox;
		lastFont = font;
		lastParams = textParams;
		lastRealSize = RealSize;
		lastHideText = HideText;
		lastType = Type;

		if (Text != "") {
			newSize = MeasureTextEx(getFont(font), Text.c_str(), textParams.z, Spacing);
		} else {
			if (CursorIndex == -1 or FocusedTextBox != this) {
				newSize = MeasureTextEx(getFont(font), PlaceholderText.c_str(), textParams.z, Spacing);
			}
		}

		if (lastNewSize.x < newSize.x or lastNewSize.y < newSize.y) {
			if (cachedText.id != 0) {
				UnloadRenderTexture(cachedText);
			}

			cachedText = LoadRenderTexture(newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect);
			lastNewSize = { newSize.x * TextTextureUpdateAspect, newSize.y * TextTextureUpdateAspect };
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
				t = Text;
			} else {
				for (int i = 0; i < Text.size(); i++) {
					t += HideText;
				}
			}

			DrawTextEx(getFont(font), t.c_str(), {0,0}, textParams.z, Spacing, {255,255,255,255});
		} else {
			if (CursorIndex == -1 or FocusedTextBox != this) {
				DrawTextEx(getFont(font), PlaceholderText.c_str(), { 0,0 }, textParams.z, Spacing, { 255,255,255,255 });
			}
		}

		EndTextureMode();
		SetTextureWrap(cachedText.texture, TEXTURE_WRAP_CLAMP);
		if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
	}
public:
	Color CursorColor = { 0,0,0,255 };
	std::string PlaceholderText = "PlaceholderText";
	Color PlaceholderTextColor = { 150, 150, 150, 255 };
	Color TextColor = { 0,0,0,255 };
	TextAnchorEnum TextAnchor = TextAnchorEnum::CENTER;
	int TextSize = -1;
	int maxSymbols = 20;
	float TextTransparency = 0;
	std::string AllowedSymbols = "";
	std::string font = "Arial";
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

		if (lastType != Type or cachedText.id == 0 or lastHideText != HideText or lastParams.x != textParams.x or lastParams.y != textParams.y or lastParams.z != textParams.z or lastFont != font or (FocusedTextBox == this and lastFocused != this) or (lastFocused == this and FocusedTextBox != this)) {
			updateTexture();
		} else if (lastText != Text) {
			updateTexture();
			if (TextChanged) {
				TextChanged(this);
			}
		} else {
			if (lastRealSize.x != RealSize.x or lastRealSize.y != RealSize.y) {
				updateTextParams();
				if (Text == "") {
					newSize = MeasureTextEx(getFont(font), PlaceholderText.c_str(), textParams.z, Spacing);
				} else {
					newSize = MeasureTextEx(getFont(font), Text.c_str(), textParams.z, Spacing);
				}
			}
		}

		if (textParams.z > 1) {
			if (cachedText.id == 0) {
				updateTexture();
			}

			Vector2 sizeToDraw = (Type == Viewported) ? RealSize : newSize;

			Rectangle sourceRec = { (Type == Viewported) ? viewportPosition : 0.0f, (cachedText.texture.height - sizeToDraw.y), sizeToDraw.x, -sizeToDraw.y};
			Rectangle destRec = { RealPos.x + textParams.x, RealPos.y + textParams.y, sizeToDraw.x, sizeToDraw.y };
			Vector2 origin = { 0, 0 };

			Color clr;
			if (Text == "") {
				clr = { PlaceholderTextColor.r, PlaceholderTextColor.g, PlaceholderTextColor.b, (unsigned char)(PlaceholderTextColor.a * (1 - TextTransparency)) };
			} else {
				clr = { TextColor.r, TextColor.g, TextColor.b, (unsigned char)(TextColor.a * (1 - TextTransparency)) };
			}

			DrawTexturePro(cachedText.texture, sourceRec, destRec, origin, 0, clr);
		}

		if (Text == "") {
			if (CursorVisible and FocusedTextBox == this) {
				if (textParams.z > 3) {
					float sizeY = MeasureTextEx(getFont(font), " ", textParams.z, Spacing).y;
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

			Vector2 size = MeasureTextEx(getFont(font), textBeforeCursor.c_str(), textParams.z, Spacing);

			if (size.x == 0 and size.y == 0) {
				size.y = MeasureTextEx(getFont(font), "a", textParams.z, Spacing).y;
			}

			DrawLineEx({ RealPos.x + textParams.x + size.x + 2 - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + 2 }, { RealPos.x + textParams.x + size.x + 2 - ((Type == Viewported) ? viewportPosition : 0), RealPos.y + textParams.y + size.y - 4 }, CursorSize, CursorColor);
		}
	}

	void Update() override {
		if (!Visible) { CursorIndex = -1; CursorVisible = false; Text = ""; return; }
		if (!(FocusedTextBox == this)) { CursorIndex = -1; CursorVisible = false; deleteText = true; }
		
		getRealObject2Dsize();
		getRealObject2Dposition();
		eventHandler();

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		if (FocusedTextBox == this and deleteText and ClearOnClick) {
			Text = "";
			CursorIndex = 0;
			deleteText = false;
			updateCharOffsets();
		}

		// CURSOR

		CursorTime += dt;
		if (CursorTime >= CursorCooldown) { CursorVisible = !CursorVisible; CursorTime = 0.0f; }

		Vector2 mousePosition = GetMousePosition();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (pointInObject(mousePosition) and FocusedTextBox != this and higherObject == this and ClearOnClick) {
				Text = "";
			}

			if (!pointInObject(mousePosition)) {
				CursorIndex = -1;
				CursorVisible = false;

				if (higherObject and higherObject->Class == TEXTBOX) {
					FocusedTextBox = static_cast<TextBox*>(higherObject);
				} else {
					FocusedTextBox = nullptr;
				}
			}

			if (pointInObject(mousePosition) and higherObject == this) {
				CursorTime = 0.0f;
				CursorVisible = true;
				FocusedTextBox = this;

				std::string text;
				if (HideText != '\0') {
					for (int i = 0; i < Text.size(); i++) {
						text += HideText;
					}
				}
				else {
					text = Text;
				}

				updateTextParams();
				std::string textBeforeCursor;
				if (HideText != '\0') {
					for (int i = 0; i < Text.size(); i++) {
						textBeforeCursor += HideText;
					}
				} else {
					textBeforeCursor = Text;
				}

				float textStartX = RealPos.x + textParams.x;
				float clickX = mousePosition.x - textStartX + ((Type == Viewported) ? viewportPosition : 0.0f);

				CursorIndex = 0;
				if (!Text.empty()) {
					for (int i = 1; i < charOffsets.size(); i++) {
						float widthPrev = MeasureTextEx(getFont(font), text.substr(0, charOffsets[i - 1]).c_str(), textParams.z, Spacing).x;
						float widthCurr = MeasureTextEx(getFont(font), text.substr(0, charOffsets[i]).c_str(), textParams.z, Spacing).x;
						if (clickX < (widthPrev + widthCurr) / 2.0f) {
							CursorIndex = i - 1;
							break;
						}
						CursorIndex = i;
					}
				} else {
					CursorIndex = 0;
				}
			}
		}

		// KEYBOARD INPUT

		if (FocusedTextBox == this and Visible) {
			if (maxSymbols >= charOffsets.size()) {
				std::string layout = getLayout();
				int index = (layout == "RU") ? 2 : 0;
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
							if (not allowed) continue;
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

		// UTILS (BACKSPACE | DEL | CTRL BACKSPACE | ARROWS
		if (FocusedTextBox == this and Visible) {
			if (IsKeyPressed(KEY_BACKSPACE) or IsKeyPressed(KEY_DELETE)) {
				if (IsKeyDown(KEY_LEFT_CONTROL)) {
					if (CursorIndex == 0) return;

					int lower = CursorIndex;
					bool Space = false;
					bool first = true;

					for (int i = charOffsets.size() - 1; i >= 0; i--) {
						if (charOffsets[i] > CursorIndex) continue;
						int index = charOffsets[i];
						unsigned char c = Text[index];

						if (c == '.' or c == ',' or c == ':' or c == ';' or c == '?' or c == '!' or c == '/' or c == '\\' or c == '\'' or c == '\"' or c == '`' or c == '~') {
							lower = index;
							break;
						}
						else if ((c == ' ' and first)) {
							Space = true;
							continue;
						}
						else if (c == ' ' and not Space) {
							break;
						}
						else if (c != ' ' and Space) {
							Space = false;
						}

						first = false;

						lower = index;
						CursorVisible = true; CursorTime = 0.0f;
					}

					Text = Text.substr(0, lower);
					updateCharOffsets();
				} else {
					if (charOffsets.size() > 1) {
						if (CursorIndex > 0) {
							for (int i = charOffsets.size() - 1; i >= 0; i--) {
								if (charOffsets[i] > CursorIndex) continue;
								int index = charOffsets[i];

								Text = Text.substr(0, index - 1) + Text.substr(index);

								CursorIndex = charOffsets[((i - 1) >= 0 ? i - 1 : 0)];
								break;
							}

							CursorVisible = true;
							CursorTime = 0.0f;
						}
					} else {
						CursorIndex = 0;
						Text = "";
					}

					updateCharOffsets();
				}
			}

			if (IsKeyPressed(KEY_LEFT)) {
				int z = CursorIndex - 1; if (z < 0) z = 0;
				CursorIndex = z;
				CursorVisible = true; CursorTime = 0.0f;
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				int z = CursorIndex + 1; if (z > charOffsets.size() - 1) z = charOffsets.size() - 1;
				CursorIndex = z;
				CursorVisible = true; CursorTime = 0.0f;
			}
		}

		if (Type == Viewported) {
			if (lastCursorIndex != CursorIndex) {
				lastCursorIndex = CursorIndex;

				if (Text.empty() or CursorIndex == -1) {
					viewportPosition = 0.0f;
				} else {
					std::string textBeforeCursor = Text.substr(0, CursorIndex);
					Vector2 textSize = MeasureTextEx(getFont(font), textBeforeCursor.c_str(), textParams.z, Spacing);

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

	void SetText(const std::string& t) {
		if (t.size() > maxSymbols) {
			Text = t.substr(0, maxSymbols);
			updateCharOffsets();
			CursorIndex = maxSymbols;
			return;
		}

		Text = t;
		updateCharOffsets();
		CursorIndex = t.size();
	}

	std::string GetText() const {
		return Text;
	}

	void OnTextChanged(std::function<void(Object2D*)> f) {
		TextChanged = f;
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
	TextBox(Instance* p) :  Object2D(p) { Name = DefaultName; Class = DefaultClass; Active = true; }

	TextBox() = delete;
};

enum ImageOverlayFormat {
	STRETCH = 0, // STRETCH ON FULL OBJECT
	FIT = 1, // FIT BY RESOLUTION
	CROP = 2, // CUT EXCESS
};

class ImageLabel : public Object2D {
	constexpr static const char* DefaultName = "ImageLabel";
	constexpr static InstanceType DefaultClass = IMAGELABEL;

	Texture2D tex{};
	int lastId = -1;
	bool imageOwner = false;

	void updateTexture() {
		if (tex.id == 0 or lastId != tex.id) {
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
public:
	Image image{};
	ImageOverlayFormat Overlay = FIT;
	float ImageTransparency = 0.0f;
	Color ImageColor = { 255,255,255,255 };
	bool RoundImage = false;
	float Rotation = 0;
	Vector2 Origin = { 0, 0 };

	void setImage(std::string way = "") {
		if (way == "" or way == "\n") {
			imageOwner = false;
			if (imageOwner and image.data) UnloadImage(image);
			if (tex.id != 0) {
				UnloadTexture(tex);
				imageOwner = false;
				tex.id = 0;
			}
			return;
		}

		if (imageOwner and image.data) UnloadImage(image);
		imageOwner = true;
		image = LoadImage(way.c_str());
		if (tex.id != 0) {
			UnloadTexture(tex);
		}

		lastId = -1818489;
	}

	void setImage(Image im) {
		if (imageOwner and image.data) UnloadImage(image);
		imageOwner = false;
		image = im;
		if (tex.id != 0) {
			UnloadTexture(tex);
			tex.id = 0;
		}

		lastId = -1010101;
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
			Rectangle destRec = { RealPos.x+Origin.x, RealPos.y+Origin.y, RealSize.x, RealSize.y };
			Rectangle srcRec = { 0, 0, image.width, image.height };

			if (Overlay == FIT) {
				float imageAspect = (float)image.width / image.height;
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
				float imageAspect = (float)image.width / image.height;
				float rectAspect = RealSize.x / RealSize.y;

				if (imageAspect > rectAspect) {
					float cropWidth = image.height * rectAspect;
					srcRec.x = (image.width - cropWidth) / 2.0f;
					srcRec.width = cropWidth;
				} else {
					float cropHeight = image.width / rectAspect;
					srcRec.y = (image.height - cropHeight) / 2.0f;
					srcRec.height = cropHeight;
				}
			}

			if (Roundness and RoundImage) {
				static bool roundShaderLoaded = false;
				static Shader shader;
				static float lastRoundness = 0;
				if (!roundShaderLoaded) {
					shader = getShader("TextureRoundness");
					roundShaderLoaded = true;
				}

				if (Roundness != lastRoundness) {
					lastRoundness = Roundness;
					SetShaderValue(shader, GetShaderLocation(shader, "roundness"), &Roundness, SHADER_UNIFORM_FLOAT);
				}
				
				BeginShaderMode(shader);
				DrawTexturePro(tex, srcRec, destRec, Origin, Rotation, { ImageColor.r, ImageColor.g, ImageColor.b, (unsigned char)(ImageColor.a * (1 - ImageTransparency)) });
				EndShaderMode();
			} else {
				DrawTexturePro(tex, srcRec, destRec, Origin, Rotation, { ImageColor.r, ImageColor.g, ImageColor.b, (unsigned char)(ImageColor.a * (1 - ImageTransparency)) });
			}
		}
	}

	void UpdateWithType(const std::string& type, std::vector<unsigned char>& data) {
		if (imageOwner and image.data) UnloadImage(image);
		
		image = LoadImageFromMemory(type.c_str(), data.data(), data.size());

		if (image.data == nullptr) return;
		if (tex.id != 0 and imageOwner) UnloadTexture(tex);

		imageOwner = true;
		tex = LoadTextureFromImage(image);
		GenTextureMipmaps(&tex);
		SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
		SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
		UnloadImage(image);
	}

	void Update() override {
		RelativeSCalculated = false;
		RelativePCalculated = false;
		if (!Visible) return;

		if (updateChildrenZIndex) {
			updateChildren(this);
		}

		getRealObject2Dsize();
		getRealObject2Dposition();
		eventHandler();
		updateTexture();
		Draw();

		for (int i = 0; i < Children.size(); i++) {
			Instance* child = Children[i];
			child->Update();
		}
	}

	ImageLabel* Clone() const override {
		ImageLabel* i = new ImageLabel(*this);
		i->Parent = nullptr;
		i->Children.clear();
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		i->imageOwner = false;
		i->tex.id = 0;
		i->setImage(this->image);

		return i;
	}

	ImageLabel(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	ImageLabel(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }

	ImageLabel() = delete;

	~ImageLabel() {
		if (imageOwner) UnloadImage(image);
		if (tex.id != 0) UnloadTexture(tex);
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
	Vector2 Origin = { 0, 0 };

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
		} else {
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

std::vector<unsigned char> ImageToJpgBytes(const std::string& path, int quality = 90) {
	Image img = LoadImage(path.c_str());
	
	ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8);

	std::vector<unsigned char> out;

	stbi_write_jpg_to_func(
		[](void* context, void* data, int size) {
			auto* vec = static_cast<std::vector<unsigned char>*>(context);
			unsigned char* bytes = static_cast<unsigned char*>(data);
			vec->insert(vec->end(), bytes, bytes + size);
		},
		&out,
		img.width,
		img.height,
		3,
		img.data,
		quality
	);

	UnloadImage(img);

	return out;
}

void DrawFrame(Instance* StartInstance) {
	BeginDrawing();
	ClearBackground({ 255,255,255,255 });
	StartInstance->Update();
	EndDrawing();
}

void toggleFPS(Instance* s, Color textColor = { 0,0,0,255 }) {
	static TextLabel* labelFPS = nullptr;
	if (!labelFPS) {
		labelFPS = new TextLabel(s);
		labelFPS->BackgroundTransparency = 1;
		labelFPS->TextSize = -1;
		new ChangedSignal(accurateFPS, []() { 
			static int last = 0; 
			if (last != accurateFPS) {
				std::ostringstream os;
				os << accurateFPS << " FPS";
				labelFPS->Text = os.str();
			}
		});
		labelFPS->Name = "FPS_LABEL";
		labelFPS->Active = false;
		labelFPS->Size = { 0.15, 0.1 };
		labelFPS->Position = { 0.85, 0 };
		labelFPS->TextAnchor = TextAnchorEnum::NE;
		labelFPS->ZIndex = 1000;
		labelFPS->TextColor = { 0,0,0,255 };
		labelFPS->Visible = false;
		labelFPS->TextColor = textColor;
	}

	labelFPS->Visible = !labelFPS->Visible;
}

namespace debug {
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
		sas->Text = text;
		sas->BackgroundTransparency = 1;
		sas->TextColor = typeColor[currentColor]; sas->TextSize = -1;
		sas->font = "Arial";
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
		debugMenu->Size = { 1,1 };
		debugMenu->BackgroundTransparency = 0.9;
		debugMenu->BackgroundColor = DefaultDebugColor;
		debugMenu->Visible = false;
		debugMenu->ZIndex = 100000;
		debugMenu->Name = "debugMenu";

		TextLabel* lowerName = new TextLabel(debugMenu);
		lowerName->Name = "debugName";
		lowerName->Text = "(F2) Debug Menu";
		lowerName->TextSize = -1;
		lowerName->TextColor = DefaultDebugColor;
		lowerName->Position = { 0.03, 0.9 };
		lowerName->Size = { 0.24, 0.1 };
		lowerName->TextAnchor = TextAnchorEnum::SE;
		lowerName->BackgroundTransparency = 1;
		lowerName->font = "rog";

		/************************
		*       Settings        *
		************************/

		Object2D* SettingsFrame = new Object2D(debugMenu);
		SettingsFrame->Size = { 0.4, 0.25 };
		SettingsFrame->Position = { 0.04, 0.03 };
		SettingsFrame->BackgroundTransparency = 0.2;
		SettingsFrame->BorderColor = DefaultDebugColor;
		SettingsFrame->BorderThickness = 3;
		SettingsFrame->Name = "SettingsFrame";

		TextLabel* SettingsName = new TextLabel(SettingsFrame);
		SettingsName->Name = "SettingsName";
		SettingsName->Text = "Settings";
		SettingsName->TextSize = -1;
		SettingsName->TextColor = DefaultDebugColor;
		SettingsName->Position = { 0.5, 0 };
		SettingsName->AnchorPosition = { 0.5, 0 };
		SettingsName->Size = { 0.8, 0.1 };
		SettingsName->TextAnchor = TextAnchorEnum::CENTER;
		SettingsName->BackgroundTransparency = 1;
		SettingsName->font = "rog";

		TextLabel* AnimLabel = new TextLabel(SettingsFrame);
		AnimLabel->Size = { 0.7, 0.2 };
		AnimLabel->BackgroundTransparency = 1;
		AnimLabel->Position = { 0, 0.1 };
		AnimLabel->Text = " Animations";
		AnimLabel->TextAnchor = TextAnchorEnum::W;
		AnimLabel->TextSize = -1;
		AnimLabel->TextColor = DefaultDebugColor;
		AnimLabel->font = "rog";
		AnimLabel->Name = "animLabel";

		TextLabel* AnimButton = new TextLabel(SettingsFrame);
		AnimButton->Size = { 0.19, 0.15 };
		AnimButton->BackgroundColor = Animations ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 };
		AnimButton->Position = { 0.8, 0.125 };
		AnimButton->Text = Animations ? " On " : " Off ";
		AnimButton->TextAnchor = TextAnchorEnum::W;
		AnimButton->TextSize = -1;
		AnimButton->TextColor = { 0,0,0,255 };
		AnimButton->font = "rog";
		AnimButton->Name = "animButton";
		AnimButton->Active = true;
		AnimButton->SetMouseClick1([](Object2D* t) {Animations = !Animations; });
		AnimButton->Roundness = 0.3;

		TextLabel* LGMlabel = new TextLabel(SettingsFrame);
		LGMlabel->Size = { 0.7, 0.2 };
		LGMlabel->BackgroundTransparency = 1;
		LGMlabel->Position = { 0, 0.3 };
		LGMlabel->Text = " Low Graphics Mode";
		LGMlabel->TextAnchor = TextAnchorEnum::W;
		LGMlabel->TextSize = -1;
		LGMlabel->TextColor = DefaultDebugColor;
		LGMlabel->font = "rog";
		LGMlabel->Name = "LGMlabel";

		TextLabel* LGMbutton = new TextLabel(SettingsFrame);
		LGMbutton->Size = { 0.19, 0.15 };
		LGMbutton->BackgroundColor = lowGraphicsMode ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 };
		LGMbutton->Position = { 0.8, 0.325 };
		LGMbutton->Text = lowGraphicsMode ? " On " : " Off ";
		LGMbutton->TextAnchor = TextAnchorEnum::W;
		LGMbutton->TextSize = -1;
		LGMbutton->TextColor = { 0,0,0,255 };
		LGMbutton->font = "rog";
		LGMbutton->Name = "LGMbutton";
		LGMbutton->Active = true;
		LGMbutton->SetMouseClick1([](Object2D* t) {lowGraphicsMode = !lowGraphicsMode; });
		LGMbutton->Roundness = 0.3;

		TextLabel* FPSlabel = new TextLabel(SettingsFrame);
		FPSlabel->Size = { 0.65, 0.2 };
		FPSlabel->BackgroundTransparency = 1;
		FPSlabel->Position = { 0, 0.5 };
		FPSlabel->Text = " FPS mode";
		FPSlabel->TextAnchor = TextAnchorEnum::W;
		FPSlabel->TextSize = -1;
		FPSlabel->TextColor = DefaultDebugColor;
		FPSlabel->font = "rog";
		FPSlabel->Name = "FPSlabel";

		Object2D* FPSframe = new TextLabel(SettingsFrame);
		FPSframe->Size = { 0.3, 0.2 };
		FPSframe->BackgroundTransparency = 1;
		FPSframe->Roundness = 0.3;
		FPSframe->Position = { 0.7, 0.5 };
		FPSframe->Name = "FPSlabel";
		TextLabel* FPSleft = new TextLabel(FPSframe);
		FPSleft->Size = { 0.25, 0.6 };
		FPSleft->BackgroundTransparency = 1;
		FPSleft->Position = { 0.0, 0.2 };
		FPSleft->Text = "<";
		FPSleft->TextAnchor = TextAnchorEnum::CENTER;
		FPSleft->TextSize = -1;
		FPSleft->TextColor = DefaultDebugColor;
		FPSleft->font = "rog";
		FPSleft->Name = "FPSleft";
		FPSleft->Active = true;
		FPSleft->SetMouseClick1([](Object2D* t) { currentFPSindex--; currentFPSindex += 4; currentFPSindex = currentFPSindex % 4; });
		TextLabel* FPSquantity = new TextLabel(FPSframe);
		FPSquantity->Size = { 0.5, 1 };
		FPSquantity->BackgroundTransparency = 1;
		FPSquantity->Position = { 0.25, 0 };
		std::ostringstream st; st << " " << typeFPS[currentFPSindex] << " "; FPSquantity->Text = (currentFPSindex == 2 ? "FULL" : ((currentFPSindex == 3) ? "V-SYNC" : st.str()));
		FPSquantity->TextSize = -1;
		FPSquantity->TextColor = DefaultDebugColor;
		FPSquantity->font = "rog";
		FPSquantity->Name = "FPSquantity";
		TextLabel* FPSright = new TextLabel(FPSframe);
		FPSright->Size = { 0.25, 0.6 };
		FPSright->BackgroundTransparency = 1;
		FPSright->Position = { 0.75, 0.2 };
		FPSright->Text = ">";
		FPSright->TextAnchor = TextAnchorEnum::CENTER;
		FPSright->TextSize = -1;
		FPSright->TextColor = DefaultDebugColor;
		FPSright->font = "rog";
		FPSright->Name = "FPSright";
		FPSright->Active = true;
		FPSright->SetMouseClick1([](Object2D* t) { currentFPSindex++; currentFPSindex += 4; currentFPSindex = currentFPSindex % 4; });

		TextLabel* Colorlabel = new TextLabel(SettingsFrame);
		Colorlabel->Size = { 0.65, 0.2 };
		Colorlabel->BackgroundTransparency = 1;
		Colorlabel->Position = { 0, 0.7 };
		Colorlabel->Text = " Menu color";
		Colorlabel->TextAnchor = TextAnchorEnum::W;
		Colorlabel->TextSize = -1;
		Colorlabel->TextColor = DefaultDebugColor;
		Colorlabel->font = "rog";
		Colorlabel->Name = "Colorlabel";

		Object2D* Colorframe = new TextLabel(SettingsFrame);
		Colorframe->Size = { 0.3, 0.2 };
		Colorframe->BackgroundTransparency = 1;
		Colorframe->Roundness = 0.3;
		Colorframe->Position = { 0.7, 0.7 };
		Colorframe->Name = "Colorframe";
		TextLabel* Colorleft = new TextLabel(Colorframe);
		Colorleft->Size = { 0.25, 0.6 };
		Colorleft->BackgroundTransparency = 1;
		Colorleft->Position = { 0.0, 0.2 };
		Colorleft->Text = "<";
		Colorleft->TextAnchor = TextAnchorEnum::CENTER;
		Colorleft->TextSize = -1;
		Colorleft->TextColor = DefaultDebugColor;
		Colorleft->font = "rog";
		Colorleft->Name = "Colorleft";
		Colorleft->Active = true;
		Colorleft->SetMouseClick1([](Object2D* t) { currentColor--; currentColor += 9; currentColor = currentColor % 9; });
		Object2D* ColorBlock = new TextLabel(Colorframe);
		ColorBlock->Size = { 0.5, 0.8 };
		ColorBlock->BackgroundColor = DefaultDebugColor;
		ColorBlock->Position = { 0.25, 0.1 };
		ColorBlock->Roundness = 0.3;
		ColorBlock->Name = "ColorBlock";
		TextLabel* Colorright = new TextLabel(Colorframe);
		Colorright->Size = { 0.25, 0.6 };
		Colorright->BackgroundTransparency = 1;
		Colorright->Position = { 0.75, 0.2 };
		Colorright->Text = ">";
		Colorright->TextAnchor = TextAnchorEnum::CENTER;
		Colorright->TextSize = -1;
		Colorright->TextColor = DefaultDebugColor;
		Colorright->font = "rog";
		Colorright->Name = "Colorright";
		Colorright->Active = true;
		Colorright->SetMouseClick1([](Object2D* t) { currentColor++; currentColor += 9; currentColor = currentColor % 9; });

		/******************
		*       logs      *
		******************/

		Object2D* LogsFrame = new Object2D(debugMenu);
		LogsFrame->Size = { 0.4, 0.6 };
		LogsFrame->Position = { 0.04, 0.3 };
		LogsFrame->BackgroundTransparency = 0.2;
		LogsFrame->BorderColor = DefaultDebugColor;
		LogsFrame->BorderThickness = 3;
		LogsFrame->Name = "LogsFrame";

		TextLabel* LogsName = new TextLabel(LogsFrame);
		LogsName->Name = "LogsName";
		LogsName->Text = "Logs";
		LogsName->TextSize = -1;
		LogsName->TextColor = DefaultDebugColor;
		LogsName->Position = { 0.5, 0 };
		LogsName->AnchorPosition = { 0.5, 0 };
		LogsName->Size = { 0.8, 0.055 };
		LogsName->TextAnchor = TextAnchorEnum::CENTER;
		LogsName->BackgroundTransparency = 1;
		LogsName->font = "rog";

		console = new ScrollFrame(LogsFrame);
		console->BackgroundColor = { 0,0,0,255 };
		console->BackgroundTransparency = 0.1;
		console->BorderThickness = 3;
		console->BorderColor = DefaultDebugColor;
		console->Size = { 1, 0.93 };
		console->Position = { 0, 0.07 };
		console->SliderColor = { 255,255,255,255 };
		console->Name = "consoleLogs";
		console->OnChildAdded([](Instance* child) {
			int n = console->Children.size();
			std::ostringstream s; s << n;
			TextLabel* c = static_cast<TextLabel*>(child);
			c->Name = s.str();
			c->Size = { 1, 0.05 };
			c->Position = { 0, 0.05f * (n - 1) };
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
		treeFrame->Size = { 0.49, 0.87 };
		treeFrame->Position = { 0.47, 0.03 };
		treeFrame->BackgroundTransparency = 0.2;
		treeFrame->BorderColor = DefaultDebugColor;
		treeFrame->BorderThickness = 3;
		treeFrame->Name = "treeFrame";
		treeFrame->Active = true;
		TextLabel* treeName = new TextLabel(treeFrame);
		treeName->Name = "treeName";
		treeName->Text = "Objects hierarchy";
		treeName->TextSize = -1;
		treeName->TextColor = DefaultDebugColor;
		treeName->Position = { 0.5, 0 };
		treeName->AnchorPosition = { 0.5, 0 };
		treeName->Size = { 0.8, 0.055 };
		treeName->TextAnchor = TextAnchorEnum::CENTER;
		treeName->BackgroundTransparency = 1;
		treeName->font = "rog";
		Object2D* manageMenu = new Object2D(treeFrame);
		manageMenu->Name = "manageMenu";
		manageMenu->Position = { 0, 0.06 };
		manageMenu->Size = { 1, 0.05 };
		manageMenu->BorderThickness = 3;
		manageMenu->BackgroundTransparency = 1;
		manageMenu->BorderColor = DefaultDebugColor;
		ScrollFrame* way = new ScrollFrame(manageMenu);
		way->Name = "directory";
		way->BackgroundTransparency = 1;
		way->Position = { 0, 0 };
		way->Size = { 1, 1 };
		way->Direction = 'X';
		way->SliderColor = { 255,255,255,255 };
		ScrollFrame* treeScroll = new ScrollFrame(treeFrame);
		treeScroll->Name = "treeScroll";
		treeScroll->Position = { 0, 0.12 };
		treeScroll->Size = { 0.5, 0.88 };
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

		new ChangedSignal<int>(currentFPSindex, [FPSquantity]() { SetTargetFPS((typeFPS[currentFPSindex] == 0) ? GetMonitorRefreshRate(GetCurrentMonitor()) : typeFPS[currentFPSindex]); std::ostringstream s; s << " " << typeFPS[currentFPSindex] << " "; FPSquantity->Text = (currentFPSindex == 2 ? "FULL" : ((currentFPSindex == 3) ? "V-SYNC" : s.str())); });
		new ChangedSignal<bool>(Animations, [AnimButton]() { AnimButton->BackgroundColor = Animations ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 }; AnimButton->Text = Animations ? " On " : " Off "; });
		new ChangedSignal<bool>(lowGraphicsMode, [LGMbutton]() { LGMbutton->BackgroundColor = lowGraphicsMode ? Color{ 204, 255, 204, 255 } : Color{ 255, 204, 204, 255 }; LGMbutton->Text = lowGraphicsMode ? " On " : " Off "; });

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
					element->Position = { (objects.size() - i - 1) * 0.25f, 0 };
					element->Size = { 0.2, 0.9 };
					element->Text = objects[i]->Name;
					element->Active = true;

					if (i != 0) {
						TextLabel* element2 = new TextLabel(way);
						element2->Name = ">";
						element2->BackgroundTransparency = 1;
						element2->TextColor = DefaultDebugColor;
						element2->Position = { (objects.size() - i - 1) * 0.25f + 0.2f , 0 };
						element2->Size = { 0.05, 0.9 };
						element2->Text = ">";
					}

					element->SetMouseClick1([i](Object2D* t) { currentInstance = objects[i]; });
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
					element->Position = { 0, (i - dec) * 0.05f };
					element->Size = { 1, 0.05 };
					std::ostringstream pupupupu; pupupupu << " > " << currentInstance->Children[i]->Name;
					element->Text = pupupupu.str();
					element->Active = true;
					element->TextAnchor = TextAnchorEnum::W;
					element->SetMouseClick1([i, dec](Object2D* t) { currentInstance = objects2[i - dec]; });
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

void updateSignals() {
	for (auto obj : ActiveSignals) {
		if (!obj) continue;
		obj->Update();
	}
}

void SUI_SetWindowSize(int newW, int newH) {
	changeWindowSize = { (float)newW, (float)newH };
	changeWindowSizeB = true;
}

void SUI_SetWindowPosition(int newX, int newY) {
	#ifdef __linux__
		SetWindowPosition(newX, newY);
	#elif _WIN32
		SetWindowPosition(newX, newY);
	#endif
}

void start(Instance& StartInstance, Vector3 inf, const char* name, const char* iconName="", unsigned int flags = 4) {
	SetConfigFlags(flags);
	SetTraceLogLevel(LOG_NONE);

	winWidth = inf.x;
	winHeight = inf.y;
	
	InitWindow(inf.x, inf.y, name);
	SetTargetFPS((inf.z <= 0) ? GetMonitorRefreshRate(GetCurrentMonitor()) : inf.z);
	if (iconName != "") SetWindowIcon(LoadImage(iconName));
	
	SetExitKey(KEY_NULL);
	createFont("Arial", "Fonts/arial.ttf", 100);
	createFont("rog", "Fonts/rogFont.otf", 50);
	loadNewShader("TextureRoundness", "", "include/simpleUI Shaders/texture_roundness.frag");

	for (auto& tup : queuedFonts) {
		createFont(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup));
	}
	queuedFonts.clear();

	while (programRunning and !WindowShouldClose()) {
		if (IsWindowFullscreen()) ToggleFullscreen();
		if (changeWindowSizeB) {
			#ifdef __linux__
				SetWindowSize(changeWindowSize.x, changeWindowSize.y);
				changeWindowSizeB = false;
			#elif _WIN32
				SetWindowSize(changeWindowSize.x, changeWindowSize.y);
				changeWindowSizeB = false;
			#endif
		}

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
		Vector2 mousePosition = GetMousePosition();
		Tasks::UpdateTasks(dt);

		std::function<Object2D* (Instance*)> getTop = [&mousePosition](Instance* parent) {
			Object2D* result = nullptr;

			while (true) {
				Object2D* next = nullptr;

				for (auto it = parent->Children.rbegin(); it != parent->Children.rend(); ++it) {
					auto obj = dynamic_cast<Object2D*>(*it);
					if (!obj or !obj->Visible) continue;

					if (!obj->pointInObject(mousePosition)) continue;

					if (obj->Active) {
						next = obj;
						break;
					}

					if (!next) {
						next = obj;
					}
				}

				if (!next) break;

				if (next->Active)
					result = next;

				parent = next;
			}

			return result;
		};

		PreviousHigherObject = higherObject;
		higherObject = getTop(&StartInstance);

		if (IsKeyPressed(KEY_F1)) { toggleFPS(&StartInstance); }
		if (IsKeyPressed(KEY_F2)) { debug::toggleDebug(&StartInstance); }
		if (IsKeyPressed(KEY_F3)) { std::cout << accurateFPS << std::endl; }

		DrawFrame(&StartInstance);
	}

	for (int i = 0; i < StartInstance.Children.size();) {
		Instance* child = StartInstance.Children[i];
		Delete(child);
	}

	for (auto it : Fonts) {
		UnloadFont(it.second);
	}

	Fonts.clear();

	CloseWindow();
}