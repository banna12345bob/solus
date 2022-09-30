#include <Solus.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Solus::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(m_Camera.GetPosition())
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
			layout(location = 1) in vec4 a_Colour;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
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
				colour = vec4(v_Position + 0.5, 1.0);
				colour = vec4(v_Colour);
			}
		)";

		m_Shader.reset(Solus::Shader::Create(vertexSrc, fragmentSrc));


		std::string flatColourShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColourShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Colour = vec3(1, 0, 1);

			void main()
			{
				color = vec4(u_Colour, 1.0f);
			}
		)";

		m_flatColourShader.reset(Solus::Shader::Create(flatColourShaderVertexSrc, flatColourShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Solus::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
		m_OakLogTex = Solus::Texture2D::Create("assets/textures/Oak_Log.png");

		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Solus::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Solus::Timestep time) override
	{
		if (Solus::Input::IsKeyPressed(SU_KEY_LEFT))
			m_CameraPosition.x -= m_CameraSpeed * time;

		else if (Solus::Input::IsKeyPressed(SU_KEY_RIGHT))
			m_CameraPosition.x += m_CameraSpeed * time;

		if (Solus::Input::IsKeyPressed(SU_KEY_UP))
			m_CameraPosition.y += m_CameraSpeed * time;

		else if (Solus::Input::IsKeyPressed(SU_KEY_DOWN))
			m_CameraPosition.y -= m_CameraSpeed * time;

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(0.0f);

		Solus::Renderer::BeginScene(m_Camera);
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

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

		m_Texture->Bind();
		Solus::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_OakLogTex->Bind();
		Solus::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
	}

private:
	Solus::Ref<Solus::Shader> m_Shader;
	Solus::Ref<Solus::VertexArray> m_VertexArray;

	Solus::Ref<Solus::Shader> m_flatColourShader, m_TextureShader;
	Solus::Ref<Solus::VertexArray> m_SquareVA;

	Solus::Ref<Solus::Texture2D> m_Texture, m_OakLogTex;

	Solus::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 5.0f;

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