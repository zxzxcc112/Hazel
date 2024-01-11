#pragma once
#include "Event.h"

namespace Hazel
{
	class HAZEL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class HAZEL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			:KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int GeteRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_repeatCount;
	};

	class HAZEL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}