#include "supch.h"
#include "Platform/SDL/SDLBuffer.h"

#include <glad/glad.h>

namespace Solus {

	/*---------------------------------- Vertex Buffer ----------------------------------*/
	SDLVertexBuffer::SDLVertexBuffer(uint32_t size)
	{
		SU_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	SDLVertexBuffer::SDLVertexBuffer(float* vertices, uint32_t size)
	{
		SU_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	SDLVertexBuffer::~SDLVertexBuffer()
	{
		SU_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RenderID);
	}

	void SDLVertexBuffer::Bind() const
	{
		SU_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void SDLVertexBuffer::Unbind() const
	{
		SU_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SDLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/*---------------------------------- Index Buffer ----------------------------------*/

	SDLIndexBuffer::SDLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		SU_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	SDLIndexBuffer::~SDLIndexBuffer()
	{
		SU_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RenderID);
	}

	void SDLIndexBuffer::Bind() const
	{
		SU_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}

	void SDLIndexBuffer::Unbind() const
	{
		SU_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}