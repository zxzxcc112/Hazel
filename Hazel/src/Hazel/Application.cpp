#include "Application.h"
#include "Log.h"
#include "Events/ApplicationEvent.h"

namespace Hazel
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		HZ_CORE_INFO("The Hazel message!");
		HZ_CORE_WARN("The Hazel warning!");
		HZ_CLIENT_WARN("The App {0}!", "message");
		WindowResizeEvent e(1366, 768);
		HZ_CORE_TRACE(e);

		while (true);
	}
}