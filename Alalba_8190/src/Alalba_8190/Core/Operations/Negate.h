#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	template<typename T>
	class NegateVolume : public lux::Volume<T>
	{
	public:
		NegateVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
			:m_fieldPtr1(fieldPtr1) {};
		~NegateVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override { return -m_fieldPtr1->eval(P); };
		virtual const volumeGradType grad(const lux::Vector& P) const override { return -m_fieldPtr1->grad(P); };
	
	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		
	};

	/// Color
	/// Matrix
	

	template<typename T>
	std::shared_ptr<NegateVolume<T>> Negate(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
	{
		return std::make_shared< NegateVolume<T> >(fieldPtr1);
	}
}


