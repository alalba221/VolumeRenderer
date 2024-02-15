#pragma once
#include "Union.h"
namespace Alalba
{
	/// Scalar
	template<>
	virtual const float UnionVolume<float>::eval(const lux::Vector& P) const override
	{
		return std::max(m_fieldPtr1->eval(P), m_fieldPtr2->eval(P));
	};

	///  Color
	template<>
	virtual const lux::Color UnionVolume<lux::Color>::eval(const lux::Vector& P) const override
	{
		lux::Color Black(0.0, 0.0, 0.0,1.0);
		
		lux::Color color1 = m_fieldPtr1->eval(P);
		lux::Color color2 = m_fieldPtr2->eval(P);
		
		if (color1 != Black && color2 != Black)
		{
			return color1 *0.5 + color2 *0.5;
		}

		return color1 + color2;
	};
}