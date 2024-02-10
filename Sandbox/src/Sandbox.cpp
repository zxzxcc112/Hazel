//Inherits the Hazel Application and define how to create application

#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example Layer")
	{

	}

	~ExampleLayer()
	{

	}

	void OnUpdate() override
	{
		//HZ_CLIENT_INFO("{0} update.", GetName());
	}

	void OnEvent(Hazel::Event& event) override
	{
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_CLIENT_TRACE("{0}", (char)e.GetKeyCode());
		}
		HZ_CLIENT_TRACE(event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	~Sandbox() {}
};

//implement how to create app
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}