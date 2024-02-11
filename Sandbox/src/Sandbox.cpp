//Inherits the Hazel Application and define how to create application

#include <Hazel.h>

#include "imgui.h"

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

	void OnImGuiRender() override
	{
		ImGui::Begin("Title");
		ImGui::Text("text");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_CLIENT_TRACE(event);
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