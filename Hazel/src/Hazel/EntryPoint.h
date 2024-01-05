#pragma once

#ifdef HZ_PLATFORM_WINDOWS

//Implemented by Client
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_INFO("The Hazel message!");
	HZ_CORE_WARN("The Hazel warning!");
	HZ_CLIENT_WARN("The App {0}!", "message");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif