//Inherits the Hazel Application and define how to create application

#include <Hazel.h>

class Sandbox : public Hazel::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

//implement how to create app
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}