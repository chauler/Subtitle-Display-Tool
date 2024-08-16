#pragma once
#include "WindowManager.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

enum class FormatField : unsigned int {Marked, Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text};

class Parser {
public:
	Parser(WindowManager&);
	void Parse(const std::string& input);
private:
	WindowManager& m_windowManager;
	void HandleSimpleMode(const nlohmann::json& data);
	void HandleAdvancedMode(const nlohmann::json& data);
	void HandleFileMode(const nlohmann::json& data);
	void ParseSSA(std::ifstream& file);
	void ParseSRT(std::ifstream& file);
	void _ScriptInfo(std::ifstream& file);
	void _V4Styles(std::ifstream& file);
	void _Events(std::ifstream& file);
	void _Fonts(std::ifstream& file);
	void _Graphics(std::ifstream& file);
	void HandleCommandMode(const nlohmann::json& data);
	Styles ParseStyles(const nlohmann::json& data);
	double ParseTimeStamp(const std::string& timestamp);
	bool ValidateDataString(const nlohmann::json& data, const std::string& key);
	bool ValidateDataObject(const nlohmann::json& data, const std::string& key);
};