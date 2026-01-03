#pragma once

#include <string>

namespace usd
{
	class Importer
	{
	public:
		static bool import(const std::string& filepath);
	};
}