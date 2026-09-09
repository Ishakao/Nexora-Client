#pragma once
#include "simpleUI.h"
#include "raylib.h"

enum GraphDisplayType {
	GRAPH_LINEAR = 0, // Default linear graph where lines going from i value to i+1 value
	GRAPH_COLUMNAR // Columnar graph style where values displays with rectangles
};

using RAYLIB_FUNCTIONAL::DrawRectangle;

class GraphBuilder : public Object2D {
	constexpr static const char* DefaultName = "GraphBuilder";
	constexpr static InstanceType DefaultClass = GRAPHBUILDER;
	constexpr static float textureAspect = 1.1;

	class GraphSequence {
	public:
		const size_t id = 0;
		std::string name;

		std::vector<long double> sequence;
		long double smallest = 9999999999;
		long double biggest = -9999999999;
		Color color = { 255,0,0,255 };
		int thickness = 3;

		GraphSequence() = delete;
		GraphSequence(size_t id, const std::string& name) : id(id), name(name.empty() ? "Sequence " + std::to_string(id) : name) {}
	};

	size_t seqID = 0;

	RenderTexture2D cachedTexture{};
	Vector2 textureSize{};

	RenderTexture2D cachedMin{};
	Vector2 textureSizeMin{};
	std::string strMin;
	Vector3 textParamsMin;

	RenderTexture2D cachedMax{};
	Vector2 textureSizeMax{};
	std::string strMax;
	Vector3 textParamsMax;

	int ScaleSizeOfY = 4;

	std::unordered_map<size_t, GraphSequence*> sequences;
	bool GraphDirty = false;

	void updateTexture() {
		textureSize = RealSize;
		int quantity = sequences.size();
		Vector2 GraphRealPos = { 0,0 };
		Vector2 GraphRealSize = RealSize;
		if (cachedTexture.id) {
			bool hadClip = !clipStack.empty();
			Clip current;
			if (hadClip) current = clipStack.back();

			if (hadClip) EndScissorMode();

			BeginTextureMode(cachedTexture);
			ClearBackground(BLANK);
			
			size_t gsize = 0;
			for (auto [id, seq] : sequences) {
				if (gsize < seq->sequence.size()) gsize = seq->sequence.size();
			}

			for (auto [id, seq] : sequences) {
				if (seq->sequence.size() < 2) continue;
				if (seq->id != ColumnarDisplayID and GraphType == GraphDisplayType::GRAPH_COLUMNAR) continue;

				long double gmin = minimalGraphValue;
				long double gmax = maximalGraphValue;
				long double lmin = seq->smallest;
				long double lmax = seq->biggest;

				size_t lsize = seq->sequence.size();
				size_t gsizel = gsize;

				if (IndependentValuesY) { gmax = lmax; gmin = lmin; }
				if (IndependentValuesX) { gsizel = lsize; }

				float xAspect = (float)lsize / (float)gsizel;

				for (size_t i = (GraphType == GraphDisplayType::GRAPH_COLUMNAR ? 0 : 1); i < seq->sequence.size(); i++) {
					if (GraphType == GraphDisplayType::GRAPH_LINEAR) {
						long double current = seq->sequence[i];
						long double prev = seq->sequence[i - 1];

						Vector2 start = {
							(GraphRealPos.x + GraphRealSize.x * ((float)(i - 1) / (seq->sequence.size() - 1))) * xAspect,
							GraphRealPos.y + GraphRealSize.y * (1 - (prev - gmin) / (gmax - gmin))
						};
						Vector2 end = {
							(GraphRealPos.x + GraphRealSize.x * ((float)(i) / (seq->sequence.size() - 1))) * xAspect,
							GraphRealPos.y + GraphRealSize.y * (1 - (current - gmin) / (gmax - gmin))
						};

						DrawLineEx(start, end, seq->thickness, seq->color);
					} else if (GraphType == GraphDisplayType::GRAPH_COLUMNAR) {
						long double current = seq->sequence[i];
						float height = GraphRealSize.y * ((current - lmin) / (lmax - lmin)); if (height <= 0) height = 1;
						float sizeAfterSpacingX = (GraphRealSize.x - (seq->sequence.size() - 1) * Spacing) / seq->sequence.size();

						if (sizeAfterSpacingX <= 0) continue;

						Rectangle rec = { (GraphRealPos.x + i * (sizeAfterSpacingX + Spacing)), (GraphRealPos.y + (GraphRealSize.y - height)), sizeAfterSpacingX, height };
						
						if (ColumnsRoundness) {
							DrawRectangleRounded(rec, ColumnsRoundness, 10, seq->color);
						} else {
							DrawRectangle(rec.x, rec.y, rec.width, rec.height, seq->color);
						}
					}
				}
			}

			EndTextureMode();

			if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
		}
	}

	void updateMinTexture(bool load = false) {
		Vector2 leftSize = { SizeOfLeftInfo.Offset + SizeOfLeftInfo.Scale * RealSize.x, RealSize.y / ScaleSizeOfY };
		strMin = textFilterMin(minimalGraphValue);
		textParamsMin = getTextCFrame(strMin.c_str(), getFont(FontFace), { 0,0, leftSize.x, leftSize.y }, TextAnchorEnum::SE, -1, 0);
		textureSizeMin = { leftSize.x - textParamsMin.x, leftSize.y - textParamsMin.y };

		if (load) {
			if (cachedMin.id) UnloadRenderTexture(cachedMin);
			cachedMin = LoadRenderTexture(textureSizeMin.x * textureAspect, textureSizeMin.y * textureAspect);
			SetTextureFilter(cachedMin.texture, TEXTURE_FILTER_TRILINEAR);
		}

		if (cachedMin.id) {
			bool hadClip = !clipStack.empty();
			Clip current;
			if (hadClip) current = clipStack.back();

			if (hadClip) EndScissorMode();

			BeginTextureMode(cachedMin);
			ClearBackground(BLANK);

			DrawTextEx(getFont(FontFace), strMin.c_str(), { 0,0 }, textParamsMin.z, 0, { 255,255,255,255 });

			EndTextureMode();

			if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
		}
	}

	void updateMaxTexture(bool load = false) {
		Vector2 leftSize = { SizeOfLeftInfo.Offset + SizeOfLeftInfo.Scale * RealSize.x, RealSize.y / ScaleSizeOfY };
		strMax = textFilterMax(maximalGraphValue);
		textParamsMax = getTextCFrame(strMax.c_str(), getFont(FontFace), { 0,0, leftSize.x, leftSize.y }, TextAnchorEnum::SE, -1, 0);
		textureSizeMax = { leftSize.x - textParamsMax.x, leftSize.y - textParamsMax.y };

		if (load) {
			if (cachedMax.id) UnloadRenderTexture(cachedMax);
			cachedMax = LoadRenderTexture(textureSizeMax.x * textureAspect, textureSizeMax.y * textureAspect);
			SetTextureFilter(cachedMax.texture, TEXTURE_FILTER_TRILINEAR);
		}

		if (cachedMax.id) {
			bool hadClip = !clipStack.empty();
			Clip current;
			if (hadClip) current = clipStack.back();

			if (hadClip) EndScissorMode();

			BeginTextureMode(cachedMax);
			ClearBackground(BLANK);

			DrawTextEx(getFont(FontFace), strMax.c_str(), { 0,0 }, textParamsMax.z, 0, { 255,255,255,255 });

			EndTextureMode();

			if (hadClip) BeginScissorMode(current.x, current.y, current.w, current.h);
		}
	}

	int lS = 0;
	float lR = 0;
	size_t lC = 0;
	GraphDisplayType lG = GraphDisplayType::GRAPH_LINEAR;
	bool lSMM = false;
	bool lIX = false;
	bool lIY = false;
	bool FilterChanged = true;
	OffsetScale lastLeft = { 0,0 };
	Vector2 lastLeftFull = { 0,0 };
	long double lastMin = 999999999;
	long double lastMax = -999999999;

	long double minimalGraphValue = 999999999;
	long double maximalGraphValue = -999999999;

	std::function<std::string(long double)> textFilterMin = [](long double v) { return std::to_string((long)v); };
	std::function<std::string(long double)> textFilterMax = [](long double v) { return std::to_string((long)v); };

	void updateGlobalMinMax() {
		long double mi = 999999999;
		long double ma = -999999999;

		if (GraphType == GraphDisplayType::GRAPH_LINEAR) {
			for (auto& sec : sequences) {
				if (sec.second->biggest > ma) ma = sec.second->biggest;
				if (sec.second->smallest < mi) mi = sec.second->smallest;
			}
		} else if (GraphType == GraphDisplayType::GRAPH_COLUMNAR) {
			auto it = sequences.find(ColumnarDisplayID);
			if (it != sequences.end()) {
				if (it->second->biggest > ma) ma = it->second->biggest;
				if (it->second->smallest < mi) mi = it->second->smallest;
			} else return;
		}

		minimalGraphValue = mi;
		maximalGraphValue = ma;
	}
public:
	void setSequenceColor(size_t id, Color c) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			if (it->second->color.r != c.r or it->second->color.g != c.g or it->second->color.b != c.b or it->second->color.a != c.a) GraphDirty = true;
			it->second->color = c;
		}
	}

	void setSequenceThickness(size_t id, int t) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			if (t != it->second->thickness) GraphDirty = true;
			it->second->thickness = t;
		}
	}

	size_t createSequence(const std::string& name="") {
		GraphSequence* s = new GraphSequence(seqID++, name);
		sequences.insert({ s->id, s });
		return s->id;
	}

	void removeSequence(size_t id) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			if (it->second->sequence.size() > 2) GraphDirty = true;
			delete it->second;
			sequences.erase(it);
		}
	}

	void addValueToSequence(size_t id, long double value) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			auto seq = it->second;
			if (seq) {
				seq->sequence.push_back(value);
				GraphDirty = true;

				if (value > seq->biggest) seq->biggest = value;
				if (value < seq->smallest) seq->smallest = value;

				if (value > maximalGraphValue) maximalGraphValue = value;
				if (value < minimalGraphValue) minimalGraphValue = value;
			}
		}
	}

	void removeValueFromSequence(size_t id, long double value) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			auto seq = it->second;
			if (seq) {
				auto it2 = std::find(seq->sequence.begin(), seq->sequence.end(), value);
				if (it2 != seq->sequence.end()) {
					seq->sequence.erase(it2);
					GraphDirty = true;

					if (value == seq->biggest or value == seq->smallest) {
						long double min = seq->sequence.size() > 0 ? seq->sequence[0] : 9999999999;
						long double max = seq->sequence.size() > 0 ? seq->sequence[0] : -9999999999;

						for (long double obj : seq->sequence) {
							if (obj < min) min = obj;
							if (obj > max) max = obj;
						}

						seq->biggest = max;
						seq->smallest = min;
					}
				}
			}
		}
	}

	void removeIndexFromSequence(size_t id, size_t index) {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			auto seq = it->second;
			if (seq and seq->sequence.size() > index) {
				auto it2 = seq->sequence.begin() + index;
				long double value = *it2;
				seq->sequence.erase(it2);
				GraphDirty = true;

				if (value == seq->biggest or value == seq->smallest) {
					long double min = seq->sequence.size() > 0 ? seq->sequence[0] : 9999999999;
					long double max = seq->sequence.size() > 0 ? seq->sequence[0] : -9999999999;

					for (long double obj : seq->sequence) {
						if (obj < min) min = obj;
						if (obj > max) max = obj;
					}

					seq->biggest = max;
					seq->smallest = min;
				}
			}
		}
	}

	long double getMaxValue(size_t seqID) {
		auto it = sequences.find(seqID);
		if (it != sequences.end()) {
			return it->second->biggest;
		}

		return 0;
	}

	long double getMinValue(size_t seqID) {
		auto it = sequences.find(seqID);
		if (it != sequences.end()) {
			return it->second->smallest;
		}

		return 0;
	}

	Color getColor(size_t seqID) {
		auto it = sequences.find(seqID);
		if (it != sequences.end()) {
			return it->second->color;
		}

		return { 255,255,255,255 };
	}

	int getThickness(size_t seqID) {
		auto it = sequences.find(seqID);
		if (it != sequences.end()) {
			return it->second->thickness;
		}

		return 0;
	}

	const std::vector<long double>& getSequenceValues(size_t id) const {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			return it->second->sequence;
		}

		SIMPLEUI_THROW(std::string("Sequence with id ") + std::to_string(id) + " was not found");
	}

	void minimalValueToTextFunction(std::function<std::string(long double)> f) {
		textFilterMin = f;
		FilterChanged = true;
	}

	void maximalValueToTextFunction(std::function<std::string(long double)> f) {
		textFilterMax = f;
		FilterChanged = true;
	}

	GraphDisplayType GraphType = GraphDisplayType::GRAPH_LINEAR;
	int Spacing = 0; // Spacing between rectangles in GRAPH_COLUMNAR graph
	float ColumnsRoundness = 0; // Roundness of rectangles in GRAPH_COLUMNAR graph (0-1)
	size_t ColumnarDisplayID = 0; // ID of sequence which will be shown on graph (only on columnar graph)

	OffsetScale SizeOfLeftInfo = { 0, 0 }; // Size by x { offset, scale } of minimal and maximal values on graph. Set {0, 0} or leave it default to not display values
	SUI_Text FontFace = BASIC_FONT_NAME; // FontFace of left info values
	Color TextColor = { 255,255,255,255 }; // Color of left info values;
	bool AutoColorForColumnar = true; // Color of left info will be with current displayed graph (columnar only)
	bool IndependentValuesX = false; // true means a same X size for all graphs (even if the max/min values are different) | false means a X size of graph will depend on aspect from maximal values quantities
	bool IndependentValuesY = false; // true means a same Y size for all graphs (even if the max/min values are different) | false means a Y size of graph will depend on aspect from maximal global value

	void Draw() override {
		if (Visible) {
			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			Object2D::Draw();

			if (GraphType != lG) {
				updateGlobalMinMax();
			}

			bool conditionToUpdate = (GraphDirty) or (Spacing != lS) or (ColumnsRoundness != lR) or (ColumnarDisplayID != lC) or (GraphType != lG) or (lIX != IndependentValuesX) or (lIY != IndependentValuesY);
			lS = Spacing;
			lR = ColumnsRoundness;
			lC = ColumnarDisplayID;
			lG = GraphType;
			lIX = IndependentValuesX;
			lIY = IndependentValuesY;

			if (GraphDirty) updateGlobalMinMax();

			if (cachedTexture.id and (cachedTexture.texture.width < RealSize.x or cachedTexture.texture.height < RealSize.y)) {
				UnloadRenderTexture(cachedTexture);
				cachedTexture = LoadRenderTexture(RealSize.x * textureAspect, RealSize.y * textureAspect);
				SetTextureFilter(cachedTexture.texture, TEXTURE_FILTER_TRILINEAR);
				updateTexture();
			} else if (!cachedTexture.id) {
				cachedTexture = LoadRenderTexture(RealSize.x * textureAspect, RealSize.y * textureAspect);
				SetTextureFilter(cachedTexture.texture, TEXTURE_FILTER_TRILINEAR);
				updateTexture();
			} else if (conditionToUpdate) {
				updateTexture();
			}

			Vector2 leftSizeFull = { SizeOfLeftInfo.Offset + SizeOfLeftInfo.Scale * RealSize.x, RealSize.y / 4 };
			bool bothCondition = FilterChanged or (lastLeftFull.x != leftSizeFull.x or lastLeftFull.y != leftSizeFull.y) or FontFace.isChanged() or lastLeft.Offset != SizeOfLeftInfo.Offset or lastLeft.Scale != SizeOfLeftInfo.Scale;
			bool conditionToUpdateMin = lastMin != minimalGraphValue or bothCondition;
			bool conditionToUpdateMax = lastMax != maximalGraphValue or bothCondition;

			FilterChanged = false;

			if (!(SizeOfLeftInfo.Scale == 0 and SizeOfLeftInfo.Offset == 0)) {
				if (!cachedMin.id or (cachedMin.texture.width < textureSizeMin.x or cachedMin.texture.height < textureSizeMin.y)) {
					updateMinTexture(true);
				} else if (conditionToUpdateMin) {
					updateMinTexture();
				}
			}

			if (!(SizeOfLeftInfo.Scale == 0 and SizeOfLeftInfo.Offset == 0)) {
				if (!cachedMax.id or (cachedMax.texture.width < textureSizeMax.x or cachedMax.texture.height < textureSizeMax.y)) {
					updateMaxTexture(true);
				} else if (conditionToUpdateMax) {
					updateMaxTexture();
				}
			}

			FontFace.restate();
			GraphDirty = false;
			lastLeft = SizeOfLeftInfo;
			lastLeftFull = leftSizeFull;
			lastMin = minimalGraphValue;
			lastMax = maximalGraphValue;

			/* I will add padding in future. Work piece of GraphBuilder
			
			Vector2 start = {
				GraphRealPos.x + GraphPadding.left.Offset + GraphPadding.left.Scale * GraphRealSize.x + (GraphRealSize.x - GraphPadding.right.Offset - GraphPadding.left.Offset - GraphPadding.left.Scale * GraphRealSize.x - GraphPadding.right.Scale * GraphRealSize.x) * ((float)(i - 1) / (seq->sequence.size() - 1)),
				GraphRealPos.y + GraphPadding.upper.Offset + GraphPadding.upper.Scale * GraphRealSize.y + (GraphRealSize.y - GraphPadding.lower.Offset - GraphPadding.upper.Offset - GraphPadding.upper.Scale * GraphRealSize.y - GraphPadding.lower.Scale * GraphRealSize.y) * (1 - ((prev - min) / (max - min)))
			};
			Vector2 end = {
				GraphRealPos.x + GraphPadding.left.Offset + GraphPadding.left.Scale * GraphRealSize.x + (GraphRealSize.x - GraphPadding.right.Offset - GraphPadding.left.Offset - GraphPadding.left.Scale * GraphRealSize.x - GraphPadding.right.Scale * GraphRealSize.x) * ((float)i / (seq->sequence.size() - 1)),
				GraphRealPos.y + GraphPadding.upper.Offset + GraphPadding.upper.Scale * GraphRealSize.y + (GraphRealSize.y - GraphPadding.lower.Offset - GraphPadding.upper.Offset - GraphPadding.upper.Scale * GraphRealSize.y - GraphPadding.lower.Scale * GraphRealSize.y) * (1 - ((current - min) / (max - min)))
			};

			*/

			Rectangle sourceRec = { 0.0f, (float)(cachedTexture.texture.height - textureSize.y), (float)textureSize.x, -(float)textureSize.y };
			Rectangle destRec = { RealPos.x + leftSizeFull.x, RealPos.y, (float)RealSize.x - leftSizeFull.x, (float)RealSize.y };

			Rectangle sourceRecMin = { 0.0f, (float)(cachedMin.texture.height - textureSizeMin.y), (float)textureSizeMin.x, -(float)textureSizeMin.y };
			Rectangle destRecMin = { RealPos.x + textParamsMin.x, RealPos.y + RealSize.y * 0.75 + textParamsMin.y, textureSizeMin.x, textureSizeMin.y };

			Rectangle sourceRecMax = { 0.0f, (float)(cachedMax.texture.height - textureSizeMax.y), (float)textureSizeMax.x, -(float)textureSizeMax.y };
			Rectangle destRecMax = { RealPos.x + textParamsMax.x, RealPos.y + textParamsMax.y, textureSizeMax.x, textureSizeMax.y };

			Color c = TextColor;

			if (AutoColorForColumnar and GraphType == GraphDisplayType::GRAPH_COLUMNAR) {
				auto it = sequences.find(ColumnarDisplayID);
				if (it != sequences.end()) {
					c = it->second->color;
				}
			}

			DrawTexturePro(cachedTexture.texture, sourceRec, destRec, { 0,0 }, 0, { 255,255,255,255 });
			if (!(SizeOfLeftInfo.Scale == 0 and SizeOfLeftInfo.Offset == 0)) {
				DrawTexturePro(cachedMin.texture, sourceRecMin, destRecMin, { 0,0 }, 0, c);
				DrawTexturePro(cachedMax.texture, sourceRecMax, destRecMax, { 0,0 }, 0, c);
			}
		}
	}

	GraphBuilder* Clone() const override {
		GraphBuilder* i = new GraphBuilder(*this);
		i->Parent = nullptr;
		i->Children.clear();
		i->cachedTexture.id = 0;
		i->cachedMin.id = 0;
		i->cachedMax.id = 0;
		for (Instance* c : Children) {
			c->Clone()->setParent(i);
		}

		return i;
	}

	GraphBuilder(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	GraphBuilder(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }
	~GraphBuilder() {
		if (cachedTexture.id != 0) {
			UnloadRenderTexture(cachedTexture);
		}

		if (cachedMin.id != 0) {
			UnloadRenderTexture(cachedMin);
		}

		if (cachedMax.id != 0) {
			UnloadRenderTexture(cachedMax);
		}
	}
	GraphBuilder() = delete;
};

class ToggleSwitcher : public Object2D {
	constexpr static const char* DefaultName = "ToggleSwitcher";
	constexpr static InstanceType DefaultClass = TOGGLESWITCHER;

	bool Value = false;
	float currentSliderPos = 0;
	std::function<void(bool)> func = [](bool _) {};

	void _setValue(bool v) {
		Value = v;
		func(Value);
		if (AnimationSpeed) {
			Animate::Create(&currentSliderPos, AnimationSpeed, (Value ? 1 : 0), AnimationFunction, AnimationEase);
		} else {
			currentSliderPos = (Value ? 1 : 0);
		}
	}

	void checkClick() {
		if (Enabled and Active and IsMouseButtonPressed(ButtonType) and pointInObject(mousePosition)) {
			bool canBePressed = (
				EnterEventCondition == SUI_EEC::EEC_DEFAULT ? this == higherObject :
				(EnterEventCondition == SUI_EEC::EEC_EVERY_ENTER ? true :
					EnterEventCondition == SUI_EEC::EEC_IF_DESCENDANT_HIGHER ? ((higherObject == this and higherObject != nullptr) or (higherObject and higherObject != this and higherObject->isDescendantOf(this))) : false)
				);

			if (canBePressed) {
				if (ClickOnSlider) {
					float RealBallPosX = RealPos.x + (RealSize.x - RealSize.y) * currentSliderPos;
					Rectangle rec = { RealBallPosX + SliderBorderThickness, RealPos.y + SliderBorderThickness, RealSize.y - SliderBorderThickness * 2, RealSize.y - SliderBorderThickness * 2 };

					float size = RealSize.y - SliderBorderThickness * 2.0f;
					float halfSize = size / 2.0f;
					float r = Roundness * halfSize;

					float cx = RealBallPosX + SliderBorderThickness + halfSize;
					float cy = RealPos.y + SliderBorderThickness + halfSize;

					float dx = std::abs(mousePosition.x - cx);
					float dy = std::abs(mousePosition.y - cy);

					float diffX = dx - halfSize + r;
					float diffY = dy - halfSize + r;

					bool isHovered = (dx <= halfSize and dy <= halfSize) and ((diffX <= 0.0f or diffY <= 0.0f) or (diffX * diffX + diffY * diffY <= r * r));

					if (!isHovered) return;
				}

				_setValue(!Value);
			}
		}
	}
public:
	float AnimationSpeed = 0.3; // 0 means instant toggle
	bool Enabled = true; // Allows user to click
	bool ClickOnSlider = false; // true means click will toggle value only if click on slider | false means any on-object click will toggle value
	Animate::Function AnimationFunction = Animate::Linear;
	Animate::Ease AnimationEase = Animate::In;
	MouseButtonType ButtonType = MouseButtonType::MOUSE_LEFT; // Which button click will toggle switcher
	Color SliderColor = { 130,130,130,255 };
	Color SliderBorderColor = { 0,0,0,255 };
	float SliderBorderTransparency = 0;
	float SliderTransparency = 0;
	int SliderBorderThickness = 0;

	bool getValue() const {
		return Value;
	}

	void setValue(bool v) {
		_setValue(v);
	}

	void onToggle(std::function<void(bool)> f) {
		func = f;
	}

	void Draw() override {
		if (Visible) {
			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			Object2D::Draw();

			float RealBallPosX = RealPos.x + (RealSize.x - RealSize.y) * currentSliderPos;
			DrawRectangleRounded({ RealBallPosX + SliderBorderThickness, RealPos.y + SliderBorderThickness, RealSize.y - SliderBorderThickness * 2, RealSize.y - SliderBorderThickness * 2 }, Roundness, Segments, { SliderColor.r, SliderColor.g, SliderColor.b, (unsigned char)(SliderColor.a * (1 - SliderTransparency)) });
			
			if (SliderBorderThickness) {
				DrawRectangleRoundedLinesEx({ RealBallPosX + SliderBorderThickness, RealPos.y + SliderBorderThickness, RealSize.y - SliderBorderThickness * 2, RealSize.y - SliderBorderThickness * 2 }, Roundness, Segments, SliderBorderThickness,{ SliderBorderColor.r, SliderBorderColor.g, SliderBorderColor.b, (unsigned char)(SliderBorderColor.a * (1 - SliderBorderTransparency)) });
			}
		}
	}

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

		eventHandler();
		getRealObject2Dsize();
		getRealObject2Dposition();

		checkClick();

		Draw();

		for (int i = 0; i < Children.size(); i++) {
			Instance* child = Children[i];
			child->Update();
		}
	}
	
	ToggleSwitcher(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; Roundness = 1; Active = true; };
	ToggleSwitcher(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; Roundness = 1; Active = true; };
	~ToggleSwitcher() {

	}
	ToggleSwitcher() = delete;
};
