#pragma once

#include "Solus/Renderer/VertexArray.h"

namespace Solus {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RenderID;
	};

}

