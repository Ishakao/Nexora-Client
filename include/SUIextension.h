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
	constexpr static float textureAspect = 1.1f;

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
			
			for (auto [id, seq] : sequences) {
				if (seq->sequence.size() < 2) continue;
				if (seq->id != ColumnarDisplayID and GraphType == GraphDisplayType::GRAPH_COLUMNAR) continue;

				long double min = seq->smallest;
				long double max = seq->biggest;
				for (size_t i = (GraphType == GraphDisplayType::GRAPH_COLUMNAR ? 0 : 1); i < seq->sequence.size(); i++) {
					if (GraphType == GraphDisplayType::GRAPH_LINEAR) {
						long double current = seq->sequence[i];
						long double prev = seq->sequence[i - 1];

						Vector2 start = {
							GraphRealPos.x + GraphRealSize.x * ((float)(i - 1) / (seq->sequence.size() - 1)),
							GraphRealPos.y + GraphRealSize.y * (1 - (prev - min) / (max - min))
						};
						Vector2 end = {
							GraphRealPos.x + GraphRealSize.x * ((float)(i) / (seq->sequence.size() - 1)),
							GraphRealPos.y + GraphRealSize.y * (1 - (current - min) / (max - min))
						};

						DrawLineEx(start, end, seq->thickness, seq->color);
					} else if (GraphType == GraphDisplayType::GRAPH_COLUMNAR) {
						long double current = seq->sequence[i];
						float height = GraphRealSize.y * ((current - min) / (max - min)); if (height <= 0) height = 1;
						float sizeAfterSpacingX = (GraphRealSize.x - (seq->sequence.size() - 1) * Spacing) / seq->sequence.size();

						if (sizeAfterSpacingX <= 0) continue;

						Rectangle rec = { (GraphRealPos.x + i * (sizeAfterSpacingX + Spacing)), (GraphRealPos.y + (GraphRealSize.y - height)), sizeAfterSpacingX, height };
						
						if (Roundness) {
							DrawRectangleRounded(rec, Roundness, 10, seq->color);
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

	int lS = 0;
	float lR = 0;
	size_t lC = 0;
	GraphDisplayType lG = GraphDisplayType::GRAPH_LINEAR;
	bool lSMM = false;
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

	std::vector<long double> getSequenceValues(size_t id) const {
		auto it = sequences.find(id);
		if (it != sequences.end()) {
			return it->second->sequence;
		}

		return {};
	}

	GraphDisplayType GraphType = GraphDisplayType::GRAPH_LINEAR;
	int Spacing = 0; // Spacing between rectangles in GRAPH_COLUMNAR graph
	float Roundness = 0; // Roundness of rectangles in GRAPH_COLUMNAR graph (0-1)
	size_t ColumnarDisplayID = 0; // ID of sequence which will be shown on graph (only on columnar graph)
	bool ShowMinMax = false;

	void Draw() override {
		if (Visible) {
			if (RealPos.x + RealSize.x + BorderThickness < 0
				or RealPos.x - RealSize.x - BorderThickness > winWidth
				or RealPos.y + RealSize.y + BorderThickness < 0
				or RealPos.y - RealSize.y - BorderThickness > winHeight) {
				return;
			}

			Object2D::Draw();

			bool conditionToUpdate = (GraphDirty) or (Spacing != lS) or (Roundness != lR) or (ColumnarDisplayID != lC) or (GraphType != lG) or (ShowMinMax != lSMM);
			lS = Spacing;
			lR = Roundness;
			lC = ColumnarDisplayID;
			lG = GraphType;
			lSMM = ShowMinMax;

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

			GraphDirty = false;

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
			Rectangle destRec = { RealPos.x, RealPos.y, (float)RealSize.x, (float)RealSize.y };

			DrawTexturePro(cachedTexture.texture, sourceRec, destRec, { 0,0 }, 0, { 255,255,255,255 });
		}
	}

	GraphBuilder(bool a) : Object2D(a) { Name = DefaultName; Class = DefaultClass; };
	GraphBuilder(Instance* p) : Object2D(p) { Name = DefaultName; Class = DefaultClass; }
	~GraphBuilder() {
		if (cachedTexture.id != 0) {
			UnloadRenderTexture(cachedTexture);
		}
	}
	GraphBuilder() = delete;
};