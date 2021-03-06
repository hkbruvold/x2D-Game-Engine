#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "../engine.h"
#include "shader.h"
#include "blendState.h"
#include "textureregion.h"

BEGIN_XD_NAMESPACE

class RenderTarget2D;
class Vertex;
class VertexBuffer;
class IndexBuffer;

/**
 * \brief Handles primitive rendering to the screen.
 */
class XDAPI GraphicsContext
{
	friend class Graphics;
public:

	/**
	 * Graphics capabilites.
	 * For enabling and disabling certain rendering options.
	 */
	enum Capability
	{
		BLEND = GL_BLEND,						///< Back buffer blending
		DEPTH_TEST = GL_DEPTH_TEST,				///< Depth testing
		SCISSOR_TEST = GL_SCISSOR_TEST,			///< Scissor testing
		LINE_SMOOTH = GL_LINE_SMOOTH,			///< Smooth lines
		POLYGON_SMOOTH = GL_POLYGON_SMOOTH,		///< Smooth polygons
		MULTISAMPLE = GL_MULTISAMPLE,			///< Multisample?
		TEXTURE_1D = GL_TEXTURE_1D,				///< 1D textures
		TEXTURE_2D = GL_TEXTURE_2D,				///< 2D textures
		TEXTURE_3D = GL_TEXTURE_3D				///< 3D textures
	};

	/**
	 * Primitive types.
	 * For rendering different kinds of primitives.
	 */
	enum PrimitiveType
	{
		PRIMITIVE_POINTS = GL_POINTS,					///< Points. 1 vertex per primitive.
		PRIMITIVE_LINES = GL_LINES,						///< Lines. 2 vertex per primitive.
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,				///< Triangles. 3 vertex per primitive.
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,	///< Triangle strip. 3 vertex for the first primitive, 1 vertex for the next.
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN		///< Triangle fan. 3 vertex for the first primitive, 1 vertex for the next.
	};

	/**
	 * Clearing buffer mask.
	 * Used for selecting how to clear the backbuffer.
	 */
	enum BufferMask
	{
		COLOR_BUFFER = GL_COLOR_BUFFER_BIT,		///< %Color buffer
		DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,		///< Depth buffer
		STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT	///< Stencil buffer
	};

	/**
	 * Enables the select capability.
	 * \param cap Capability to enable.
	 */
	void enable(const Capability cap);

	/**
	 * Disables the select capability.
	 * \param cap Capability to disable.
	 */
	void disable(const Capability cap);

	/**
	 * Clears the back buffer using \p mask.
	 * \param mask Decides what channels in the back buffer to clear.
	 * \param fillColor Decides what value to clear to.
	 */
	void clear(const uint mask, const Color &fillColor = Color(0, 0, 0, 0));

	/**
	 * Sets a render target. This means that everything drawn beyond this point
	 * will be rendered to the given \p renderTarget instead of the screen.
	 * \param renderTarget The target buffer to render to.
	 */
	void setRenderTarget(RenderTarget2D *renderTarget);

	/**
	 * Get current render target.
	 */
	RenderTarget2D *getRenderTarget() const
	{
		return m_renderTarget;
	}
	
	/**
	 * Sets the model-view matrix. This means that every vertex drawn after this
	 * will be multiplied by \p mat.
	 * \param mat The 4x4 matrix to be multiplied with every vertex.
	 */
	void setModelViewMatrix(const Matrix4 &mat);

	/**
	 * Gets the current model-view matrix
	 */
	Matrix4 getModelViewMatrix() const;

	/**
	 * Pushes a matrix onto the model-view matrix stack.
	 * The matrix is pushed as follows \code stack.push_back(stack.top() * mat) \endcode
	 * \param mat Matrix to push on the stack.
	 */
	void pushMatrix(const Matrix4 &mat);

	/**
	 * Pops the top matrix of the model-view matrix stack.
	 */
	void popMatrix();
	
	/**
	 * Set texture. Every primitive rendered after this will have \p texture
	 * applied to it.
	 * \param texture Texture to apply to the primitives.
	 */
	void setTexture(const Texture2DPtr texture);

	/**
	 * Gets the current texture.
	 */
	Texture2DPtr getTexture() const;

	/**
	 * Set shader. Every vertex and fragment rendered after this will
	 * have the effect of \p shader applied to them.
	 * \param shader Shader to render the primitves with.
	 */
	void setShader(const ShaderPtr shader);

	/**
	 * Returns the current shader.
	 */
	ShaderPtr getShader() const;

	/**
	 * Set blend state. Every pixel rendered after this will use a 
	 * formula defined by \p blendState to blend new pixels with the back buffer.
	 * \param blendState The blend state to render with.
	 */
	void setBlendState(const BlendState &blendState);

	/**
	 * Returns the current blend state.
	 */
	BlendState getBlendState();

	/**
	 * Saves a screen shot of the back buffer to \p path as a PNG file.
	 * \param path Screen shot destination path
	 */
	void saveScreenshot(string path);

	/**
	 * Resizes the viewport (the area of the screen rendered to).
	 * \param w Width of the viewport in pixels
	 * \param h Height of the viewport in pixels
	 */
	void resizeViewport(const uint w, const uint h);

	/**
	 * Returns the width of the viewport.
	 */
	uint getWidth() const
	{
		return m_width;
	}

	/**
	 * Returns the height of the viewport.
	 */
	uint getHeight() const
	{
		return m_height;
	}

	/**
	 * Renders an indexed primitive to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 * \param indices Array of indices.
	 * \param indexCount Number of indices.
	 */
	void drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount);

	/**
	 * Renders an indexed primitive to the screen using vertex and index buffers.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 * \param ibo Index buffer object.
	 */
	void drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo);

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 */
	void drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount);

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 */
	void drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo);

	/**
	 * Renders a rectangle.
	 * \param rect Rectangle to render.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Rect &rect, const Color &color = Color(255), const TextureRegion &textureRegion = TextureRegion());


	/**
	 * Renders a rectangle.
	 * \param pos Top-left corner of the rectangle.
	 * \param size Size of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Vector2 &pos, const Vector2 &size, const Color &color = Color(255), const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a rectangle.
	 * \param x Left x position of the rectangle.
	 * \param y Top y position of the rectangle.
	 * \param width Width of the rectangle.
	 * \param height Height of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const float x, const float y, const float width, const float height, const Color &color = Color(255), const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a circle.
	 * \param pos Center of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircle(const Vector2 &pos, const float radius, const uint segments, const Color &color = Color(255));

	/**
	 * Renders a circle.
	 * \param x Center x position of the circle.
	 * \param y Center y position of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircle(const float x, const float y, const float radius, const uint segments, const Color &color = Color(255));

private:
	GraphicsContext();
	void setupContext();

	uint m_width;
	uint m_height;
	Texture2DPtr m_texture;
	ShaderPtr m_shader;
	BlendState m_blendState;
	RenderTarget2D *m_renderTarget;
	stack<Matrix4> m_modelViewMatrixStack;
	Matrix4 m_projectionMatrix;
};

END_XD_NAMESPACE

#endif // GRAPHICS_CONTEXT_H