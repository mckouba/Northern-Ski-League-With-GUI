#include "pch.h"
#include "Helpers.h"
#include <fstream>

extern std::string selectedValues[4] = { "gs", "mens", "2019-20", "1" };


std::string make_string(const std::wstring& wstring) {
	auto wideData = wstring.c_str();
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideData, -1, nullptr, 0, NULL, NULL);
	auto utf8 = std::make_unique<char[]>(bufferSize);
	if (0 == WideCharToMultiByte(CP_UTF8, 0, wideData, -1, utf8.get(), bufferSize, NULL, NULL))
		throw std::exception("Can't convert string to utf8");

	return std::string(utf8.get());
}

std::string getCurrentLocation() {
	Windows::Storage::StorageFolder^ f = Windows::Storage::ApplicationData::Current->LocalFolder;

	Platform::String^ s = f->Path->ToString();
	std::wstring temp(s->Data());
	auto location = make_string(temp);
	return location;
}

Platform::String^ convertFromString(const std::string& input) {
	std::wstring w_str = std::wstring(input.begin(), input.end());
	const wchar_t* w_chars = w_str.c_str();

	return (ref new Platform::String(w_chars));
}