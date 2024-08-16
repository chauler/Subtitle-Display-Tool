#include "gtest/gtest.h"
#include "Subtitle.h"
#include "raylib.h"

class SubtitleTest : public testing::Test {
protected:
	SubtitleTest() {

	}

	std::string testString = "test string";
};

TEST_F(SubtitleTest, DefaultConstruction) {
	//While default values may change, ensure no value is 0-initialized unless explicit
	Subtitle testValue{ testString, {} };

	//Can't directly compare fonts, so reasonably check members
	EXPECT_EQ(testValue.GetFont().baseSize, GetFontDefault().baseSize);
	EXPECT_EQ(testValue.GetFont().glyphCount, GetFontDefault().glyphCount);
	EXPECT_EQ(testValue.GetDialogue(), testString);
	EXPECT_EQ(testValue.GetStartTime(), 0.0);
}

TEST_F(SubtitleTest, OutlineConstruction) {
	float colors[4]{ 1.0, 1.0, 1.0, 1.0 };
	Subtitle testValue{ testString, {.outline{ colors, 5 } } };
	OutlineSettings result = testValue.GetStyles().outline;
	EXPECT_EQ(result.outlineColor.r, 1.0);
	EXPECT_EQ(result.outlineColor.g, 1.0);
	EXPECT_EQ(result.outlineColor.b, 1.0);
	EXPECT_EQ(result.outlineColor.a, 1.0);
	EXPECT_EQ(result.outlineSize, 5);
}

TEST_F(SubtitleTest, OutlineNegativeColors) {
	float colors[4]{ -1.0 , -1.0 , -1.0 , -1.0 };
	Subtitle testValue{ testString, {.outline{ colors, 5 } } };
	OutlineSettings result = testValue.GetStyles().outline;
	EXPECT_EQ(result.outlineColor.r, 0.0);
	EXPECT_EQ(result.outlineColor.g, 0.0);
	EXPECT_EQ(result.outlineColor.b, 0.0);
	EXPECT_EQ(result.outlineColor.a, 0.0);
}

TEST_F(SubtitleTest, ShadowConstruction) {
	Subtitle testValue{ testString, {.shadow{.color{.values{1.0, 1.0, 1.0}}, .offset{.values{5, 5}}}} };
	DropShadowSettings result = testValue.GetStyles().shadow;
	EXPECT_EQ(result.color.r, 1.0);
	EXPECT_EQ(result.color.g, 1.0);
	EXPECT_EQ(result.color.b, 1.0);
	EXPECT_EQ(result.offset.x, 5);
	EXPECT_EQ(result.offset.y, 5);
}