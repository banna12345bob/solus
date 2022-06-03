#include <Solus.h>

#include "ImGui/imgui.h"

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

		std::shared_ptr<Solus::VertexBuffer> vertexBuffer;
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
		std::shared_ptr<Solus::IndexBuffer> indexBuffer;
		indexBuffer.reset(Solus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Solus::VertexArray::Create());

		float squareVerticies[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Solus::VertexBuffer> squareVB;
		squareVB.reset(Solus::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));
		squareVB->SetLayout({
			{ Solus::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Solus::IndexBuffer> squareIB;
		squareIB.reset(Solus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Solus::Shader(vertexSrc, fragmentSrc));


		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Solus::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate() override
	{
		if (Solus::Input::IsKeyPressed(SU_KEY_LEFT))
			m_CameraPosition.x -= m_CameraSpeed;

		else if (Solus::Input::IsKeyPressed(SU_KEY_RIGHT))
			m_CameraPosition.x += m_CameraSpeed;

		if (Solus::Input::IsKeyPressed(SU_KEY_UP))
			m_CameraPosition.y += m_CameraSpeed;

		else if (Solus::Input::IsKeyPressed(SU_KEY_DOWN))
			m_CameraPosition.y -= m_CameraSpeed;

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(0.0f);

		Solus::Renderer::BeginScene(m_Camera);

		Solus::Renderer::Submit(m_BlueShader, m_SquareVA);
		Solus::Renderer::Submit(m_Shader, m_VertexArray);

		Solus::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
	}

	void OnEvent(Solus::Event& event) override
	{
	}

private:
	std::shared_ptr<Solus::Shader> m_Shader;
	std::shared_ptr<Solus::VertexArray> m_VertexArray;

	std::shared_ptr<Solus::Shader> m_BlueShader;
	std::shared_ptr<Solus::VertexArray> m_SquareVA;

	Solus::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.1f;
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