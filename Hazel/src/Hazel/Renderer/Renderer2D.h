#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Camera.h"
#include "EditorCamera.h"

#include <glm/glm.hpp>

#include "Hazel/Scene/Components.h"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shotdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		// Rotation is in radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

        static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, int entityID = -1);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetVertexCount() const { return QuadCount * 4; }
			uint32_t GetIndexCount() const { return QuadCount * 6; }
		};

		static const Statistics& GetStatistics();
		static void ResetStatistics();
	private:
		static void FlushAndReset();
	};
}