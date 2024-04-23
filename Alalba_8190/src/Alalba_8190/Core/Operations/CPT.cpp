#include "pch.h"
#include "CPT.h"
namespace Alalba
{
	std::shared_ptr<lux::Volume<lux::Vector>> CPT(const std::shared_ptr<lux::Volume<float>>& sdf)
	{
		return std::make_shared<CPTVolume>(sdf);
	}
}