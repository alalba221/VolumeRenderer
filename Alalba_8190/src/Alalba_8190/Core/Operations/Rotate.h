#pragma once
#include "Volume.h"
namespace Alalba
{

	template<typename T>
	class RotationVolume : public lux::Volume<T>
	{
	public:
		RotationVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr, const lux::Vector& axis, const double rad) 
			:m_fieldPtr(fieldPtr), m_axis(axis), m_rad(rad) {};
		~RotationVolume() {};

		virtual const T eval(const lux::Vector& P) const override 
		{ 

			lux::Matrix rotationMatrix = lux::rotation(m_axis, m_rad);
			return m_fieldPtr->eval(rotationMatrix.inverse() * P);
		};

	private:
		lux::Vector m_axis;
		double m_rad;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr;
	};

	template<typename T>
	std::shared_ptr< RotationVolume<T> > Rotate(const std::shared_ptr< lux::Volume<T> >& fieldPtr, const lux::Vector& axis, const double rad)
	{

		return std::make_shared< RotationVolume<T> >(fieldPtr,axis, rad);
	}


}