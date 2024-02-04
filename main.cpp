#include <cstdio>
#include <cstdlib>
#include <DirectXTex.h>

#include "TextureConverter.h"

enum Argument {
	kApplicationPath,
	kFilePath,
	NumArgument
};

int main(int argc,char* argv[]) {

	assert(argc >= NumArgument);

	if (argc < NumArgument) {
		TextureConverter::OutputUsage();
		return 0;
	}

	HRESULT hr = CoInitializeEx(nullptr,COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	int numOptions = argc - NumArgument;
	char** options = argv + NumArgument;

	TextureConverter converter;
	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions,options);

	CoUninitialize();

	system("pause");
	return 0;
}