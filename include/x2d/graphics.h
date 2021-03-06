#include "config.h"
#include "graphics/graphicsContext.h"
#include "graphics/blendState.h"
#include "graphics/animation.h"
#include "graphics/spritebatch.h"
#include "graphics/font.h"
#include "graphics/rendertarget.h"
#include "graphics/pixmap.h"
#include "graphics/shader.h"
#include "graphics/shape.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "graphics/textureatlas.h"
#include "graphics/textureregion.h"
#include "graphics/vertex.h"
#include "graphics/vertexbuffer.h"
#include "graphics/viewport.h"

#ifndef X2D_GRAPHICS_H
#define X2D_GRAPHICS_H

#define GL_CHECK_ERROR \
	{ \
		GLenum error; \
		if((error = glGetError()) != GL_NO_ERROR) \
		{ \
			THROW("glGetError() returned 0x%X", error); \
		} \
	}

/*********************************************************************
**	Graphics class [static]											**
**********************************************************************/

BEGIN_XD_NAMESPACE

XDAPI extern uint QUAD_INDICES[6];
XDAPI extern Vector4 QUAD_VERTICES[4];
XDAPI extern Vector2 QUAD_TEXCOORD[4];

class XDAPI Graphics
{
	friend class Engine;
	friend class Window;
	friend class GraphicsContext;
public:

	enum Feature
	{
		VertexBufferObjects,
		FrameBufferObjects
	};

	// Vsync
	static void setVsync(const int mode);
	static int getVsync();

	// Wireframe
	static void enableWireframe();
	static void disableWireframe();

	// Time step & fps
	static double getFPS();

	// Swap buffers
	static void swapBuffers();

protected:
	static void init();
	static void clear();

	static double s_framesPerSecond;

	static ShaderPtr s_defaultShader;
	static Texture2DPtr s_defaultTexture;
	static GLuint s_vao;
	static GLuint s_vbo;
	static GLuint s_ibo;
	static int s_vsync;

	static GraphicsContext s_graphicsContext;
};

END_XD_NAMESPACE

#endif // X2D_GRAPHICS_H