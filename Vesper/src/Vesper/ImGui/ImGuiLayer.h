#pragma once

#include "Vesper/Core/Base.h"
#include "Vesper/Events/Event.h"
#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Events/KeyEvent.h"
#include "Vesper/Events/MouseEvent.h"
#include "Vesper/Core/Layer.h"

namespace Vesper {

	class VESPER_API ImGuiLayer : public Layer
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

