#include "supch.h"
#include "Solus/Renderer/Renderer2D.h"

#include "Solus/Renderer/VertexArray.h"
#include "Solus/Renderer/Shader.h"
#include "Solus/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Solus {

	struct SquareVertex
	{
		glm::vec3 position;
		glm::vec4 colour;
		glm::vec2 texCoord;
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVerticies = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t squareIndexCount = 0;

		SquareVertex* squareVertexBufferBase = nullptr;
		SquareVertex* squareVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;	

	void Renderer2D::Init()
	{
		SU_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		float squareVerticies[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.maxVerticies * sizeof(SquareVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Colour" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.squareVertexBufferBase = new SquareVertex[s_Data.maxVerticies];

		uint32_t* squareIndices = new uint32_t[s_Data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.maxIndices; i += 6)
		{
			squareIndices[i + 0] = offset + 0;
			squareIndices[i + 1] = offset + 1;
			squareIndices[i + 2] = offset + 2;

			squareIndices[i + 3] = offset + 2;
			squareIndices[i + 4] = offset + 3;
			squareIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, s_Data.maxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(squareIB);
		delete[] squareIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		SU_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SU_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.squareIndexCount = 0;
		s_Data.squareVertexBufferPtr = s_Data.squareVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		SU_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.squareVertexBufferPtr - (uint8_t*)s_Data.squareVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.squareVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.squareIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour)
	{
		SU_PROFILE_FUNCTION();

		s_Data.squareVertexBufferPtr->position = position;
		s_Data.squareVertexBufferPtr->colour = colour;
		s_Data.squareVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_Data.squareVertexBufferPtr++;

		s_Data.squareVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		s_Data.squareVertexBufferPtr->colour = colour;
		s_Data.squareVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_Data.squareVertexBufferPtr++;

		s_Data.squareVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.squareVertexBufferPtr->colour = colour;
		s_Data.squareVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_Data.squareVertexBufferPtr++;

		s_Data.squareVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		s_Data.squareVertexBufferPtr->colour = colour;
		s_Data.squareVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_Data.squareVertexBufferPtr++;

		s_Data.squareIndexCount += 6;

		/*s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& colour, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, texture, colour, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& colour, float tilingFactor)
	{
		SU_PROFILE_FUNCTION();
		s_Data.TextureShader->SetFloat4("u_Colour", colour);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

}