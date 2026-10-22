#include <gtest/gtest.h>

#include "Vesper/Renderer/OrthographicCamera.h"
#include "Vesper/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Vesper;

namespace {

	void ExpectMat4Near(const glm::mat4& a, const glm::mat4& b, float eps = 1e-5f)
	{
		for (int c = 0; c < 4; ++c)
			for (int r = 0; r < 4; ++r)
				EXPECT_NEAR(a[c][r], b[c][r], eps) << "at [" << c << "][" << r << "]";
	}

}

TEST(OrthographicCamera, InitialProjectionMatchesGlmOrtho)
{
	OrthographicCamera cam(-1.6f, 1.6f, -0.9f, 0.9f);
	ExpectMat4Near(cam.GetProjectionMatrix(), glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f));
	ExpectMat4Near(cam.GetViewMatrix(), glm::mat4(1.0f));
	ExpectMat4Near(cam.GetViewProjectionMatrix(), cam.GetProjectionMatrix());
}

TEST(OrthographicCamera, SetPositionUpdatesViewMatrix)
{
	OrthographicCamera cam(-1, 1, -1, 1);
	cam.SetPosition({ 2.0f, 3.0f, 0.0f });

	EXPECT_EQ(cam.GetPosition(), glm::vec3(2.0f, 3.0f, 0.0f));
	ExpectMat4Near(cam.GetViewMatrix(), glm::translate(glm::mat4(1.0f), { -2.0f, -3.0f, 0.0f }));
	ExpectMat4Near(cam.GetViewProjectionMatrix(), cam.GetProjectionMatrix() * cam.GetViewMatrix());
}

TEST(OrthographicCamera, SetRotationUpdatesViewMatrix)
{
	OrthographicCamera cam(-1, 1, -1, 1);
	cam.SetRotation(90.0f);

	EXPECT_FLOAT_EQ(cam.GetRotation(), 90.0f);
	glm::mat4 expected = glm::inverse(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 0, 0, 1 }));
	ExpectMat4Near(cam.GetViewMatrix(), expected);
}

TEST(OrthographicCamera, SetProjectionKeepsView)
{
	OrthographicCamera cam(-1, 1, -1, 1);
	cam.SetPosition({ 1.0f, 0.0f, 0.0f });
	cam.SetProjection(-2, 2, -2, 2);

	ExpectMat4Near(cam.GetProjectionMatrix(), glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f));
	ExpectMat4Near(cam.GetViewProjectionMatrix(), cam.GetProjectionMatrix() * cam.GetViewMatrix());
}

TEST(SceneCamera, DefaultsToOrthographic)
{
	SceneCamera cam;
	EXPECT_EQ(cam.GetProjectionType(), SceneCamera::ProjectionType::Orthographic);
	EXPECT_FLOAT_EQ(cam.GetOrthographicSize(), 10.0f);
}

TEST(SceneCamera, OrthographicProjectionUsesAspectRatio)
{
	SceneCamera cam;
	cam.SetOrthographic(10.0f, -1.0f, 1.0f);
	cam.SetViewportSize(200, 100);

	ExpectMat4Near(cam.GetProjection(), glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, -1.0f, 1.0f));
}

TEST(SceneCamera, SetPerspectiveSwitchesProjectionType)
{
	SceneCamera cam;
	cam.SetViewportSize(160, 90);
	cam.SetPerspective(glm::radians(60.0f), 0.1f, 100.0f);

	EXPECT_EQ(cam.GetProjectionType(), SceneCamera::ProjectionType::Perspective);
	EXPECT_FLOAT_EQ(cam.GetPerspectiveNearClip(), 0.1f);
	EXPECT_FLOAT_EQ(cam.GetPerspectiveFarClip(), 100.0f);
	ExpectMat4Near(cam.GetProjection(), glm::perspective(glm::radians(60.0f), 160.0f / 90.0f, 0.1f, 100.0f));
}

TEST(SceneCamera, SettersRecalculateProjection)
{
	SceneCamera cam;
	cam.SetViewportSize(100, 100);
	cam.SetOrthographicSize(4.0f);
	ExpectMat4Near(cam.GetProjection(), glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f));
}
