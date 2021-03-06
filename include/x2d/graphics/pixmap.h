#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

BEGIN_XD_NAMESPACE

class XDAPI PixelFormat
{
public:
	enum Components
	{
		R,
		RG,
		RGB,
		RGBA
	};

	enum DataType
	{
		INT,
		UNSIGNED_INT,
		BYTE,
		UNSIGNED_BYTE,
		FLOAT
	};

	PixelFormat(Components components = RGBA, DataType dataType = UNSIGNED_BYTE)
	{
		m_components = components;
		m_dataType = dataType;
	}

	Components getComponents() const { return m_components; }
	DataType getDataType() const { return m_dataType; }

	uint getComponentCount() const;
	uint getDataTypeSizeInBytes() const;
	uint getPixelSizeInBytes() const;

private:
	Components m_components;
	DataType m_dataType;
};

class XDAPI Pixmap
{
public:

public:
	Pixmap(const PixelFormat &format = PixelFormat());
	Pixmap(const uint width, const uint height, const PixelFormat &format = PixelFormat());
	Pixmap(const uint width, const uint height, const void *data, const PixelFormat &format = PixelFormat());
	Pixmap(const Pixmap& other);
	Pixmap(const string &imageFile, const bool premultiplyAlpha = false);
	~Pixmap();

	Pixmap &operator=(Pixmap &other);

	uint getWidth() const;
	uint getHeight() const;
	PixelFormat getFormat() const;

	void getPixel(const uint x, const uint y, void *data) const;
	void setPixel(const uint x, const uint y, const void *data);

	void fill(const void *data);
	void clear();

	void exportToFile(string path) const;

	const uchar *getData() const;

private:
	uchar *m_data;
	uint m_width;
	uint m_height;
	PixelFormat m_format;
};

END_XD_NAMESPACE

#endif // X2D_PIXMAP_H