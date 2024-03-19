#include "hzpch.h"
#include "Buffer.h"
#include "Hazel/Core.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

// Maybe add in future
//#ifdef HZ_PLATFORM_WINDOWS
//#include "Platform/Direct3D/Direct3DBuffer.h"
//#endif

namespace Hazel
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "Renderer API: None is  currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
			//#ifdef HZ_PLATFORM_WINDOWS
			//case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size);
			//#endif
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: HZ_CORE_ASSERT(false, "Renderer API: None is  currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}