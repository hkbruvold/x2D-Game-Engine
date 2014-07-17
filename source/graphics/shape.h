#ifndef GFX_SHAPE_H
#define GFX_SHAPE_H

#include <x2d/math.h>
#include <x2d/graphics.h>
#include <x2d/config.h>
#include <x2d/base.h>
#include <x2d/graphics/batch.h>
#include <x2d/graphics/texture.h>

class XDAPI Shape
{
	friend class Sprite;
public:
	AS_DECL_REF

	Shape();
	Shape(const Rect &rect);
	Shape(const Vector2 &center, const float radius, const int vertCount = -1);
	Shape(const vector<Vertex> &vertices);
	~Shape();

	//void addVertex(const Vertex &vertex);
	//void addVertices(const vector<Vertex> &vertices);

	void setFillColor(const Vector4 &color);
	void setFillTexture(Texture* texture);
	void setPenColor(const Vector4 &color);
	void setPenSize(const float size);

	void draw(Batch *batch);

	Shape merge(const Shape &shape); // union
	Shape subtract(const Shape &shape);
	Shape intersect(const Shape &shape);

private:
	bool validate();

	Vector4 m_fillColor;
	Texture *m_fillTexture;
	Vector4 m_penColor;
	float m_penSize;
	vector<Vertex> m_vertices;
	vector<uint> m_indices;
	int m_index;
	
	static Shape *Factory() { return new Shape(); }
	static Shape *Factory(const Rect& rect) { return new Shape(rect); }
	static Shape *Factory(const Vector2& center, const float radius, const int vertCount) { return new Shape(center, radius, vertCount); }
	static Shape *Factory(const Array& arr) {

		vector<Vertex> vertices;
		Vertex vert;
		vert.color.set(1.0f, 1.0f, 1.0f, 1.0f);
		vert.texCoord.set(0.0f, 0.0f);
		for(uint i = 0; i < arr.GetSize(); i++) {
			vert.position = *(Vector2*)arr.At(i);
			vertices.push_back(vert);
		}

		return new Shape(vertices);
	}
};

#endif // GFX_SHAPE_H