#pragma once

#include "Solus/Renderer/Texture.h"

#include <glad/glad.h>

namespace Solus {

	class SDLTexture2D : public Texture2D
	{
	public:
		SDLTexture2D(const uint32_t width, const uint32_t height);
		SDLTexture2D(const std::string& path);
		virtual ~SDLTexture2D();

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{ return m_RendererID == ((SDLTexture2D&)other).m_RendererID; }
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};

}

