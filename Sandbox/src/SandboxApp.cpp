//Inherits the Hazel Application and define how to create application
#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "sandbox2D.h"
#include "FlappyBird/GameLayer.h"

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		//PushLayer(new GameLayer());
	}
	~Sandbox() {}
};

//implement how to create app
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}