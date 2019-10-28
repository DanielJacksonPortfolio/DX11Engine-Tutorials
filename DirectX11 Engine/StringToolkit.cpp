#include "StringToolkit.h"
#include <algorithm>

std::wstring StringToolkit::StringToWide(std::string str)
{
	std::wstring wideString(str.begin(), str.end());
	return wideString;
}

std::string StringToolkit::GetDirectoryFromPath(const std::string& filepath)
{
	size_t offset1 = filepath.find_last_of('\\');
	size_t offset2 = filepath.find_last_of('/');
	if (offset1 == std::string::npos && offset2 == std::string::npos)
		return "";
	if (offset1 == std::string::npos)
		return filepath.substr(0, offset2);
	if (offset2 == std::string::npos)
		return filepath.substr(0, offset1);

	return filepath.substr(0, std::max(offset1,offset2));
}

std::string StringToolkit::GetFileExtension(const std::string& filename)
{
	size_t offset = filename.find_last_of('.');
	if (offset == std::string::npos)
		return{ };
	
	return std::string(filename.substr(offset + 1));
}
