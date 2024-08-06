#pragma once
#include <string>
#include "WindowManager.h"

class Parser {
public:
	Parser(WindowManager&);
	void Parse(const std::string& input);
private:
	WindowManager& m_windowManager;
};