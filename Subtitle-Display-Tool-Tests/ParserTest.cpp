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

TEST_F(ParserTest, ParserInvalidJson) {
	std::string invalidJson = "{ invalid json";
	EXPECT_NO_THROW(p1.Parse(invalidJson));
}

TEST_F(ParserTest, SimpleModeCreatesWindow) {
	std::string simpleJson = R"({"mode": "simple", "data": {"dialogue": "Hello, World!"}})";
	p1.Parse(simpleJson);

	std::vector<Window>& windows = GetWindows(wm);
	ASSERT_EQ(windows.size(), 1);
}

TEST_F(ParserTest, AdvancedModeCreatesWindow) {
	std::string advancedJson = R"({"mode": "advanced", "data": {"dialogue": "Advanced Dialogue", "styles": {"fontSize": 18}}})";
	p1.Parse(advancedJson);

	std::vector<Window>& windows = GetWindows(wm);
	ASSERT_EQ(windows.size(), 1);
}

static TEST_F(ParserTest, CommandModeDoesNotCreateWindow) {
	std::string commandJson = R"({"mode": "command", "data": {"command": "UpdateWindowTransform", "x": 100, "y": 200, "width": 1280, "height": 720}})";
	p1.Parse(commandJson);

	std::vector<Window>& windows = GetWindows(wm);
	ASSERT_EQ(windows.size(), 0);
}

static TEST_F(ParserTest, FileModeCreatesWindowsFromVTTFile) {
	std::ofstream testFile("test.vtt");
	testFile << "WEBVTT\n\n"
		<< "00:00:01.000 --> 00:00:04.000\n"
		<< "Hello, VTT!\n";
	testFile.close();

	std::string fileJson = R"({"mode": "file", "data": {"filePath": "test.vtt"}})";
	p1.Parse(fileJson);

	std::vector<Window>& windows = GetWindows(wm);
	ASSERT_EQ(windows.size(), 1);

	std::remove("test.vtt");
}

static TEST_F(ParserTest, FileModeCreatesWindowsFromSRTFile) {
	std::ofstream testFile("test.srt");
	testFile << "1\n00:00:01,000 --> 00:00:04,000\nHello, SRT!\n\n";
	testFile << "2\n00:00:04,001 --> 00:00:05,000\nGoodbye, SRT!\n\n";
	testFile.close();

	std::string fileJson = R"({"mode": "file", "data": {"filePath": "test.srt"}})";
	p1.Parse(fileJson);

	std::vector<Window>& windows = GetWindows(wm);
	ASSERT_EQ(windows.size(), 2);

	std::remove("test.srt");
}