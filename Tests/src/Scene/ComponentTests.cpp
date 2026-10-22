#include <gtest/gtest.h>

#include "Vesper/Scene/Components.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Vesper;

TEST(Components, UuidGeneratedWhenDefaultConstructed)
{
	UUID a, b;
	EXPECT_FALSE(a.ID.empty());
	EXPECT_NE(a.ID, b.ID);
}

TEST(Components, UuidConvertsToString)
{
	UUID id("abc");
	const std::string& s = id;
	EXPECT_EQ(s, "abc");
}

TEST(Components, NameComponent)
{
	NameComponent n("Hero");
	EXPECT_EQ(n.GetName(), "Hero");
	const std::string& s = n;
	EXPECT_EQ(s, "Hero");
}

TEST(Components, TransformDefaultsToIdentity)
{
	TransformComponent t;
	EXPECT_EQ(t.GetTransform(), glm::mat4(1.0f));
}

TEST(Components, TransformComposesTranslationAndScale)
{
	TransformComponent t({ 1.0f, 2.0f, 3.0f });
	t.Scale = { 2.0f, 2.0f, 2.0f };

	const glm::mat4 expected = glm::translate(glm::mat4(1.0f), { 1.0f, 2.0f, 3.0f })
		* glm::scale(glm::mat4(1.0f), { 2.0f, 2.0f, 2.0f });
	EXPECT_EQ(t.GetTransform(), expected);
}

TEST(Components, SpriteRendererDefaults)
{
	SpriteRendererComponent s;
	EXPECT_EQ(s.Color, glm::vec4(1.0f));
	EXPECT_EQ(s.Texture, nullptr);
	EXPECT_FLOAT_EQ(s.TilingFactor, 1.0f);
	EXPECT_FALSE(s.TextureEnabled);
	EXPECT_FALSE(s.Billboard);
}

TEST(Components, SpriteRendererColorCtor)
{
	SpriteRendererComponent s(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	const glm::vec4& c = s;
	EXPECT_EQ(c, glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
}

TEST(TextureAnimationComponent, FrameBookkeeping)
{
	TextureAnimationComponent anim;
	anim.AddFrame(nullptr);
	anim.AddFrame(nullptr);
	EXPECT_EQ(anim.FrameCount, 2);

	anim.AddFrameAt(nullptr, 0);
	EXPECT_EQ(anim.FrameCount, 3);

	anim.RemoveFrameAt(1);
	EXPECT_EQ(anim.FrameCount, 2);

	anim.RemoveFrameAt(99);
	EXPECT_EQ(anim.FrameCount, 2);
}

TEST(TextureAnimationComponent, UpdateAdvancesAndWraps)
{
	TextureAnimationComponent anim;
	anim.FrameTime = 0.5f;
	anim.AddFrame(nullptr);
	anim.AddFrame(nullptr);
	anim.AddFrame(nullptr);

	anim.Update(0.25f);
	EXPECT_EQ(anim.GetCurrentFrame(), 0u);

	anim.Update(0.25f);
	EXPECT_EQ(anim.GetCurrentFrame(), 1u);

	anim.Update(1.0f);
	EXPECT_EQ(anim.GetCurrentFrame(), 0u);
}

TEST(TextureAnimationComponent, UpdateWithoutFramesIsNoOp)
{
	TextureAnimationComponent anim;
	anim.Update(10.0f);
	EXPECT_EQ(anim.GetCurrentFrame(), 0u);
}

TEST(TextureAnimationComponent, ResetWithNullTextureKeepsState)
{
	TextureAnimationComponent anim;
	anim.AddFrame(nullptr);
	anim.Reset(nullptr, 5, { 0, 0 }, { 1, 1 }, { 1, 1 });
	EXPECT_EQ(anim.FrameCount, 1);
}
