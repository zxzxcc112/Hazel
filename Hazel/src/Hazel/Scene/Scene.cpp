#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag = std::string())
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{	
		// Update Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			// TODO: Move to Scene::OnScenePlay
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = { entity, this };
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(ts);
		});


		//Rneder Sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &view.get<TransformComponent>(entity).Transform;
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transformComponent, spriteRendererComponent.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}