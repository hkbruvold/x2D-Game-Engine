//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "textureRegion.h"
#include "texture.h"

AS_REGISTER(TextureRegion)

int TextureRegion::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	AS_REGISTER_FACTORY(TextureRegion, "const Texture @", (const Texture *))
	AS_REGISTER_FACTORY(TextureRegion, "const Texture @, const Vector2 &in, const Vector2 &in", (const Texture *, const Vector2&, const Vector2&))
	AS_REGISTER_FACTORY(TextureRegion, "const Texture @, const float, const float, const float, const float", (const Texture *, const float u0, const float v0, const float u1, const float v1))
	
	AS_REGISTER_MEMBER(TextureRegion, "Vector2 uv0", uv0)
	AS_REGISTER_MEMBER(TextureRegion, "Vector2 uv1", uv1)

	AS_REGISTER_METHOD(TextureRegion, "void setRegion(const Vector2 &in, const Vector2 &in)", setRegion, (const Vector2&, const Vector2&), void)
	AS_REGISTER_METHOD(TextureRegion, "void setRegion(const float, const float, const float, const float)", setRegion, (const float, const float, const float, const float), void)
	AS_REGISTER_METHOD(TextureRegion, "const Texture @getTexture() const", getTexture, () const, const Texture*)
	AS_REGISTER_METHOD(TextureRegion, "void @setTexture(const Texture @) const", setTexture, (const Texture*), void)

	return r;
}

TextureRegion::TextureRegion(const Texture *texture) :
	refCounter(this),
	uv0(0.0f, 0.0f),
	uv1(1.0f, 1.0f),
	texture(texture)
{
}

TextureRegion::TextureRegion(const Texture *texture, const Vector2 &uv0, const Vector2 &uv1) :
	refCounter(this),
	uv0(uv0),
	uv1(uv1),
	texture(texture)
{
}

TextureRegion::TextureRegion(const Texture *texture, const float u0, const float v0, const float u1, const float v1) :
	refCounter(this),
	uv0(u0, v0),
	uv1(u1, v1),
	texture(texture)
{
}

void TextureRegion::setTexture(const Texture *texture)
{
	this->texture = texture;
}

const Texture *TextureRegion::getTexture() const
{
	return texture;
}

Vector2i TextureRegion::getSize() const
{
	return Vector2i(int(texture->getWidth()*uv1.x - texture->getWidth()*uv0.x),
		int(texture->getHeight()*uv1.y - texture->getHeight()*uv0.y));
}
	
void TextureRegion::setRegion(const Vector2 &uv0, const Vector2 &uv1)
{
	this->uv0 = uv0;
	this->uv1 = uv1;
}

void TextureRegion::setRegion(const float u0, const float v0, const float u1, const float v1)
{
	uv0.set(u0, v0);
	uv1.set(u1, v1);
}