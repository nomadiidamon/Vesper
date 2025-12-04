#pragma once

#include "Vesper/Core.h"
#include "Vesper/Events/Event.h"
#include "Vesper/Layer.h"

namespace Vesper {

	class VESPER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();

		void OnUpdate();
		void OnEvent(Event& event) {}

	private:
		float m_Time = 0.0f;
	};

}

