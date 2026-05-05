#pragma once

#include <vector>
#include <string>
#include <usd.h>

#include "math/vec3.h"

namespace usd
{
	class MeshPart
	{
	public:
		std::string mPath;
		std::vector<uint32_t> mIndices;
		std::vector<vec3> mVertices;
	};

	class Importer
	{
	public:
		static void configurePluginPath();

		bool load(const char* filePath);

	private:
		bool addMeshPart(const pxr::UsdGeomMesh& usdMesh);

		std::vector<MeshPart> mMeshParts;
	};
}