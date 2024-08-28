#pragma once
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Layer.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;
	};
}


