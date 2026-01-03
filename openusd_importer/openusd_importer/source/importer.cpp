#include "pch.h"
#include "importer.h"

#include <pxr/pxr.h>
#undef PXR_PYTHON_SUPPORT_ENABLED // 本当はpythonをのぞくべきだが、usdviewも使いたいのでビルドを通すために強引にundefする
#include <pxr/usd/usd/stage.h>

PXR_NAMESPACE_USING_DIRECTIVE

#include <iostream> // for test

namespace usd
{
	bool Importer::import(const std::string& filepath)
	{
		if (UsdStageRefPtr stage = UsdStage::Open(filepath))
		{
			return true;
		}

		return false;
	}
}