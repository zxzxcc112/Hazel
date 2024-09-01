#include "hzpch.h"
#include "SceneSerializer.h"

#include <fstream>
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"

namespace YAML
{
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& vec3) {
            Node node;
            node.push_back(vec3.x);
            node.push_back(vec3.y);
            node.push_back(vec3.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& vec3) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            vec3.x = node[0].as<float>();
            vec3.y = node[1].as<float>();
            vec3.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& vec4) {
            Node node;
            node.push_back(vec4.x);
            node.push_back(vec4.y);
            node.push_back(vec4.z);
            node.push_back(vec4.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& vec4) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }

            vec4.x = node[0].as<float>();
            vec4.y = node[1].as<float>();
            vec4.z = node[2].as<float>();
            vec4.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Hazel
{
    static YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec3& vec3)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
        return out;
    }

    static YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4& vec4)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        :m_Scene(scene)
    {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity& entity)
    {
        out << YAML::BeginMap;  // Entity
        out << YAML::Key << "Entity" << YAML::Value << "12345678945642312"; // TODO: Entity ID goes here

        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;

            auto& transform = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            out << YAML::Key << "Camera";
            out << YAML::BeginMap;
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)cameraComponent.Camera.GetProjectionType();
            out << YAML::Key << "AspectRatio" << YAML::Value << cameraComponent.Camera.GetAspectRatio();
            out << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << cameraComponent.Camera.GetPerspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << cameraComponent.Camera.GetPerspectiveFar();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << cameraComponent.Camera.GetPerspectiveNear();
            out << YAML::Key << "OrthographicSize" << YAML::Value << cameraComponent.Camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicFar" << YAML::Value << cameraComponent.Camera.GetOrthographicFar();
            out << YAML::Key << "OrthographicNear" << YAML::Value << cameraComponent.Camera.GetOrthographicNear();
            out << YAML::EndMap;

            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap;

            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

            out << YAML::EndMap;
        }
        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        for (auto& entityID : m_Scene->m_Registry.view<entt::entity>())
        {
            Entity entity = { entityID, m_Scene.get() };
            if (!entity)
                return;

            SerializeEntity(out, entity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::filesystem::path path(filepath);
        path.remove_filename();
        if (!std::filesystem::exists(path))
            std::filesystem::create_directories(path);
        
        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        HZ_CORE_ASSERT(false, "Not implemented!");
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data = YAML::LoadFile(filepath);

        if (!data["Scene"])
            return false;

        std::string SceneName = data["Scene"].as<std::string>();
        HZ_CORE_TRACE("Deserialize scene: {0}", SceneName);

        if (data["Entities"])
        {
            for (YAML::Node entity : data["Entities"])
            {
                uint64_t uuid = entity["Entity"].as<std::uint64_t>(); // TODO: uuid

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                HZ_CORE_TRACE("Deserialize entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializeEntity = m_Scene->CreateEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // entities always have transforms
                    auto& tc = deserializeEntity.GetComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }
                
                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializeEntity.AddComponent<CameraComponent>();
                    auto& cameraProps = cc.Camera;
                    
                    auto camera = cameraComponent["Camera"];

                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                    cc.Primary = cameraComponent["Primary"].as<bool>();

                    // TODO: Fix camera aspect ratio deserialize
                    //cameraProps.SetAspectRatio(camera["AspectRatio"].as<float>());
                    cameraProps.SetViewportSize(m_Scene->m_ViewportWidth, m_Scene->m_ViewportHeight);
                    cameraProps.SetOrthographic(camera["OrthographicSize"].as<float>(),
                                           camera["OrthographicFar"].as<float>(),
                                           camera["OrthographicNear"].as<float>());
                    cameraProps.SetPerspective(camera["PerspectiveVerticalFOV"].as<float>(),
                                          camera["PerspectiveFar"].as<float>(),
                                          camera["PerspectiveNear"].as<float>());
                    cameraProps.SetProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializeEntity.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
                }
            }
        }
        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        HZ_CORE_ASSERT(false, "Not implemented!");
        return false;
    }


}