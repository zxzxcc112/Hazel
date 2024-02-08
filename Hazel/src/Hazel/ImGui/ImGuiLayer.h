#pragma once
#include "Hazel/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
	private:
		//bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		//bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		//bool OnMouseMovedEvent(MouseMovedEvent& e);
		//bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		//
		//bool OnKeyPressedEvent(KeyPressedEvent& e);
		//bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//bool OnKeyTypedEvent(KeyTypedEvent& e);
		//
		//bool OnWindowResized(WindowResizeEvent& e);

		float m_Time = 0.0f;
	};

}


