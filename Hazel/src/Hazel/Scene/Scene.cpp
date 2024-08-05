#include "hzpch.h"
#include "Scene.h"
#include "Components.h"

#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel
{
	//use component to do something
	static void DoMath(const glm::mat4& transform)
	{

	}

	//do something when a component is constructed by registry and determine which entity it belongs to
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		HZ_CORE_WARN("OnTransformConstruct");
	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		struct MeshComponent {};
		struct TransformComponent
		{
			glm::mat4 Transform{ 1.0f };

			TransformComponent() = default;
			TransformComponent(const TransformComponent& component) = default;
			TransformComponent(const glm::mat4& transform)
				:Transform(transform) {}

			operator glm::mat4&() { return Transform; }
			operator const glm::mat4&() const { return Transform; }
		};

		TransformComponent transform;
		DoMath(transform);

		//create entity
		entt::entity entity = m_Registry.create();

		//connect function when a component constructed
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		//create a component in entity
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if(m_Registry.all_of<TransformComponent>(entity))
			auto& trans = m_Registry.get<TransformComponent>(entity);

		//create a view or group to iterate over entities that have specific components
		auto view = m_Registry.view<TransformComponent, MeshComponent>();
		for (auto entity : view)
		{

		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{

		}
#endif
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{	
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transformComponent, spriteRendererComponent.Color);
		}
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}