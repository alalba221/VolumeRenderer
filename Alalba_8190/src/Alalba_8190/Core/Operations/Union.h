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
	

	template<typename T>
	std::shared_ptr<UnionVolume<T>> Union(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
	{
		return std::make_shared< UnionVolume<T> >(fieldPtr1, fieldPtr2);
	}
}

