#pragma once

#include "Event.h"
#include "Hazel/Core/MouseCodes.h"

namespace Hazel
{
	class HAZEL_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			:m_mouseX(mouseX), m_mouseY(mouseY) {}

		float GetX() const { return m_mouseX; }
		float GetY() const { return m_mouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_mouseX;
		float m_mouseY;
	};
	
	class HAZEL_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			:m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetOffsetX() const { return m_XOffset; }
		float GetOffsetY() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_XOffset;
		float m_YOffset;
	};

	class HAZEL_API MouseButtonEvent :public Event
	{
	public:
		MouseCode GetButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(MouseCode button)
			:m_button(button) {}

		MouseCode m_button;
	};

	class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MousePressed)
	};

	class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseReleased)
	};
}