#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"

#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotatable = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetAspectRatio() const { return m_AspectRatio; }
		inline float GetZoomLevel() const { return m_ZoomLevel; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotatable;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslatationSpeed = 5.0f;
		float m_CameraRotatationSpeed = 180.0f;
	};
}