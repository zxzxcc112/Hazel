#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	OpenGLContext::~OpenGLContext()
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int gladInit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(gladInit, "Glad initialize failed!");

		HZ_CORE_INFO("---OpenGL Info---:");
		HZ_CORE_INFO("  vendor: {0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("renderer: {0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO(" version: {0}", (const char*)glGetString(GL_VERSION));
		HZ_CORE_INFO("    GLSL: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		HZ_CORE_INFO("------------------");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}