#include "pch.h"
#include "Warp.h"

std::shared_ptr<lux::Volume<float>> Alalba::Warp(const std::shared_ptr<lux::Volume<float>>& scalar, const std::shared_ptr<lux::Volume<lux::Vector>>& vector)
{
    return std::make_shared<WarpVolume>(scalar,vector);
}
