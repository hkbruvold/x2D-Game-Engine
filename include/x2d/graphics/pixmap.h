#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

class XDAPI XPixmap
{
public:
	XPixmap();
	XPixmap(const int width, const int height);
	XPixmap(const int width, const int height, const Vector4 *pixels);
	XPixmap(const XPixmap& other);
	~XPixmap();

	int getWidth() const;
	int getHeight() const;

	Vector4 getColor(const int x, const int y) const;
	void setColor(const int x, const int y, const Vector4 &color);

	void fill(const Vector4 &color);
	void clear();

	void exportToFile(const string &path) const;

	const float *getData() const;

	//Pixmap &operator=(Pixmap &other);

private:
	Vector4 *m_data;
	int m_width;
	int m_height;
};

#endif // X2D_PIXMAP_H