#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	/// UNION
	template<typename T>
	class UnionVolume : public lux::Volume<T>
	{
	public:
		UnionVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, std::shared_ptr< lux::Volume<T> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~UnionVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr2;
	};
	
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

	template<typename T>
	std::shared_ptr<UnionVolume<T>> Union(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
	{
		return std::make_shared< UnionVolume<T> >(fieldPtr1, fieldPtr2);
	}
}

