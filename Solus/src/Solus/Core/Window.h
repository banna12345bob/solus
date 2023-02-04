#pragma once

#include "supch.h"

#include "Solus/Core/Core.h"
#include "Solus/Events/Event.h"

namespace Solus {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		const char* pathToIcon;
		bool fullscreen;
		bool nativeResulution;

		//Default title, width and height
		WindowProps(const std::string& title = "Solus Application",
			unsigned int width = 1280,
			unsigned int height = 720,
			const char* pathToIcon = "assets/textures/Checkerboard.png",
			bool fullscreen = false,
			bool nativeResulution = true)
			: Title(title), Width(width), Height(height), pathToIcon(pathToIcon), fullscreen(fullscreen), nativeResulution(nativeResulution)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}