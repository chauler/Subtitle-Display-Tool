#pragma once
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>

/*
From https://stackoverflow.com/a/24386991
*/

/**
* Given a filepath, returns the file name with extension.
*/
template<class T>
T base_name(T const& path, T const& delims = "/\\")
{
	return path.substr(path.find_last_of(delims) + 1);
}

/**
* Given a filename (note: not a path), returns the base name without the file extension.
*/
template<class T>
T remove_extension(T const& filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

/**
* Given a filename (note: not a path), returns the file extension.
*/
template<class T>
T file_extension(T const& filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos && filename.size() >= p+1 ? filename.substr(p + 1, filename.size()) : filename;
}

inline std::string string_base_name(std::string const& path)
{
	return base_name(path);
}

/**
* From https://stackoverflow.com/a/217605
*/

// trim from start (in place)
inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string& s) {
	rtrim(s);
	ltrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
inline std::string trim_copy(std::string s) {
	trim(s);
	return s;
}