#include "Sandbox2D.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Solus::Timestep time)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	m_CameraController.OnUpdate(time);

	{
		PROFILE_SCOPE("Renderer Prep");

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");

		Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Solus::Renderer2D::DrawQuad(glm::vec3(m_CheckerboardPosition[0], m_CheckerboardPosition[1], -0.1f), m_CheckerboardRotation[0], glm::vec2(m_CheckerboardScale[0], m_CheckerboardScale[1]), m_CheckerboardTexture, m_CheckerboardColour);

		Solus::Renderer2D::DrawQuad(glm::vec2(m_Position[0], m_Position[1]), m_Rotation[0], glm::vec2(m_Scale[0], m_Scale[1]), m_squareColour);

		Solus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Square Settings");
	ImGui::ColorEdit4("Colour", glm::value_ptr(m_squareColour));
	ImGui::SliderFloat2("Position", m_Position, -5, 5, "%.3f", 1.0f);
	ImGui::SliderFloat2("Scale", m_Scale, 0, 5, "%.3f", 1.0f);
	ImGui::SliderFloat("Roation", m_Rotation, 0, 360, "%.3f", 1.0f);
	ImGui::End();

	ImGui::Begin("Checkerboard Settings");
	ImGui::ColorEdit4("Colour", glm::value_ptr(m_CheckerboardColour));
	ImGui::SliderFloat2("Position", m_CheckerboardPosition, -5, 5, "%.3f", 1.0f);
	ImGui::SliderFloat2("Scale", m_CheckerboardScale, 0, 10, "%.3f", 1.0f);
	ImGui::SliderFloat("Roation", m_CheckerboardRotation, 0, 360, "%.3f", 1.0f);
	ImGui::End();

	ImGui::Begin("Timer");
	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}