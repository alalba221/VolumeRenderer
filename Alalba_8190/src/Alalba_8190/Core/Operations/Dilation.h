#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	template<typename T>
	class DilationVolume : public lux::Volume<T>
	{
	public:
		DilationVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const T& d)
			:m_fieldPtr1(fieldPtr1), m_d(d) {};
		~DilationVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return m_fieldPtr1->eval(P) + m_d; };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		T m_d;
	};
	

	template<typename T>
	std::shared_ptr<DilationVolume<T>> Dilation(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const T& d)
	{
		return std::make_shared< DilationVolume<T> >(fieldPtr1, d);
	}

}
