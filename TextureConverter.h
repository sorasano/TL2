#pragma once

#include <string>
#include <DirectXTex.h>

class TextureConverter
{
public:

	//テクスチャをwicからdds変換
	void ConvertTextureWICToDDS(const std::string& filePath);

private:

	//テクスチャファイル読み込み
	void LoadWICTextureFromFile(const std::string &filePath);

	//マルチバイト文字列をワイド文字列に変換
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	//フォルダパスとファイル名の分離
	void SeparateFilePath(const std::wstring& filePath);

	//ddsテクスチャとしてファイル書き出し
	void SaveDDSTextureToFile();

private:

	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage scratchImage_;

private:
	std::wstring directoryPath_;
	std::wstring fileName_;
	std::wstring fileExt_;
};

