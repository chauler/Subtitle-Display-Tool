#include "util\util.h"
#include "Parser.h"

using json = nlohmann::json; //For convenience

bool AdvanceLine(std::ifstream& file, std::string& line, std::streampos& prevLine) {
	prevLine = file.tellg();
	return (bool)std::getline(file, line);
}

Parser::Parser(WindowManager& wm) : m_windowManager(wm) {}

void Parser::Parse(const std::string& input) {
	json input_data;

	try {
		input_data = json::parse(input);
	}
	catch (...) { //Invalid JSON - we just ignore it and continue for now
		std::cout << "Invalid JSON" << std::endl;
		return;
	}

	//We can't guarantee that the host passed a valid message - we have to validate existence and type of each field we access
	if (!ValidateDataString(input_data, "mode") || !ValidateDataObject(input_data, "data")) {
		return;
	}

	//Process based on the mode specified
	const std::string mode = input_data["mode"];
	json data = input_data["data"];

	if (mode == "simple") {
		HandleSimpleMode(data);
	}
	else if (mode == "advanced") {
		HandleAdvancedMode(data);
	}
	else if (mode == "file") {
		HandleFileMode(data);
	}
	else if (mode == "command") {
		HandleCommandMode(data);
	}
	else {
		std::cout << "Unknown mode: " << mode << std::endl;
	}
}

void Parser::HandleSimpleMode(const json& data) {
	if (!ValidateDataString(data, "dialogue")) return;

	//Dialogue exists and is a string, create a Window object and pass it to AddWindow(). (The braces are an initializer, it knows we're creating a window and is the same as using the Window constructor)
	m_windowManager.AddWindow({ data["dialogue"].get<json::string_t>() });
}

void Parser::HandleAdvancedMode(const json& data) {
	if (!ValidateDataString(data, "dialogue")) return;

	std::string dialogue = data["dialogue"];
	Styles styles;

	if (ValidateDataObject(data, "styles")) {
		styles = ParseStyles(data["styles"]);
	}

	m_windowManager.AddWindow(Subtitle{ dialogue, styles });
}

void Parser::HandleFileMode(const json& data) {
	if (!ValidateDataString(data, "filePath")) return;

	std::string filepath = data["filePath"].get<std::string>();

	std::ifstream file(filepath);
	
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filepath << std::endl;
		return;
	}

	std::string fileType = file_extension(string_base_name(filepath));
	if (fileType == "ssa" || "ass") {
		ParseSSA(file);
		return;
	}
	else if(fileType == "vtt") {

	}
	else if (fileType == "srt") {

	}

	std::string line;
	bool isVTT = false;
	std::vector<std::tuple<double, double, std::string>> subtitles;
	std::string currentSubtitle;
	double startTime = 0.0;
	double endTime = 0.0;

	while (std::getline(file, line)) {
		if (line.find("WEBVTT") != std::string::npos) {
			isVTT = true;
			continue;
		}

		if (line.empty()) {
			if (!currentSubtitle.empty()) {
				subtitles.push_back(std::make_tuple(startTime, endTime, currentSubtitle));
				currentSubtitle.clear();
			}
			continue;
		}

		if (line.find("-->") != std::string::npos) {
			std::istringstream timestampStream(line);
			std::string startTimestamp, endTimestamp;
			timestampStream >> startTimestamp;
			timestampStream.ignore(4);
			timestampStream >> endTimestamp;

			startTime = ParseTimeStamp(startTimestamp);
			endTime = ParseTimeStamp(endTimestamp);
			std::cout << "Parsed Timestamps: Start - " << std::fixed << std::setprecision(3) << startTime << " End - " << endTime << std::endl;
		} else {
			if (!currentSubtitle.empty()) {
				currentSubtitle += "\n";
			}
			currentSubtitle += line;
		}
	}

	if (!currentSubtitle.empty()) {
		subtitles.push_back(std::make_tuple(startTime, endTime, currentSubtitle));
	}

	for (const auto& subtitle : subtitles) {
		double lifetime = std::get<1>(subtitle) - std::get<0>(subtitle);

		std::cout << "Adding Subtitle: \"" << std::get<2>(subtitle) << "\" with lifetime: " << lifetime << " second" << std::endl;

		Styles styles;
		styles.lifetime = lifetime;
		Subtitle sub(std::get<2>(subtitle), styles, std::get<0>(subtitle));
		Window window(sub);
		m_windowManager.AddWindow(std::move(window));
	}	
}

void Parser::HandleCommandMode(const nlohmann::json& data)
{
	if (!ValidateDataString(data, "command")) return;
	
	if (data["command"] == "UpdateWindowTransform") {
		m_windowManager.hostX = data["x"].get<int>();
		m_windowManager.hostY = data["y"].get<int>();
		m_windowManager.hostWidth = data["width"].get<int>();
		m_windowManager.hostHeight = data["height"].get<int>();
	}
}

Styles Parser::ParseStyles(const json& stylesData) {
	Styles styles;

	if (stylesData.contains("fontSize") && stylesData["fontSize"].is_number()) {
		styles.fontSize = stylesData["fontSize"];
	}
	if (stylesData.contains("position") && stylesData["position"].is_array() && stylesData["position"].size() == 2) {
		styles.position = { stylesData["position"][0], stylesData["position"][1] };
	}
	if (stylesData.contains("fontColor") && stylesData["fontColor"].is_array() && stylesData["fontColor"].size() == 4) {
		styles.fontColor = { stylesData["fontColor"][0], stylesData["fontColor"][1], stylesData["fontColor"][2], stylesData["fontColor"][3] };
	}
	if (stylesData.contains("bgColor") && stylesData["bgColor"].is_array() && stylesData["bgColor"].size() == 4) {
		styles.bgColor = { stylesData["bgColor"][0], stylesData["bgColor"][1], stylesData["bgColor"][2], stylesData["bgColor"][3] };
	}
	if (stylesData.contains("fontPath") && stylesData["fontPath"].is_string()) {
		styles.fontPath = stylesData["fontPath"];
	}
	if (stylesData.contains("lifetime") && stylesData["lifetime"].is_number()) {
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
	if (stylesData.contains("shadow") && stylesData["shadow"].is_object()) {
		json outlineData = stylesData["shadow"];
		if (outlineData.contains("color") && outlineData["color"].is_array() && outlineData["color"].size() == 3) {
			styles.shadow.color.r = outlineData["color"][0];
			styles.shadow.color.g = outlineData["color"][1];
			styles.shadow.color.b = outlineData["color"][2];
		}
		if (outlineData.contains("offset") && outlineData["offset"].is_array() && outlineData["offset"].size() == 2) {
			styles.shadow.offset.x = outlineData["offset"][0];
			styles.shadow.offset.y = outlineData["offset"][1];
		}
		if (outlineData.contains("blurStrength") && outlineData["blurStrength"].is_number()) {
			styles.shadow.blurStrength = outlineData["blurStrength"];
		}
	}
	return styles;
}

double Parser::ParseTimeStamp(const std::string& timestamp) {
	int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
	char sep1, sep2, sep3;

	std::istringstream stream(timestamp);
	stream >> hours >> sep1 >> minutes >> sep2 >> seconds;
	if (stream.peek() == '.') {
		stream >> sep3 >> milliseconds;
	}

	return hours * 3600 + minutes * 60 + seconds + milliseconds / 1000.0;
}

bool Parser::ValidateDataString(const json& data, const std::string& key) {
	if (!data.contains(key) || !data[key].is_string()) {
		std::cout << "Invalid message structure: missing or invalid" << key << std::endl;
		return false;
	}
	return true;
}

bool Parser::ValidateDataObject(const json& data, const std::string& key) {
	if (!data.contains(key) || !data[key].is_object()) {
		std::cout << "Invalid message structure: missing or invalid" << key << std::endl;
		return false;
	}
	return true;
}

void Parser::ParseSSA(std::ifstream& file) {
	std::string line;

	std::getline(file, line);
	trim(line);
	if (line.find("[Script Info]")) {
		_ScriptInfo(file);
	}
	else {
		std::cout << "Invalid SSA/ASS file." << std::endl;
		return;
	}

	std::getline(file, line);
	if (line == "[V4+ Styles]") {
		_V4Styles(file);
	}

	std::getline(file, line);
	if (line == "[Events]") {
		_Events(file);
	}

	std::getline(file, line);
	if (line == "[Fonts]") {
		_Fonts(file);
	}

	std::getline(file, line);
	if (line == "[Graphics]") {
		_Graphics(file);
	}
}

void Parser::_ScriptInfo(std::ifstream& file) {
	//TEMPORARY: IMPLEMENT LATER
	std::string line;
	std::streampos prevLine;
	while (AdvanceLine(file, line, prevLine)) {
		if (line.empty()) { //We found the next section: return
			return;
		}
	}
}

void Parser::_V4Styles(std::ifstream& file) {
	//TEMPORARY: IMPLEMENT LATER
	std::string line;
	std::streampos prevLine;
	while (AdvanceLine(file, line, prevLine)) {
		if (line.empty()) { //We found the next section: return
			return;
		}
	}
}

void Parser::_Events(std::ifstream& file) {
	double creationTime = GetTime();
	std::string line, token;
	std::streampos prevLine;

	//Events section always starts with a Format line containing comma-separated fields
	AdvanceLine(file, line, prevLine);
	std::istringstream lineStream(line);
	lineStream.ignore(10, ':'); //Line starts with 'Format:'. skip it

	//Parse presence and order of fields for the dialogue lines
	std::vector<FormatField> presentFields{};
	while (std::getline(lineStream, token, ',')) {
		ltrim(token);
		if (token == "Marked") {
			presentFields.push_back(FormatField::Marked);
		}
		else if (token == "Layer") {
			presentFields.push_back(FormatField::Layer);
		}
		else if (token == "Start") {
			presentFields.push_back(FormatField::Start);
		}
		else if (token == "End") {
			presentFields.push_back(FormatField::End);
		}
		else if (token == "Style") {
			presentFields.push_back(FormatField::Style);
		}
		else if (token == "Name") {
			presentFields.push_back(FormatField::Name);
		}
		else if (token == "MarginL") {
			presentFields.push_back(FormatField::MarginL);
		}
		else if (token == "MarginR") {
			presentFields.push_back(FormatField::MarginR);
		}
		else if (token == "MarginV") {
			presentFields.push_back(FormatField::MarginV);
		}
		else if (token == "Effect") {
			presentFields.push_back(FormatField::Effect);
		}
		else if (token == "Text") {
			presentFields.push_back(FormatField::Text);
		}
	}

	//Parse the event lines using the defined order of fields
	while (AdvanceLine(file, line, prevLine)) {
		std::istringstream event(line);
		//Get Event type
		std::getline(event, token, ':');
		if (event.fail()) { //Line descriptor not present. Assume end of section.
			break;
		}

		if (token == "Dialogue") {
			double start = 0, end = 0;
			std::string dialogue;
			Styles styles;
			for (const FormatField& field : presentFields) {
				if (field == FormatField::Text) {
					std::getline(event, token);
				}
				else {
					std::getline(event, token, ',');
				}
				std::istringstream tokenStream(token);
				switch (field) {
				case FormatField::Marked:
					break;
				case FormatField::Layer:
					break;
				case FormatField::Start:
					start = ParseTimeStamp(token);
					break;
				case FormatField::End:
					end = ParseTimeStamp(token);
					break;
				case FormatField::Style:
					break;
				case FormatField::Name:
					break;
				case FormatField::MarginL:
					break;
				case FormatField::MarginR:
					break;
				case FormatField::Effect:
					break;
				case FormatField::Text:
					bool styleOverride = false;
					bool drawing = false;
					for (int i = 0; i < token.length(); i++) {
						if (token[i] == '{') {
							styleOverride = true;
						}
						else if (styleOverride && token[i] == '}') {
							styleOverride = false;
							continue;
						}
						if (styleOverride) {
							//Check for drawing commands
							if (token.length() > i + 2 && token[i] == '\\' && token[i+1] == 'p' && isdigit(token[i + 2]) && token[i + 2] != '0') {
								drawing = true;
							}
							else if (token.length() > i + 2 && token[i] == '\\' && token[i + 1] == 'p' && token[i + 2] == '0') {
								drawing = false;
							}
						}
						if(!styleOverride && !drawing) {
							dialogue.push_back(token[i]);
						}
					}
					break;
				}
			}
			if (dialogue.empty()) {
				continue;
			}
			styles.lifetime = end - start;
			Subtitle sub(dialogue, styles, start);
			Window window(sub);
			m_windowManager.AddWindow(std::move(window));
		}
		else if (token == "Comment") {

		}
		else if (token == "Picture") {

		}
		else if (token == "Sound") {

		}
		else if (token == "Movie") {

		}
		else if (token == "Command") {

		}

	}
}

void Parser::_Fonts(std::ifstream& file) {
	//TEMPORARY: IMPLEMENT LATER
	std::string line;
	std::streampos prevLine;
	while (AdvanceLine(file, line, prevLine)) {
		if (line.empty()) { //We found the next section: return
			return;
		}
	}
}

void Parser::_Graphics(std::ifstream& file) {
	//TEMPORARY: IMPLEMENT LATER
	std::string line;
	std::streampos prevLine;
	while (AdvanceLine(file, line, prevLine)) {
		if (line.empty()) { //We found the next section: return
			return;
		}
	}
}
