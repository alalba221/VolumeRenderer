#pragma once
#include "Volume.h"
#include "Fields.h"
#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
namespace Alalba
{
	class WarpVolume:public lux::Volume<float>
	{
	public:
		WarpVolume(const std::shared_ptr< lux::Volume<float> >& scalar,
			const std::shared_ptr < lux::Volume < lux::Vector > >& vector) 
		:m_scalar(scalar), m_map(vector) {}
		
		virtual const volumeDataType eval(const lux::Vector& P) const override
		{
			lux::Vector X = m_map->eval(P);
			return m_scalar->eval(X);

		};

	private:
		std::shared_ptr< lux::Volume<float> > m_scalar;
		std::shared_ptr < lux::Volume < lux::Vector > > m_map;
	};

	std::shared_ptr<lux::Volume<float>> Warp(const std::shared_ptr< lux::Volume<float> >& scalar,
		const std::shared_ptr < lux::Volume < lux::Vector > >& vector);
}
