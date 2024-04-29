#pragma once
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} //When layer push to layer stack
		virtual void OnDetach() {} //When layer pop from layer stack
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}

