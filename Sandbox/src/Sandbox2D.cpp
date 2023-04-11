#include "Sandbox2D.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"gggggggggPPPgggggggggggg"
"ggggggggPPPggggggggggggg"
"gggggggggPPPgggggggggggg"
"gggggggggPPPgggggggggggg"
"ggggggggPPPggggggggggggg"
"ggggggggPPPggggggggggggg"
"gggggggggPPPgggggggggggg"
"ggggggggggPPPggggggggggg"
"gggggggggPPPgggggggggggg"
"gggggggggPPPgggggggggggg"
"ggggggggPPPggggggggggggg"
"gggggggggPPPgggggggggggg"
"gggggggggPPPgggggggggggg"
"ggggggggggPPPggggggggggg"
"ggggggggggPPPgggggHggggg"
"gggggggggPPPgggggggggggg";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	SU_PROFILE_FUNCTION();
	m_CheckerboardTexture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Tilesheet = Solus::Texture2D::Create("assets/textures/game/kenny_tiny_town.png");
	m_TextureTree = Solus::SubTexture2D::CreateFromCoords(m_Tilesheet, { 4, 9 }, { 16, 16 }, { 1, 2 });
	m_TextureMushrooms = Solus::SubTexture2D::CreateFromCoords(m_Tilesheet, { 5, 8 }, { 16, 16 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

	s_TextureMap['P'] = Solus::SubTexture2D::CreateFromCoords(m_Tilesheet, { 7, 7 }, { 16, 16 });
	s_TextureMap['g'] = Solus::SubTexture2D::CreateFromCoords(m_Tilesheet, {0, 10}, {16, 16});

	m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	SU_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Solus::Timestep time)
{
	SU_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(time);

	Solus::Renderer2D::ResetStats();

	{
		SU_PROFILE_SCOPE("Renderer Prep");

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();
	}

	{
		SU_PROFILE_SCOPE("Renderer Draw");
#if 0
		Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Solus::Renderer2D::DrawQuad(glm::vec3(m_CheckerboardPosition[0], m_CheckerboardPosition[1], -0.1f), m_CheckerboardRotation[0], glm::vec2(m_CheckerboardScale[0], m_CheckerboardScale[1]), m_CheckerboardTexture, m_CheckerboardColour, m_TilingFactor[0]);

		Solus::Renderer2D::DrawQuad(glm::vec2(m_Position[0], m_Position[1]), m_Rotation[0], glm::vec2(m_Scale[0], m_Scale[1]), m_squareColour);

		Solus::Renderer2D::EndScene();
#endif

		Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = 0; y <= m_CameraController.getBounds().Top; y++)
		{
			//SU_WARN("{0}", y);
			for (float x = 0; x <= m_CameraController.getBounds().Right; x++)
			{
				//SU_INFO("{0}", x);
				Solus::Renderer2D::DrawQuad({ x, y }, 0.0f, { 1.0f, 1.0f }, s_TextureMap['g']);
			}
		}

		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				if (tileType != 'g')
				{
					Solus::Ref<Solus::SubTexture2D> texture;

					if (s_TextureMap.find(tileType) != s_TextureMap.end())
						texture = s_TextureMap[tileType];
					else
						texture = m_TextureMushrooms;

					Solus::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f }, 0.0f, { 1.0f, 1.0f }, texture);
				}
			}
		}

		/*Solus::Renderer2D::DrawQuad({ 1.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, m_TextureGrass);
		Solus::Renderer2D::DrawQuad({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, m_TexturePath);
		Solus::Renderer2D::DrawQuad({-1.0f, 0.0f}, 0.0f, {1.0f, 2.0f}, m_TextureTree);*/

		Solus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	SU_PROFILE_FUNCTION();

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
	ImGui::SliderFloat("TilingFactor", m_TilingFactor, 0, 10, "%.3f", 1.0f);
	ImGui::End();

	ImGui::Begin("Audio test");

	bool playTest2Ch = ImGui::Button("Play STEREO test audio");
	if (playTest2Ch) m_AudioPlayer.PlayThreaded("assets/audio/test (2ch).wav");

	bool playTest1Ch = ImGui::Button("Play MONO test audio");
	if (playTest1Ch) m_AudioPlayer.PlayThreaded("assets/audio/test (1ch).wav");

	ImGui::End();

	auto stats = Solus::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.squareCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
}

void Sandbox2D::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}