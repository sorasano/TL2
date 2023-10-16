#pragma once

#include <string>

class TextureConverter
{
public:

	void ConvertTextureWICToDDS(const std::string& filePath);

private:

	void LoadWICTextureFromFile(const std::string &filePath);

	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
};

