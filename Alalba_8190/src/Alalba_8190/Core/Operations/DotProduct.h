
#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	class DotProductVolume : public lux::Volume<float>
	{
	public:
		DotProductVolume(const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr1, const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~DotProductVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override { return m_fieldPtr1->eval(P) * m_fieldPtr2->eval(P); };
		//virtual const volumeGradType grad(const lux::Vector& P) const override { return m_fieldPtr1->grad(P) * m_fieldPtr2->eval(P); };

	private:

		std::shared_ptr< lux::Volume<lux::Vector> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<lux::Vector> > m_fieldPtr2;
	};

	

	std::shared_ptr<DotProductVolume> DotProduct(const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<lux::Vector> >& fieldPtr2)
	{
		return std::make_shared< DotProductVolume>(fieldPtr1, fieldPtr2);
	}
}
