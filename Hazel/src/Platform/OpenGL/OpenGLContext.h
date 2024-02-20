#pragma once
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}