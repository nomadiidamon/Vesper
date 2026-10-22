#pragma once

#include "Vesper/GUI/ImGui/ImGuiLayer.h"

namespace Vesper {

	class ImGuiLayer_OpenGL : public ImGuiLayer
	{
	public:
		ImGuiLayer_OpenGL();
		~ImGuiLayer_OpenGL();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;
	};

}
