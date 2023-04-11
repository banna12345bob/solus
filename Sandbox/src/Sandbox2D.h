#pragma once

#include <Solus.h>

class Sandbox2D : public Solus::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnDetach() override;
	virtual void OnAttach() override;

	void OnUpdate(Solus::Timestep time) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Solus::Event& event) override;
private:
	Solus::OrthographicCameraController m_CameraController;

	Solus::Ref<Solus::VertexArray> m_SquareVA;
	Solus::Ref<Solus::Shader> m_flatColourShader;
	Solus::Ref<Solus::Texture2D> m_CheckerboardTexture;
	Solus::Ref<Solus::Texture2D> m_Tilesheet;
	Solus::Ref<Solus::SubTexture2D> m_TextureGrass, m_TextureTree, m_TexturePath, m_TextureMushrooms;

	Solus::audioPlayer m_AudioPlayer;

	float m_Position[2] = { 0.0f, 0.0f };
	float m_CheckerboardPosition[2] = {0.0f, 0.0f};
	float m_Scale[2] = { 1.0f, 1.0f };
	float m_CheckerboardScale[2] = {10.0f, 10.0f};
	float m_Rotation[1] = { 0.0f };
	float m_CheckerboardRotation[1] = { 0.0f };
	float m_TilingFactor[1] = { 1.0f };
	glm::vec4 m_squareColour = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_CheckerboardColour = { 1.0f, 1.0f, 1.0f, 1.0f };

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Solus::Ref<Solus::SubTexture2D>> s_TextureMap;
};