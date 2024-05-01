#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> SquareVA;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage *s_Data;

	void Hazel::Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->SquareVA = Hazel::VertexArray::Create();

		float squarevertices[] = {
			-0.7f, -0.7f, 0.0f,
			 0.7f, -0.7f, 0.0f,
			 0.7f,  0.7f, 0.0f,
			-0.7f,  0.7f, 0.0f
		};

		Ref<VertexBuffer> squarevertexBuffer;
		squarevertexBuffer = VertexBuffer::Create(squarevertices, sizeof(squarevertices));

		squarevertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
		});

		s_Data->SquareVA->AddVertexBuffer(squarevertexBuffer);

		uint32_t squareindices[] = {
				0, 1, 2, 2, 3, 0
		};

		Ref<IndexBuffer> squareindexBuffer;
		squareindexBuffer = IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t));
		s_Data->SquareVA->SetIndexBuffer(squareindexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shotdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMatrix4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMatrix4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_FlatColor", color);

		s_Data->SquareVA->Bind();
		RenderCommand::DrawIndexed(s_Data->SquareVA);
	}
}