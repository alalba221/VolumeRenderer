#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>

namespace Alalba
{
	template<typename T>
	class EXPVolume : public lux::Volume<T>
	{
	public:
		EXPVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
			:m_fieldPtr1(fieldPtr1) {};
		~EXPVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return std::exp(m_fieldPtr1->eval(P)); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;

	};


	template<typename T>
	std::shared_ptr<lux::Volume<T>> EXP(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
	{
		return std::make_shared< EXPVolume<T> >(fieldPtr1);
	}
}
