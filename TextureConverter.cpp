#include "TextureConverter.h"
#include <Windows.h>
using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[] = nullptr)
{
	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile(numOptions, options);
}

void TextureConverter::OutputUsage()
{
	printf("画像ファイルをWIC形式からDDS形式に変換します\n");
	printf("\n");
	printf("TextureConverter [ドライブ:][パス][ファイル名]\n");
	printf("\n");
	printf("[ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します\n");
	printf("\n");
	printf("[-ml level]: ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します\n");

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{

	HRESULT result;

	std::wstring wfilepath = ConvertMultiByteStringToWideString(filePath);

	result = LoadFromWICFile(wfilepath.c_str(),WIC_FLAGS_NONE,&metadata_,scratchImage_);
	assert(SUCCEEDED(result));

	SeparateFilePath(wfilepath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	int filePathBufferSize = MultiByteToWideChar(CP_ACP,0,mString.c_str(),-1,nullptr,0);

	std::wstring wString;
	wString.resize(filePathBufferSize);

	MultiByteToWideChar(CP_ACP,0,mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind('.');
	if (pos1 != std::wstring::npos) {
		fileExt_ = filePath.substr(pos1 + 1,filePath.size() - pos1 - 1);
		exceptExt = filePath.substr(0,pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0,pos1+1);
		fileName_ = exceptExt.substr(pos1 + 1,exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[] = nullptr)
{

	size_t mipLevel = 0;

	for (int i = 0; i < numOptions; i++) {
		if (std::string(options[i]) == "-mt") {
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	HRESULT result;
	ScratchImage mipChain;

	//ミップマップ生成
	result = GenerateMipMaps(scratchImage_.GetImages(),scratchImage_.GetImageCount(),scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT,mipLevel,mipChain);
	if (SUCCEEDED(result)) {
		//イメージとメタデータをミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	//圧縮形式に変換
	ScratchImage converted;
	result = Compress(scratchImage_.GetImages(),scratchImage_.GetImageCount(),metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB,TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | 
		TEX_COMPRESS_PARALLEL,1.0f,converted);
	if (SUCCEEDED(result)) {
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}


	//読み込んだデータをSRGBとして使う
	metadata_.format = MakeSRGB(metadata_.format);

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	result = SaveToDDSFile(scratchImage_.GetImages(),scratchImage_.GetImageCount(),metadata_,DDS_FLAGS_NONE,filePath.c_str());
	assert(SUCCEEDED(result));
}
