#pragma once

#include "Solus/Core/Core.h"

namespace Solus {
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};

}

