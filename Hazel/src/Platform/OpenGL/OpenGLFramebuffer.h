#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_FramebufferSpecification; }

		void Invalidate();
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_FramebufferSpecification;
	};
}