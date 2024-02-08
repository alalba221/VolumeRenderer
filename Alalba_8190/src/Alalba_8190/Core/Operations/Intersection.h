#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	template<typename T>
	class IntersectionVolume : public lux::Volume<T>
	{
	public:
		IntersectionVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, std::shared_ptr< lux::Volume<T> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~IntersectionVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };
	
	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr2;
	};
	// Scalar 
	template<>
	virtual const float IntersectionVolume<float>::eval(const lux::Vector& P) const 
	{
		return std::min(m_fieldPtr1->eval(P), m_fieldPtr2->eval(P));
	};
	

	// Color 
	template<>
	virtual const lux::Color IntersectionVolume<lux::Color>::eval(const lux::Vector& P) const 
	{
		return m_fieldPtr1->eval(P) + m_fieldPtr2->eval(P);
	};
	

	template<typename T>
	std::shared_ptr<IntersectionVolume<T>> Intersection(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		std::shared_ptr< lux::Volume<T> > fieldPtr2)
	{
		return std::make_shared< IntersectionVolume<T> >(fieldPtr1, fieldPtr2);
	}

}