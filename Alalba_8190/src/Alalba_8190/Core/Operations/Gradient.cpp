#include "pch.h"
#include "Gradient.h"

namespace Alalba
{
	std::shared_ptr<lux::Volume<lux::Vector>> GradientVector(const std::shared_ptr< lux::Volume<float> >& fieldPtr1)
	{
		return std::make_shared< GradientVectorVolume >(fieldPtr1);
	}
}