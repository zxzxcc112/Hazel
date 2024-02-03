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
		HZ_CLIENT_INFO("{0} update.", GetName());
	}

	void OnEvent(Hazel::Event& e) override
	{
		HZ_CLIENT_TRACE(e);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

//implement how to create app
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}