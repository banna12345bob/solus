#include "supch.h"
#include "SubTexture2D.h"

namespace Solus {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, glm::vec2& max, const glm::vec2& coords)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };

		glm::vec2 cellsize = { (min.x * texture->getWidth()) / coords.x, (min.y * texture->getHight()) / coords.y };
		m_SpriteSize = { ((max.x * texture->getWidth()) / cellsize.x) - coords.x, ((max.y * texture->getHight()) / cellsize.y) - coords.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * cellSize.x) / texture->getWidth(), (coords.y * cellSize.y) / texture->getHight()};
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->getWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->getHight() };
		return CreateRef<SubTexture2D>(texture, min, max, coords);
	}

}