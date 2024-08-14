#include "hzpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	SceneCamera::SceneCamera()
	{
		RecalculateOrthographic();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateOrthographic();
	}

	void SceneCamera::SetOrthographic(float size, float farClip, float nearClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateOrthographic();
	}

	void SceneCamera::RecalculateOrthographic()
	{
		float orthographicLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthographicRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthographicBottom = -m_OrthographicSize * 0.5f;
		float orthographicTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthographicLeft, orthographicRight, orthographicBottom, orthographicTop,
			m_OrthographicNear, m_OrthographicFar);
	}
}