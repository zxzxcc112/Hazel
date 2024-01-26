#include "hzpch.h"
#include "Application.h"

namespace Hazel
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {}

	void Application::Run()
	{
		HZ_CORE_INFO("The Hazel Application is running.");

		while (m_Running)
		{
			m_Window->OnUpdate();
		}

	}
}