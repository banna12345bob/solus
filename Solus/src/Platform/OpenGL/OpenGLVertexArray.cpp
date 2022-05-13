#include "supch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Solus {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RenderID);
		glBindVertexArray(m_RenderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}