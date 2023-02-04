#pragma once

#include "Solus/Core/Layer.h"
#include "Solus/Events/AllEvents.h"

namespace Solus {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}