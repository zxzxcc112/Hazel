#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <entt.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/Components.h"

namespace Hazel
{
	template<typename T, typename Func>
	static void ComponentsLayoutTemplate(Entity& entity, const char* label, Func&& func)
	{
		if (entity.HasComponent<T>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, label))
			{
				func();
				ImGui::TreePop();
			}
		}
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
		:m_Context(context)
	{

	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		for (auto& entityID : m_Context->m_Registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_Context.get() };
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity+456), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			static char buffer[64];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)) && ImGui::IsWindowFocused())
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Transform", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic", "third", "fourth", "5.", "6.", "7."};
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						const bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
							camera.SetProjectionType((SceneCamera::ProjectionType)i);

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = camera.GetPerspectiveVerticalFOV();
					if (ImGui::DragFloat("VerticalFOV", &verticalFOV))
						camera.SetPerspectiveVerticalFOV(verticalFOV);

					float farClip = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetPerspectiveFar(farClip);

					float nearClip = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetPerspectiveNear(nearClip);
				}
				else
				{
					float size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size))
						camera.SetOrthographicSize(size);

					float farClip = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetOrthographicFar(farClip);

					float nearClip = camera.GetOrthographicNear();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetOrthographicNear(nearClip);
					
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
				ImGui::TreePop();
			}
		}

		//ComponentsLayoutTemplate<SpriteRendererComponent>(entity, "Sprite Renderer", [&]() {
		//	auto& src = entity.GetComponent<SpriteRendererComponent>();
		//	ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
		//});
	}
}