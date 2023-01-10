#include <Solus.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Solus::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray.reset(Solus::VertexArray::Create());

		float vertices[3 * 7] =
		{	/*   Positions   */		/* Colours RGB Format */
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		Solus::Ref<Solus::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Solus::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Solus::BufferLayout layout = {
				{ Solus::ShaderDataType::Float3, "a_Position" },
				{ Solus::ShaderDataType::Float4, "a_Colour" }
			};

			vertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(vertexBuffer);
		}

		unsigned int indices[3] = { 0, 1, 2 };
		Solus::Ref<Solus::IndexBuffer> indexBuffer;
		indexBuffer.reset(Solus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Solus::VertexArray::Create());

		float squareVerticies[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Solus::Ref<Solus::VertexBuffer> squareVB;
		squareVB.reset(Solus::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));
		squareVB->SetLayout({
			{ Solus::ShaderDataType::Float3, "a_Position" },
			{ Solus::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Solus::Ref<Solus::IndexBuffer> squareIB;
		squareIB.reset(Solus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = vec4(1.0f, 0.0f, 1.0f, 1.0f);
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 colour;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				colour = vec4(v_Colour);
			}
		)";

		m_Shader = Solus::Shader::Create("VertexPosColour", vertexSrc, fragmentSrc);
		m_ShaderLibary.Add("defaultShader", m_Shader);

		auto m_flatColourShader = m_ShaderLibary.Load("assets/shaders/flatColour.glsl");

		auto m_TextureShader = m_ShaderLibary.Load("assets/shaders/texture.glsl");

		m_Texture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
		m_OakLogTex = Solus::Texture2D::Create("assets/textures/Oak_Log.png");

		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Solus::Timestep time) override
	{
		m_CameraController.OnUpdate(time);

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();

		Solus::Renderer::BeginScene(m_CameraController.GetCamera());
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto m_flatColourShader = m_ShaderLibary.Get("flatColour");

		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_flatColourShader)->Bind();
		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_flatColourShader)->UploadUniformFloat3("u_Colour", m_squareColour);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Solus::Renderer::Submit(m_flatColourShader, m_SquareVA, transform);

			}
		}

		auto m_TextureShader = m_ShaderLibary.Get("texture");

		m_Texture->Bind();
		Solus::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_OakLogTex->Bind();
		Solus::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Shader = m_ShaderLibary.Get("defaultShader");
		//Solus::Renderer::Submit(m_Shader, m_VertexArray);

		Solus::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Colour", glm::value_ptr(m_squareColour));

		ImGui::End();
	}

	void OnEvent(Solus::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Solus::ShaderLibary m_ShaderLibary;
	Solus::Ref<Solus::Shader> m_Shader;
	Solus::Ref<Solus::VertexArray> m_VertexArray;

	Solus::Ref<Solus::VertexArray> m_SquareVA;

	Solus::Ref<Solus::Texture2D> m_Texture, m_OakLogTex;

	Solus::OrthographicCameraController m_CameraController;

	glm::vec3 m_squareColour = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Solus::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

Solus::Application* Solus::CreateApplication()
{
	return new Sandbox();
}