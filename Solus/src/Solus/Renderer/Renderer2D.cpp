#include "supch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Solus {

	struct Renderer2DData
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColourShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData;

		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVerticies[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVerticies, sizeof(squareVerticies));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// Flat Colour Shader Code

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Colour;

			void main()
			{
				color = vec4(u_Colour);
			}
		)";

		s_Data->FlatColourShader = Shader::Create("flatColourShader", vertexSrc, fragmentSrc);
		s_Data->TextureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColourShader->Bind();
		s_Data->FlatColourShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour)
	{
		s_Data->FlatColourShader->Bind();
		s_Data->FlatColourShader->SetFloat4("u_Colour", colour);

		float nrotation = rotation * 3.14159 / 180;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), nrotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColourShader->SetMat4("u_Transform", transform);


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		s_Data->TextureShader->Bind();

		float nrotation = rotation * 3.14159 / 180;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), nrotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, texture, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& colour)
	{
		s_Data->TextureShader->Bind();
		s_Data->FlatColourShader->SetFloat4("u_Colour", colour);

		float nrotation = rotation * 3.14159 / 180;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), nrotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}