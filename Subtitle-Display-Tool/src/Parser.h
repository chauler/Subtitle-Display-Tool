#pragma once
#include "WindowManager.h"
#include "nlohmann\json.hpp"
#include <iostream>
#include <fstream>

class Parser {
public:
	Parser(WindowManager&);
	void Parse(const std::string& input);
private:
	WindowManager& m_windowManager;
	void HandleSimpleMode(const nlohmann::json& data);
	void HandleAdvancedMode(const nlohmann::json& data);
	void HandleFileMode(const nlohmann::json& data);
	Styles ParseStyles(const nlohmann::json& data);
	double ParseTimeStamp(const std::string& timestamp);
	bool ValidateDataString(const nlohmann::json& data, const std::string& key);
	bool ValidateDataObject(const nlohmann::json& data, const std::string& key);
};