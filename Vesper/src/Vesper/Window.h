#pragma once

#include "vzpch.h"

#include "Vesper/Core.h"

namespace Vesper {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const std::string& title = "Vesper Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class VESPER_API Window
	{
	public:

		virtual ~Window() {}
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};
}