#include <gtest/gtest.h>

#include "Vesper/Core/Math.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace Vesper;

namespace {

	void ExpectVec3Near(const glm::vec3& a, const glm::vec3& b, float eps = 1e-4f)
	{
		EXPECT_NEAR(a.x, b.x, eps);
		EXPECT_NEAR(a.y, b.y, eps);
		EXPECT_NEAR(a.z, b.z, eps);
	}

}

TEST(Math, DecomposeIdentity)
{
	glm::vec3 t, r, s;
	ASSERT_TRUE(Math::DecomposeTransform(glm::mat4(1.0f), t, r, s));
	ExpectVec3Near(t, { 0, 0, 0 });
	ExpectVec3Near(r, { 0, 0, 0 });
	ExpectVec3Near(s, { 1, 1, 1 });
}

TEST(Math, DecomposeTranslationAndScale)
{
	glm::mat4 m = glm::translate(glm::mat4(1.0f), { 1.0f, 2.0f, 3.0f })
		* glm::scale(glm::mat4(1.0f), { 2.0f, 3.0f, 4.0f });

	glm::vec3 t, r, s;
	ASSERT_TRUE(Math::DecomposeTransform(m, t, r, s));
	ExpectVec3Near(t, { 1, 2, 3 });
	ExpectVec3Near(s, { 2, 3, 4 });
	ExpectVec3Near(r, { 0, 0, 0 });
}

TEST(Math, DecomposeRotationRoundTrip)
{
	const glm::vec3 rotation{ 0.3f, -0.5f, 0.8f };
	glm::mat4 m = glm::translate(glm::mat4(1.0f), { -4.0f, 5.0f, 6.0f })
		* glm::toMat4(glm::quat(rotation))
		* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.5f });

	glm::vec3 t, r, s;
	ASSERT_TRUE(Math::DecomposeTransform(m, t, r, s));
	ExpectVec3Near(t, { -4, 5, 6 });
	ExpectVec3Near(r, rotation);
	ExpectVec3Near(s, { 1.5f, 1.5f, 1.5f });
}

TEST(Math, DecomposeFailsOnZeroW)
{
	glm::mat4 m(1.0f);
	m[3][3] = 0.0f;

	glm::vec3 t, r, s;
	EXPECT_FALSE(Math::DecomposeTransform(m, t, r, s));
}
