#include "pch.h"
#include "importer.h"

#include <pxr/pxr.h>
#undef PXR_PYTHON_SUPPORT_ENABLED // 本当はpythonをのぞくべきだが、usdviewも使いたいのでビルドを通すために強引にundefする
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>

PXR_NAMESPACE_USING_DIRECTIVE

#include <iostream> // for test

namespace usd
{
	bool Importer::import(const std::string& filepath)
	{
        // Stage (シーン全体) をメモリ上にロード
        // Open は既存のファイルを読み込みます
		UsdStageRefPtr stage = UsdStage::Open(filepath);

        if (!stage)
        {
            std::cerr << "Error: Could not open stage [" << filepath << "]" << std::endl;
            return 1;
        }

        std::cout << "Successfully opened: " << filepath << std::endl;

        // シーン内のすべての Prim (要素) を走査
        // Traverse() はシーングラフの全ノードをイテレートします
        for (UsdPrim prim : stage->Traverse())
        {
            // Prim のパスと型（Mesh, Xform, etc.）を表示
            std::cout << "Prim Path: " << prim.GetPath()
                << " | Type: " << prim.GetTypeName() << std::endl;
        }

		return true;
	}
}