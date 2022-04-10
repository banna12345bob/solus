#pragma once

#include "Solus/Layers/Layer.h"
#include "Solus/Events/AllEvents.h"

namespace Solus {

	class SOLUS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}