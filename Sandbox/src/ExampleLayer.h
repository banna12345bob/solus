#pragma once

#include <Solus.h>

class ExampleLayer : public Solus::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Solus::Timestep time) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Solus::Event& event) override;
private:
	Solus::Ref<Solus::Texture2D> m_Texture, m_OakLogTex;

	Solus::OrthographicCameraController m_CameraController;

	glm::vec4 m_squareColour = { 0.2f, 0.3f, 0.8f, 1.0f };
};