#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	template<typename T>
	class AbsVolume : public lux::Volume<T>
	{
	public:
		AbsVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
			:m_fieldPtr1(fieldPtr1) {};
		~AbsVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override { return std::abs(m_fieldPtr1->eval(P)); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;

	};

	/// Color
	/// Matrix


	template<typename T>
	std::shared_ptr<lux::Volume<T>> Abs(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
	{
		return std::make_shared< AbsVolume<T> >(fieldPtr1);
	}
}
