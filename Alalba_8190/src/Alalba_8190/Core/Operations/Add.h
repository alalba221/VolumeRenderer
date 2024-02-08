#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	template<typename T>
	class AddVolume : public lux::Volume<T>
	{
	public:
		AddVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~AddVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return m_fieldPtr1->eval(P)+ m_fieldPtr2->eval(P); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr2;
	};

	
	template<typename T>
	std::shared_ptr<AddVolume<T>> Add(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
	{
		return std::make_shared< AddVolume<T> >(fieldPtr1, fieldPtr2);
	}
}

