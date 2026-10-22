#include <gtest/gtest.h>

#include "Vesper/Core/Color.h"
#include "Vesper/Core/Timestep.h"

using namespace Vesper;

TEST(Color, PrimaryColors)
{
	EXPECT_EQ(Color::White(), glm::vec4(1, 1, 1, 1));
	EXPECT_EQ(Color::Black(), glm::vec4(0, 0, 0, 1));
	EXPECT_EQ(Color::Red(), glm::vec4(1, 0, 0, 1));
	EXPECT_EQ(Color::Green(), glm::vec4(0, 1, 0, 1));
	EXPECT_EQ(Color::Blue(), glm::vec4(0, 0, 1, 1));
	EXPECT_EQ(Color::Transparent(), glm::vec4(0, 0, 0, 0));
}

TEST(Color, StripAlphaMakesOpaque)
{
	EXPECT_EQ(Color::StripAlpha({ 0.1f, 0.2f, 0.3f, 0.0f }), glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
}

TEST(Color, SetAlphaKeepsRgb)
{
	EXPECT_EQ(Color::SetAlpha(Color::Red(), 0.25f), glm::vec4(1, 0, 0, 0.25f));
	EXPECT_EQ(Color::SetAlpha(Color::Red()), glm::vec4(1, 0, 0, 0));
}

TEST(Timestep, DefaultsToZero)
{
	Timestep ts;
	EXPECT_FLOAT_EQ(ts.GetSeconds(), 0.0f);
}

TEST(Timestep, SecondsAndMilliseconds)
{
	Timestep ts(0.5f);
	EXPECT_FLOAT_EQ(ts.GetSeconds(), 0.5f);
	EXPECT_FLOAT_EQ(ts.GetMilliseconds(), 500.0f);
}

TEST(Timestep, ConvertsImplicitlyToFloat)
{
	Timestep ts(2.0f);
	const float f = ts;
	EXPECT_FLOAT_EQ(f, 2.0f);
}
