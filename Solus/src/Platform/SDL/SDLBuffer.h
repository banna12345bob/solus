#pragma once

#include "Solus/Renderer/Buffer.h"

namespace Solus {

	class SDLVertexBuffer : public VertexBuffer
	{
	public:
		SDLVertexBuffer(uint32_t size);
		SDLVertexBuffer(float* vertices, uint32_t size);
		virtual ~SDLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void SetData(const void* data, uint32_t size) override;
	private:
		uint32_t m_RenderID;
		BufferLayout m_Layout;
	};

	class SDLIndexBuffer : public IndexBuffer
	{
	public:
		SDLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~SDLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};

}