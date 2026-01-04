#include <openusd_importer/openusd_importer.h>
#include <iostream>
#include <string>

int main()
{
	const std::string fileDir = "assets\\";
	const std::string fileName = "HelloWorld.usda";
	const std::string filePath = fileDir + fileName;

	bool result = usd::Importer::import(filePath);
	
	return 0;
}