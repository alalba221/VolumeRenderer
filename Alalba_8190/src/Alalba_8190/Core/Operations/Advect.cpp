#include "pch.h"
#include "Advect.h"

std::shared_ptr<lux::Volume<float>> Alalba::Addvect(const std::shared_ptr< lux::Volume<float> >& fieldPtr1,
	const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr2,
	float t)
{
    return std::make_shared<AdvectVolume>(fieldPtr1, fieldPtr2, t);
}
