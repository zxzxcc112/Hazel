#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.15f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.15f, 0.1f, 1.0f));

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.55f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.75f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.55f, 0.1f, 1.0f));

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.1f, 0.75f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.2f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.1f, 0.75f, 1.0f));

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
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
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				glm::vec3 degrees = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", degrees);
				tc.Rotation = glm::radians(degrees);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

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
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("VerticalFOV", &verticalFOV))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

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