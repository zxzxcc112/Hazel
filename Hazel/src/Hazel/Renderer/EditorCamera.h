#pragma once

#include "Camera.h"

#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Hazel
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        void SetDistance(float distance) { m_Distance = distance; }
        float GetDistance() const { return m_Distance; }

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        glm::quat GetOrientation() const;
        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        const glm::vec3& GetPosition() const { return m_Position; }
        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }
    private:
        void UpdateView();
        void UpdateProjection();

        bool OnMouseScroll(MouseScrolledEvent& e);

        glm::vec3 CalculatePosition() const;

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        std::pair<float, float> PanSpeed() const;
        float RotateSpeed() const;
        float ZoomSpeed() const;
    private:
        float m_FOV = 45.0f, m_AspectRatio = 1.7778f, m_NearClip = 0.0001f, m_FarClip = 1000.0f;
        float m_ViewportWidth = 1280, m_ViewportHeight = 720;

        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
        float m_Distance = 10.0f;

        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
    };
}