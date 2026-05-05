#include "usdImporter.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>

#include "math/matrix.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace
{
    std::filesystem::path GetExecutableDirectory()
    {
        return std::filesystem::current_path();
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

        _putenv_s("PXR_PLUGINPATH_NAME", pluginPath.string().c_str());
    }
}

namespace usd
{
	void Importer::configurePluginPath()
	{
		ConfigureDefaultPluginPath();
	}

	bool Importer::load(const char* filePath)
	{
		pxr::UsdStageRefPtr stage = pxr::UsdStage::Open(filePath);
		if (!stage) {
			std::cerr << "Failed to open USD file: " << filePath << std::endl;
			return false;
		}
		std::cout << "Successfully loaded USD file: " << filePath << std::endl;

        for (const pxr::UsdPrim& prim : stage->Traverse())
        {
            if (prim.IsA<pxr::UsdGeomMesh>())
            {
                addMeshPart(pxr::UsdGeomMesh(prim));
			}
        }

		return true;
	}

    bool Importer::addMeshPart(const pxr::UsdGeomMesh& usdMesh)
    {
        // triangle meshのみ
        pxr::VtIntArray faceVertexCounts;
        usdMesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);
        for (const int count : faceVertexCounts)
        {
            if (count != 3) {
				std::cout << "Non-triangle mesh found in USD file: " << usdMesh.GetPrim().GetPath().GetString() << std::endl;
                return false;
            }
        }

        pxr::UsdGeomXformCache xformCache;
		const pxr::GfMatrix4d worldMtx = xformCache.GetLocalToWorldTransform(usdMesh.GetPrim());

        MeshPart meshPart;
        meshPart.mPath = usdMesh.GetPrim().GetPath().GetString();

        pxr::VtIntArray faceVertexIndices;
        usdMesh.GetFaceVertexIndicesAttr().Get(&faceVertexIndices);
        meshPart.mIndices.assign(faceVertexIndices.begin(), faceVertexIndices.end());
        
        pxr::VtVec3fArray points;
        usdMesh.GetPointsAttr().Get(&points);
        meshPart.mVertices.reserve(points.size());
        for (const auto& point : points)
        {
			pxr::GfVec3d pd(point);
            pxr::GfVec3f p(worldMtx.Transform(pd));
            meshPart.mVertices.emplace_back(p[0], p[1], p[2]);
        }
        mMeshParts.push_back(std::move(meshPart));

        return true;
	}
}