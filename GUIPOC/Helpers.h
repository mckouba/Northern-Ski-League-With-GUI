#ifndef GUARD_H
#define GUARD_H



#include <string>

std::string getCurrentLocation();
std::string make_string(const std::wstring& wstirng);
Platform::String^ convertFromString(const std::string& input);

//string array to store values selected in the menu
//will store as:
//[{gs,sl,sg},{mens,womens},{season},{meet}]
std::string selectedValues[];
#endif // !GUARD_H
