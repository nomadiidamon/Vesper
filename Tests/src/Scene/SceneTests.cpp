#include <gtest/gtest.h>

#include "Vesper/Scene/Scene.h"
#include "Vesper/Scene/Entity.h"

#include <regex>

using namespace Vesper;

class SceneTest : public ::testing::Test
{
protected:
	Scene scene{ "TestScene" };
};

TEST_F(SceneTest, CreateEntityAddsDefaultComponents)
{
	Entity e = scene.CreateEntity("Player");

	ASSERT_TRUE(e);
	EXPECT_TRUE(e.HasComponent<TransformComponent>());
	EXPECT_TRUE(e.HasComponent<NameComponent>());
	EXPECT_TRUE(e.HasComponent<UUIDComponent>());
	EXPECT_EQ(e.GetName(), "Player");
}

TEST_F(SceneTest, EmptyNameGetsGeneratedName)
{
	Entity e = scene.CreateEntity();
	EXPECT_TRUE(std::regex_match(e.GetName(), std::regex("Entity[0-9]+")));
}

TEST_F(SceneTest, EntitiesGetUniqueUuids)
{
	Entity a = scene.CreateEntity("A");
	Entity b = scene.CreateEntity("B");
	EXPECT_NE(a.GetID().ID, b.GetID().ID);
}

TEST_F(SceneTest, CreateEntityWithExplicitUuid)
{
	Entity e = scene.CreateEntity("A", "11111111-2222-3333-4444-555555555555");
	EXPECT_EQ(e.GetID().ID, "11111111-2222-3333-4444-555555555555");
}

TEST_F(SceneTest, GetEntityFromUuidFindsEntity)
{
	Entity a = scene.CreateEntity("A");
	Entity b = scene.CreateEntity("B");

	EXPECT_EQ(scene.GetEntityFromUUID(b.GetID()), b);
	EXPECT_NE(scene.GetEntityFromUUID(a.GetID()), b);
}

TEST_F(SceneTest, GetEntityFromUuidReturnsNullForUnknown)
{
	scene.CreateEntity("A");
	EXPECT_FALSE(scene.GetEntityFromUUID(UUID("does-not-exist")));
}

TEST_F(SceneTest, DestroyEntityRemovesItFromLookup)
{
	Entity e = scene.CreateEntity("A");
	UUID id = e.GetID();

	scene.DestroyEntity(e);
	EXPECT_FALSE(scene.GetEntityFromUUID(id));
}

TEST_F(SceneTest, DefaultSceneCtorSucceeds)
{
	Scene unnamed;
	EXPECT_TRUE(unnamed.CreateEntity("A"));
}

TEST_F(SceneTest, PrimaryCameraEntityIsFound)
{
	EXPECT_FALSE(scene.GetPrimaryCameraEntity());

	Entity plain = scene.CreateEntity("Plain");
	Entity cam = scene.CreateEntity("Camera");
	cam.AddComponent<CameraComponent>().Primary = true;

	EXPECT_EQ(scene.GetPrimaryCameraEntity(), cam);
	EXPECT_NE(scene.GetPrimaryCameraEntity(), plain);
}

TEST_F(SceneTest, ViewportResizeSkipsFixedAspectCameras)
{
	Entity resizable = scene.CreateEntity("Resizable");
	auto& rc = resizable.AddComponent<CameraComponent>();
	rc.FixedAspectRatio = false;

	Entity fixed = scene.CreateEntity("Fixed");
	auto& fc = fixed.AddComponent<CameraComponent>();
	fc.FixedAspectRatio = true;
	const glm::mat4 before = fc.Camera.GetProjection();

	scene.OnViewportResize(1000, 500);

	EXPECT_EQ(fixed.GetComponent<CameraComponent>().Camera.GetProjection(), before);
	EXPECT_NE(resizable.GetComponent<CameraComponent>().Camera.GetProjection(), before);
}

TEST(Entity, DefaultEntityIsNull)
{
	Entity e;
	EXPECT_FALSE(e);
}

TEST_F(SceneTest, AddOrReplaceAndRemoveComponent)
{
	Entity e = scene.CreateEntity("A");

	e.AddOrReplaceComponent<SpriteRendererComponent>(glm::vec4(1, 0, 0, 1));
	ASSERT_TRUE(e.HasComponent<SpriteRendererComponent>());
	EXPECT_EQ(e.GetComponent<SpriteRendererComponent>().Color, glm::vec4(1, 0, 0, 1));

	e.AddOrReplaceComponent<SpriteRendererComponent>(glm::vec4(0, 1, 0, 1));
	EXPECT_EQ(e.GetComponent<SpriteRendererComponent>().Color, glm::vec4(0, 1, 0, 1));

	e.RemoveComponent<SpriteRendererComponent>();
	EXPECT_FALSE(e.HasComponent<SpriteRendererComponent>());
}

TEST_F(SceneTest, GetOrAddComponentIsIdempotent)
{
	Entity e = scene.CreateEntity("A");
	auto& first = e.GetOrAddComponent<SpriteRendererComponent>();
	first.TilingFactor = 3.0f;
	EXPECT_FLOAT_EQ(e.GetOrAddComponent<SpriteRendererComponent>().TilingFactor, 3.0f);
}
