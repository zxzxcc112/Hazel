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

		virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const;

		virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override { HZ_CORE_ASSERT(index < m_ColorAttachments.size(), "");  return m_ColorAttachments[index]; }
		virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_FramebufferSpecification; }

		void Invalidate();
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_FramebufferSpecification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment;
	};
}