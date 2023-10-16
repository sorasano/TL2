#include "TextureConverter.h"
#include <Windows.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	LoadWICTextureFromFile(filePath);

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{

	std::wstring wfilepath = ConvertMultiByteStringToWideString(filePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	int filePathBufferSize = MultiByteToWideChar(CP_ACP,0,mString.c_str(),-1,nullptr,0);

	std::wstring wString;
	wString.resize(filePathBufferSize);

	MultiByteToWideChar(CP_ACP,0,mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}
