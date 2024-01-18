#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel
{
	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MousePressed, MouseReleased, MouseScrolled, MouseMoved
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)	static inline EventType GetStaticType() {return EventType::##type; }\
								virtual EventType GetEventType() const override { return  GetStaticType(); }\
								virtual const char* GetEventName() const override { return #type; }
	
#define EVENT_CLASS_CATEGORY(flags) virtual unsigned int GetCategoryFlags() const override { return flags; }


	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:

		virtual EventType GetEventType() const = 0;
		virtual unsigned int GetCategoryFlags() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual std::string ToString() const { return GetEventName(); }
		
		inline bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & category;
		}

	private:
		bool m_handled = false;
	};

	class HAZEL_API EventDispatcher
	{
		template<typename T>
		using EventFu = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			:m_event(event) {}

		template<typename T>
		bool Dispatch(EventFu<T> Func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.m_handled = Func(*(T*)& m_event);
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}