#include "hzpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float farClip, float nearClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float farClip, float nearClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_VerticalFOV = verticalFOV;
		m_PerspectiveFar = farClip;
		m_PerspectiveNear = nearClip;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthographicLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthographicRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthographicBottom = -m_OrthographicSize * 0.5f;
			float orthographicTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthographicLeft, orthographicRight, orthographicBottom, orthographicTop,
				m_OrthographicNear, m_OrthographicFar);
		}
		else
		{
			m_Projection = glm::perspective(glm::radians(m_VerticalFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
	}
}