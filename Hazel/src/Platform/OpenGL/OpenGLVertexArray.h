#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const Hazel::Ref<VertexBuffer>&) override;
		virtual void SetIndexBuffer(const Hazel::Ref<IndexBuffer>&) override;

		virtual const std::vector<Hazel::Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; };
		virtual const Hazel::Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };
	private:
		uint32_t m_RendererID;
		std::vector<Hazel::Ref<VertexBuffer>> m_VertexBuffers;
		uint32_t m_VertexBufferIndex = 0;
		Hazel::Ref<IndexBuffer> m_IndexBuffer;
	};
}