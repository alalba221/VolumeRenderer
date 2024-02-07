#pragma once
#include "Volume.h"
namespace Alalba
{
	
	template<typename T>
	class TranslateVolume : public lux::Volume<T>
	{
	public:
		TranslateVolume( std::shared_ptr< lux::Volume<T> > fieldPtr,const lux::Vector& dx) :m_fieldPtr(fieldPtr), m_dX(dx){};
		~TranslateVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return m_fieldPtr->eval(P-m_dX); };
	
	private:
		lux::Vector m_dX;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr;
	};

	template<typename T>
	std::shared_ptr< TranslateVolume<T> > Translate(std::shared_ptr< lux::Volume<T> > fieldPtr, const lux::Vector& dX)
	{
	
		return std::make_shared< TranslateVolume<T> >(fieldPtr, dX);
	}

	
}


