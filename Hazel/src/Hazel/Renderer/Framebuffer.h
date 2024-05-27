#pragma once
#include "Hazel/Core/Base.h"

namespace Hazel
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		// FramebufferFormat Format = 
		uint32_t Samples = 1;

		bool SwapChainTarget = false; // like gl bind framebuffer 0
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentID() const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}