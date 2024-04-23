#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	/// gradient vector
	//template<typename T>
	class GradientVectorVolume : public lux::Volume<lux::Vector>
	{
	public:
		GradientVectorVolume(const std::shared_ptr< lux::Volume<float> >& fieldPtr1)
			:m_fieldPtr1(fieldPtr1) {};
		~GradientVectorVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override { return m_fieldPtr1->grad(P); };
		//virtual const volumeGradType grad(const lux::Vector& P) const override { return m_fieldPtr1->grad(P) + m_fieldPtr2->grad(P); };
	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;

	};

	std::shared_ptr<lux::Volume<lux::Vector>> GradientVector(const std::shared_ptr< lux::Volume<float> >& fieldPtr1);

}
