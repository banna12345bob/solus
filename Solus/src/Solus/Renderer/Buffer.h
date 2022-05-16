#pragma once

namespace Solus {
	
	struct BufferElements
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;

		BufferElements(const std::string& name, ShaderDataType type)
			: Name(name)
	};

	class BufferLayout
	{
	public:


		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }
	private:
		std::vector<BufferElements> m_Elements;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetSize() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

}