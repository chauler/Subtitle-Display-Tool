#include "gtest/gtest.h"
#include "WindowManager.h"
#include "Parser.h"

class ParserTest : public testing::Test {
protected:
	ParserTest(): wm(), p1(wm) {

	}

	WindowManager wm;
	Parser p1;
};

TEST_F(ParserTest, SimpleMode) {
	EXPECT_EQ(10, 10);
}