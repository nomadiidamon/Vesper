#include <gtest/gtest.h>

#include "Vesper/Scene/Scene.h"
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/SceneSerializer.h"

#include <filesystem>
#include <fstream>

using namespace Vesper;

namespace {

	std::filesystem::path TempScenePath(const char* name)
	{
		return std::filesystem::temp_directory_path() / name;
	}

}

TEST(SceneSerializer, RoundTripPreservesEntities)
{
	const auto path = TempScenePath("vesper_roundtrip_test.vesper");

	auto source = std::make_shared<Scene>("Source");
	Entity a = source->CreateEntity("Alpha", "AAAAAAAA-0000-0000-0000-000000000001");
	a.GetComponent<TransformComponent>().Translation = { 1.0f, 2.0f, 3.0f };
	a.GetComponent<TransformComponent>().Scale = { 2.0f, 2.0f, 2.0f };
	a.AddComponent<SpriteRendererComponent>(glm::vec4(0.25f, 0.5f, 0.75f, 1.0f));

	Entity cam = source->CreateEntity("Cam", "AAAAAAAA-0000-0000-0000-000000000002");
	auto& cc = cam.AddComponent<CameraComponent>();
	cc.Primary = true;
	cc.FixedAspectRatio = true;
	cc.Camera.SetOrthographic(8.0f, -2.0f, 2.0f);

	SceneSerializer(source).Serialize(path.string());

	auto loaded = std::make_shared<Scene>("Loaded");
	ASSERT_TRUE(SceneSerializer(loaded).Deserialize(path.string()));

	Entity la = loaded->GetEntityFromUUID(UUID("AAAAAAAA-0000-0000-0000-000000000001"));
	ASSERT_TRUE(la);
	EXPECT_EQ(la.GetName(), "Alpha");
	EXPECT_EQ(la.GetComponent<TransformComponent>().Translation, glm::vec3(1, 2, 3));
	EXPECT_EQ(la.GetComponent<TransformComponent>().Scale, glm::vec3(2, 2, 2));
	ASSERT_TRUE(la.HasComponent<SpriteRendererComponent>());
	EXPECT_EQ(la.GetComponent<SpriteRendererComponent>().Color, glm::vec4(0.25f, 0.5f, 0.75f, 1.0f));

	Entity lc = loaded->GetPrimaryCameraEntity();
	ASSERT_TRUE(lc);
	EXPECT_EQ(lc.GetName(), "Cam");
	EXPECT_TRUE(lc.GetComponent<CameraComponent>().FixedAspectRatio);
	EXPECT_FLOAT_EQ(lc.GetComponent<CameraComponent>().Camera.GetOrthographicSize(), 8.0f);

	std::filesystem::remove(path);
}

TEST(SceneSerializer, RejectsFileWithoutSceneNode)
{
	const auto path = TempScenePath("vesper_invalid_test.vesper");
	{
		std::ofstream out(path);
		out << "NotAScene: true\n";
	}

	auto scene = std::make_shared<Scene>("Empty");
	EXPECT_FALSE(SceneSerializer(scene).Deserialize(path.string()));

	std::filesystem::remove(path);
}

TEST(SceneSerializer, RejectsMissingFile)
{
	auto scene = std::make_shared<Scene>("Empty");
	EXPECT_FALSE(SceneSerializer(scene).Deserialize(TempScenePath("vesper_does_not_exist.vesper").string()));
}
