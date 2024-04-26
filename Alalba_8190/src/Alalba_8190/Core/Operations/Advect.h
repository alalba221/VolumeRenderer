#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	class AdvectVolume : public lux::Volume<float>
	{
	public:
		AdvectVolume(const std::shared_ptr< lux::Volume<float> >& fieldPtr1,
			const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr2,
			float t)
			:m_fieldPtr1(fieldPtr1), m_velocity(fieldPtr2),m_t(t) {};
		~AdvectVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override 
		{ 
			lux::Vector X = P - (m_velocity->eval(P)) * m_t;
			return m_fieldPtr1->eval(X);
		};
		
	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<lux::Vector> >m_velocity;
		float m_t;
	};

	std::shared_ptr<lux::Volume<float>> Addvect(const std::shared_ptr< lux::Volume<float> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr2,
		float t);
}
