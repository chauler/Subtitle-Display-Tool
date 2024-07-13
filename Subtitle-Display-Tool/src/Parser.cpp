#include "Parser.h"
#include "nlohmann\json.hpp"
#include <iostream>

using json = nlohmann::json;

Parser::Parser(WindowManager& wm) : m_windowManager(wm)
{
}

void Parser::Parse(const std::string& input)
{
	json input_data;
	try {
		input_data = json::parse(input);
	}
	catch (...) { //Invalid JSON
		std::cout << "Invalid JSON" << std::endl;
		return;
	}

	if (!input_data.contains("mode") || !input_data["mode"].is_string() || !input_data.contains("data") || !input_data["data"].is_object()) {
		std::cout << "Invalid message structure" << std::endl;
		return;
	}
	json data = input_data["data"];
	if (input_data["mode"] == "simple") {
		if (!data.contains("dialogue") || !data["dialogue"].is_string()) {
			std::cout << "Invalid message structure" << std::endl;
		}
		m_windowManager.AddWindow({ data["dialogue"].get<json::string_t>()});
	}
	else if (input_data["mode"] == "advanced") {

	}
	else if (input_data["mode"] == "file") { //Realistically could assume all other cases ==file, but still checking as we may want to reject invalid values for some reason.

	}
}

/*
Currently proposed API format:
* denotes required property
{
	"*mode": "simple" | "advanced" | "file",
	"*data": {
		"*dialogue": "string",	//This property will exist in all modes, but with differing a differing function. If mode == simple or advanced, this will be the text displayed on screen. 
								//If mode == file, it'll be a filepath for the file to be parsed.
		"styles": {
			//Various properties that are currently undecided. Should be easy to add/remove from this as the Styles constructor allows for any combination of arguments in its constructor.
		},
	}
}
*/