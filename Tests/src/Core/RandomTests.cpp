#include <gtest/gtest.h>

#include "Vesper/Core/Random.h"

#include <regex>

using namespace Vesper;

TEST(Random, SeedMakesSequenceDeterministic)
{
	Random::Seed(1234);
	const uint32_t a = Random::UInt1(1000000);
	const float b = Random::Float1();

	Random::Seed(1234);
	EXPECT_EQ(a, Random::UInt1(1000000));
	EXPECT_EQ(b, Random::Float1());
}

TEST(Random, UInt1StaysBelowMax)
{
	for (int i = 0; i < 1000; ++i)
		EXPECT_LT(Random::UInt1(10), 10u);
}

TEST(Random, Float1InUnitRange)
{
	for (int i = 0; i < 1000; ++i)
	{
		const float f = Random::Float1();
		EXPECT_GE(f, 0.0f);
		EXPECT_LE(f, 1.0f);
	}
}

TEST(Random, RangeF1RespectsBounds)
{
	for (int i = 0; i < 1000; ++i)
	{
		const float f = Random::RangeF1(-5.0f, 5.0f);
		EXPECT_GE(f, -5.0f);
		EXPECT_LE(f, 5.0f);
	}
}

TEST(Random, RangeF1SwapsReversedBounds)
{
	for (int i = 0; i < 100; ++i)
	{
		const float f = Random::RangeF1(5.0f, -5.0f);
		EXPECT_GE(f, -5.0f);
		EXPECT_LE(f, 5.0f);
	}
}

TEST(Random, RangeF2AndF3RespectBounds)
{
	for (int i = 0; i < 100; ++i)
	{
		const glm::vec2 v2 = Random::RangeF2(1.0f, 2.0f);
		EXPECT_GE(v2.x, 1.0f);
		EXPECT_LE(v2.y, 2.0f);

		const glm::vec3 v3 = Random::RangeF3(-1.0f, 0.0f);
		EXPECT_GE(v3.z, -1.0f);
		EXPECT_LE(v3.z, 0.0f);
	}
}

TEST(Random, Bool1Extremes)
{
	for (int i = 0; i < 100; ++i)
	{
		EXPECT_FALSE(Random::Bool1(0.0f));
		EXPECT_TRUE(Random::Bool1(1.1f));
	}
}

TEST(Random, StringHasLengthAndCharset)
{
	const std::string s = Random::String(64);
	EXPECT_EQ(s.size(), 64u);
	EXPECT_TRUE(std::regex_match(s, std::regex("[0-9A-Za-z]+")));
}

TEST(Random, HexStringUsesUppercaseHex)
{
	const std::string s = Random::HexString(32);
	EXPECT_EQ(s.size(), 32u);
	EXPECT_TRUE(std::regex_match(s, std::regex("[0-9A-F]+")));
}

TEST(Random, UuidFormat)
{
	const std::string id = Random::UUID();
	EXPECT_TRUE(std::regex_match(id, std::regex("[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}")));
	EXPECT_NE(id, Random::UUID());
}
