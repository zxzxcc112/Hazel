#pragma once

#ifdef HZ_PLATFORM_WINDOWS

//Implemented by Client
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif