#pragma once

#include "Solus/Core/Input.h"

namespace Solus {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;				  
		virtual float GetMouseYImpl() override;				  
	};

}