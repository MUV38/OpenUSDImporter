#include <iostream>
#include <filesystem>
#include <string>

#include "usdImporter.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <usd-file-path>\n";
        return 1;
    }

	usd::Importer::configurePluginPath();

    const char* filePath = argv[1];

	usd::Importer importer;
	const bool result = importer.load(filePath);

    if (!result)
    {
        std::cerr << "Failed to open USD stage: " << filePath << '\n';
        return 1;
    }

    std::cout << "Opened USD stage: " << filePath << '\n';

    return 0;
}
