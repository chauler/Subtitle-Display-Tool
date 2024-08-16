#pragma once
#include "WindowManager.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

/**
* Enum representing various fields in .ssa/.ass formats
*/
enum class FormatField : unsigned int {Marked, Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text};

/**
* Class responsible for parsing different subtitle formats and interacting with the WindowManager.
* @param wm: Reference to the WindowManager instance.
*/
class Parser {
public:
	Parser(WindowManager&);
	/**
	* Parses the provided JSON input string.
    * @param input: JSON string containing mode and data for parsing.
	*/
	void Parse(const std::string& input);
private:
	WindowManager& m_windowManager;///< Reference to the WindowManager
	/**
	* Handles the parsing and processing of 'simple' mode.
    * @param data: JSON data containing the dialogue.
	*/
	void HandleSimpleMode(const nlohmann::json& data);
	/**
	* Handles the parsing and processing of 'advanced' mode.
    * @param data: JSON data containing dialogue and styles.
	*/
	void HandleAdvancedMode(const nlohmann::json& data);
	/**
	* Handles the parsing and processing of 'file' mode.
    * @param data: JSON data containing the file path.
	*/
	void HandleFileMode(const nlohmann::json& data);
	/**
	* Parses SSA (SubStation Alpha) subtitle files.
    * @param file: Input file stream of the SSA file.
	*/
	void ParseSSA(std::ifstream& file);
	/**
	* Parses SRT (SubRip Subtitle) files.
    * @param file: Input file stream of the SRT file.
	*/
	void ParseSRT(std::ifstream& file);
	/**
	* Parses VTT (Web Video Text Tracks) files.
    * @param file: Input file stream of the VTT file.
	*/
	void ParseVTT(std::ifstream& file);
	/**
	* Handles the parsing of SSA [Script Info] section.
    * @param file: Input file stream.
	*/
	void _ScriptInfo(std::ifstream& file);
	/**
	* Handles the parsing of SSA [V4+ Styles] section.
    * @param file: Input file stream.
	*/
	void _V4Styles(std::ifstream& file);
	/**
	* Handles the parsing of SSA [Events] section.
    * @param file: Input file stream.
	*/
	void _Events(std::ifstream& file);
	/**
	* Handles the parsing of SSA [Fonts] section.
    * @param file: Input file stream.
	*/
	void _Fonts(std::ifstream& file);
	/**
	* Handles the parsing of SSA [Graphics] section.
    * @param file: Input file stream.
	*/
	void _Graphics(std::ifstream& file);
	/**
	* Handles the parsing and processing of 'command' mode.
    * @param data: JSON data containing the command and its parameters.
	*/
	void HandleCommandMode(const nlohmann::json& data);
	/**
	* Parses the styles from the provided JSON data.
    * @param stylesData: JSON data containing style information.
    * @return Parsed Styles object.
	*/
	Styles ParseStyles(const nlohmann::json& data);
	/**
	* Parses a timestamp from a string in the format 'HH:MM:SS,MS' or 'HH:MM:SS.MS'.
    * @param timestamp: String containing the timestamp.
    * @return Parsed timestamp in seconds.
	*/
	double ParseTimeStamp(const std::string& timestamp);
	/**
	* Validates that a JSON object contains a string value for a specific key.
    * @param data: JSON object to validate.
    * @param key: Key to check in the JSON object.
    * @return True if the key exists and its value is a string, false otherwise.
	*/
	bool ValidateDataString(const nlohmann::json& data, const std::string& key);
	/**
	* Validates that a JSON object contains an object value for a specific key.
    * @param data: JSON object to validate.
    * @param key: Key to check in the JSON object.
    * @return True if the key exists and its value is an object, false otherwise.
	*/
	bool ValidateDataObject(const nlohmann::json& data, const std::string& key);
};