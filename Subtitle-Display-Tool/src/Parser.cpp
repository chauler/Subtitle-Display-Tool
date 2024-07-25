#include "Parser.h"
#include "nlohmann\json.hpp"
#include <iostream>

using json = nlohmann::json; //For convenience

Parser::Parser(WindowManager& wm) : m_windowManager(wm)
{
}

void Parser::Parse(const std::string& input)
{
	json input_data;
	try {
		input_data = json::parse(input);
	}
	catch (...) { //Invalid JSON - we just ignore it and continue for now
		std::cout << "Invalid JSON" << std::endl;
		return;
	}

	//We can't guarantee that the host passed a valid message - we have to validate existence and type of each field we access
	if (!input_data.contains("mode") || !input_data["mode"].is_string() || !input_data.contains("data") || !input_data["data"].is_object()) {
		std::cout << "Invalid message structure" << std::endl;
		return;
	}

	//Grab a reference to the data field since we'll be accessing it a lot
	json data = input_data["data"];

	if (input_data["mode"] == "simple") {
		if (!data.contains("dialogue") || !data["dialogue"].is_string()) {
			std::cout << "Invalid message structure" << std::endl;
			return;
		}
		//Dialogue exists and is a string, create a Window object and pass it to AddWindow(). (The braces are an initializer, it knows we're creating a window and is the same as using the Window constructor)
		m_windowManager.AddWindow({ data["dialogue"].get<json::string_t>() });
	}
	else if (input_data["mode"] == "advanced") {
		if (!data.contains("dialogue") || !data["dialogue"].is_string()) {
			std::cout << "Invalid message structure" << std::endl;
			return;
		}

		Styles styles;

		if (data.contains("styles") && data["styles"].is_object()) {
			json stylesData = data["styles"];

			if (stylesData.contains("fontSize") && stylesData["fontSize"].is_number()) {
				styles.fontSize = stylesData["fontSize"];
			}
			if (stylesData.contains("position") && stylesData["position"].is_array() && stylesData["position"].size() == 2 ) {
				styles.position = { stylesData["position"][0], stylesData["position"][1] };
			}
			if (stylesData.contains("fontColor") && stylesData["fontColor"].is_array() && stylesData["fontColor"].size() == 4) {
				styles.fontColor = { stylesData["fontColor"][0], stylesData["fontColor"][1], stylesData["fontColor"][2], stylesData["fontColor"][3] };
			}
			if (stylesData.contains("bgColor") && stylesData["bgColor"].is_array() && stylesData["bgColor"].size() == 4 ) {
				styles.bgColor= { stylesData["bgColor"][0], stylesData["bgColor"][1], stylesData["bgColor"][2], stylesData["bgColor"][3] };
			}
			if (stylesData.contains("fontPath") && stylesData["fontPath"].is_string()) {
				styles.fontPath = stylesData["fontPath"];
			}
			if (stylesData.contains("lifetime") && stylesData["lifetime"].is_number_float()) {
				styles.lifetime = stylesData["lifetime"];
			}
			if (stylesData.contains("outline") && stylesData["outline"].is_object()) {
				json outlineData = stylesData["outline"];
				if (outlineData.contains("color") && outlineData["color"].is_array() && outlineData["color"].size() == 4) {
					styles.outline.outlineColor.r = outlineData["color"][0];
					styles.outline.outlineColor.g = outlineData["color"][1];
					styles.outline.outlineColor.b = outlineData["color"][2];
					styles.outline.outlineColor.a = outlineData["color"][3];
				}
				if (outlineData.contains("size") && outlineData["size"].is_number()) {
					styles.outline.outlineSize = outlineData["size"];
				}
			}
		}

		//Dialogue exists and is a string, create a Window object with styles
		Window nw = Window({ data["dialogue"].get<json::string_t>(), styles });

		m_windowManager.AddWindow(nw);

	}
	else if (input_data["mode"] == "file") { //Realistically could assume all other cases ==file, but still checking as we may want to reject invalid values for some reason.

	}
}

