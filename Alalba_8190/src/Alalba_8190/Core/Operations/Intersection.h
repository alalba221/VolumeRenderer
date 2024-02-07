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
	// float 
	template<>
	class IntersectionVolume<float> : public lux::Volume<float>
	{
	public:
		IntersectionVolume(std::shared_ptr< lux::Volume<float> > fieldPtr1, std::shared_ptr< lux::Volume<float> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~IntersectionVolume() {};

		virtual const float eval(const lux::Vector& P) const override
		{
			return std::min(m_fieldPtr1->eval(P), m_fieldPtr2->eval(P));
		};
		
	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<float> > m_fieldPtr2;
	};

	// Color 
	template<>
	class IntersectionVolume<lux::Color> : public lux::Volume<lux::Color>
	{
	public:
		IntersectionVolume(std::shared_ptr< lux::Volume<lux::Color> > fieldPtr1, std::shared_ptr< lux::Volume<lux::Color> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~IntersectionVolume() {};

		virtual const lux::Color eval(const lux::Vector& P) const override
		{
			return m_fieldPtr1->eval(P) + m_fieldPtr2->eval(P);
		};
		

	private:

		std::shared_ptr< lux::Volume<lux::Color> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<lux::Color> > m_fieldPtr2;
	};


	template<typename T>
	std::shared_ptr<IntersectionVolume<T>> Intersection(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		std::shared_ptr< lux::Volume<T> > fieldPtr2)
	{
		return std::make_shared< IntersectionVolume<T> >(fieldPtr1, fieldPtr2);
	}

}