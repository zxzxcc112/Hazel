#pragma once

#include <entt.hpp>
#include "Hazel/Scene/Scene.h"

namespace Hazel
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;
		Entity(entt::entity entity, Scene* scene)
			:m_EntityHandle(entity), m_Scene(scene)
		{

		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
