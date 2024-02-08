#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	template<typename T>
	class ScaleVolume : public lux::Volume<T>
	{
	public:
		ScaleVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, const lux::Vector& scale )
			:m_fieldPtr1(fieldPtr1), m_scale(scale) {};
		~ScaleVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		lux::Vector m_scale;
	};

	// Scalar
	template<>
	virtual const float ScaleVolume<float>::eval(const lux::Vector& P) const 
	{
		lux::Vector scaled = lux::Vector(P.X() / m_scale.X(), P.Y() / m_scale.Y(), P.Z() / m_scale.Z());
		return m_fieldPtr1->eval(scaled);
	};
	

	template<typename T>
	std::shared_ptr<ScaleVolume<T>> Scale(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		const lux::Vector& scale)
	{
		return std::make_shared< ScaleVolume<T> >(fieldPtr1, scale);
	}
}
