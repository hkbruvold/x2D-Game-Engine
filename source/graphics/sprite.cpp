//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

BEGIN_XD_NAMESPACE

Sprite::Sprite(const Texture2DPtr texture, const Rect &rectangle, const Vector2 &origin, const float angle, const TextureRegion &region, const Color &color, const float depth, const Vector2 scale) :
	m_texture(texture),
	m_textureRegion(region),
	m_position(rectangle.position),
	m_size(rectangle.size),
	m_origin(origin),
	m_angle(angle),
	m_color(color),
	m_depth(depth),
	m_scale(scale)
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(const Vector2 &pos)
{
	m_position = pos;
}

void Sprite::setPosition(const float x, const float y)
{
	m_position.set(x, y);
}

void Sprite::setX(const float x)
{
	m_position.x = x;
}

void Sprite::setY(const float y)
{
	m_position.y = y;
}

void Sprite::setSize(const Vector2 &size)
{
	m_size = size;
}

void Sprite::setSize(const float w, const float h)
{
	m_size.set(w, h);
}

void Sprite::setWidth(const float w)
{
	m_size.x = w;
}

void Sprite::setHeight(const float h)
{
	m_size.y = h;
}

void Sprite::setOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void Sprite::setRotation(const float ang)
{
	m_angle = ang;
}

void Sprite::setRegion(const TextureRegion &textureRegion, const bool resize)
{
	m_textureRegion = textureRegion;
	if(resize)
	{
		m_size = m_texture != 0 ? Vector2i(
			int(m_texture->getWidth()*m_textureRegion.uv1.x - m_texture->getWidth()*m_textureRegion.uv0.x),
			int(m_texture->getHeight()*m_textureRegion.uv1.y - m_texture->getHeight()*m_textureRegion.uv0.y)
			) : Vector2i(0);
	}
}

void Sprite::setColor(const Color &color)
{
	m_color = color;
}

void Sprite::setDepth(const float depth)
{
	m_depth = depth;
}

float Sprite::getDepth() const
{
	return m_depth;
}

void Sprite::move(const Vector2 &dt)
{
	m_position += dt;
}

void Sprite::rotate(const float ang)
{
	m_angle += ang;
}

void Sprite::scale(const float scl)
{
	m_size *= scl;
}

void Sprite::getAABB(Vector2 *points) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.scale(m_scale.x, m_scale.y, 1.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x + m_origin.x, m_position.y + m_origin.y, 0.0f);

	for(int i = 0; i < 4; i++)
	{
		points[i] = (mat * QUAD_VERTICES[i]).getXY();
	}
}

Vector2 Sprite::getPosition() const
{
	return m_position;
}

float Sprite::getX() const
{
	return m_position.x;
}

float Sprite::getY() const
{
	return m_position.y;
}

Vector2 Sprite::getSize() const
{
	return m_size;
}

float Sprite::getWidth() const
{
	return m_size.x;
}

float Sprite::getHeight() const
{
	return m_size.y;
}

Vector2 Sprite::getOrigin() const
{
	return m_origin;
}

Vector2 Sprite::getCenter() const
{
	return m_position + (m_size/2.0f);
}

float Sprite::getRotation() const
{
	return m_angle;
}

Color Sprite::getColor() const
{
	return m_color;
}

TextureRegion Sprite::getRegion() const
{
	return m_textureRegion;
}

Texture2DPtr Sprite::getTexture() const
{
	return m_texture;
}

void Sprite::getVertices(Vertex *vertices, uint *indices, const uint indexOffset) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.scale(m_scale.x, m_scale.y, 1.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x + m_origin.x, m_position.y + m_origin.y, 0.0f);

	for(int i = 0; i < 4; i++)
	{
		Vector2 pos = (mat * QUAD_VERTICES[i]).getXY();
		vertices[i].set4f(VERTEX_POSITION, pos.x, pos.y);
		vertices[i].set4ub(VERTEX_COLOR, m_color.r, m_color.g, m_color.b, m_color.a);
	}

	vertices[0].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv1.y);
	vertices[1].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv1.y);
	vertices[2].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv0.y);
	vertices[3].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv0.y);
	
	indices[0] = indexOffset + QUAD_INDICES[0];
	indices[1] = indexOffset + QUAD_INDICES[1];
	indices[2] = indexOffset + QUAD_INDICES[2];
	indices[3] = indexOffset + QUAD_INDICES[3];
	indices[4] = indexOffset + QUAD_INDICES[4];
	indices[5] = indexOffset + QUAD_INDICES[5];
}

END_XD_NAMESPACE