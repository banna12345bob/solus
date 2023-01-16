#include "Sandbox2D.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Solus::VertexArray::Create();

	float squareVerticies[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
	};

	Solus::Ref<Solus::VertexBuffer> squareVB;
	squareVB.reset(Solus::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));
	squareVB->SetLayout({
		{ Solus::ShaderDataType::Float3, "a_Position" }
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

	m_flatColourShader = Solus::Shader::Create("flatColourShader", vertexSrc, fragmentSrc);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Solus::Timestep time)
{
	m_CameraController.OnUpdate(time);

	Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
	Solus::RenderCommand::Clear();

	Solus::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Solus::OpenGLShader>(m_flatColourShader)->Bind();
	std::dynamic_pointer_cast<Solus::OpenGLShader>(m_flatColourShader)->UploadUniformFloat4("u_Colour", m_squareColour);

	Solus::Renderer::Submit(m_flatColourShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Solus::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_squareColour));
	ImGui::End();
}

void Sandbox2D::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}