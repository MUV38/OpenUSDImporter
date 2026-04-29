#include <iostream>
#include <filesystem>
#include <string>

#include <pxr/base/tf/token.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/stage.h>

#if defined(_WIN32)
#include <Windows.h>
#include <cstdlib>
#else
#include <unistd.h>
#endif

PXR_NAMESPACE_USING_DIRECTIVE

namespace
{
std::filesystem::path GetExecutableDirectory()
{
#if defined(_WIN32)
    char path[MAX_PATH] = {};
    const DWORD size = GetModuleFileNameA(nullptr, path, MAX_PATH);
    if (size == 0 || size == MAX_PATH) {
        return std::filesystem::current_path();
    }
    return std::filesystem::path(path).parent_path();
#else
    return std::filesystem::current_path();
#endif
}

void ConfigureDefaultPluginPath()
{
    if (std::getenv("PXR_PLUGINPATH_NAME") != nullptr) {
        return;
    }

    const std::filesystem::path pluginPath = GetExecutableDirectory() / "usd";
    if (!std::filesystem::exists(pluginPath / "plugInfo.json")) {
        return;
    }

#if defined(_WIN32)
    _putenv_s("PXR_PLUGINPATH_NAME", pluginPath.string().c_str());
#else
    setenv("PXR_PLUGINPATH_NAME", pluginPath.string().c_str(), 0);
#endif
}

void PrintPrimTree(const UsdStageRefPtr& stage)
{
    for (const UsdPrim& prim : stage->Traverse()) {
        const std::string path = prim.GetPath().GetString();
        const TfToken typeName = prim.GetTypeName();

        std::cout << path;
        if (!typeName.IsEmpty()) {
            std::cout << " [" << typeName.GetString() << "]";
        }
        std::cout << '\n';
    }
}

std::filesystem::path ResolveAssetPath(const std::string& inputPath)
{
    const std::filesystem::path requestedPath(inputPath);
    if (std::filesystem::exists(requestedPath)) {
        return requestedPath;
    }

    const std::filesystem::path sourceRelativePath =
        std::filesystem::path(OPENUSD_IMPORTER_SOURCE_DIR) / requestedPath;
    if (std::filesystem::exists(sourceRelativePath)) {
        return sourceRelativePath;
    }

    const std::filesystem::path exeRelativePath = GetExecutableDirectory() / requestedPath;
    if (std::filesystem::exists(exeRelativePath)) {
        return exeRelativePath;
    }

    return requestedPath;
}
}

int main(int argc, char** argv)
{
    ConfigureDefaultPluginPath();

    const char* defaultAsset = "assets/HelloWorld.usda";
    const std::string requestedAssetPath = argc > 1 ? argv[1] : defaultAsset;
    const std::filesystem::path assetPath = ResolveAssetPath(requestedAssetPath);

    UsdStageRefPtr stage = UsdStage::Open(assetPath.string());
    if (!stage) {
        std::cerr << "Failed to open USD stage: " << requestedAssetPath << '\n';
        return 1;
    }

    std::cout << "Opened USD stage: " << assetPath.string() << '\n';
    PrintPrimTree(stage);
    return 0;
}
