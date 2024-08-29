#pragma once
#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		void SetViewportSize(uint32_t width, uint32_t height);

		void SetOrthographic(float size, float farClip, float nearClip);
		void SetPerspective(float verticalFOV, float farClip, float nearClip);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthographicNear; }
		void SetOrthographicNear(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFar() const { return m_OrthographicFar; }
		void SetOrthographicFar(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		float GetPerspectiveVerticalFOV() const { return m_VerticalFOV; }
		void SetPerspectiveVerticalFOV(float VerticalFOV) { m_VerticalFOV = VerticalFOV; RecalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_AspectRatio = 1.78f;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_VerticalFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;
	};
}
