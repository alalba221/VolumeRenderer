#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	template<typename T>
	class DilationVolume : public lux::Volume<T>
	{
	public:
		DilationVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, const T& d)
			:m_fieldPtr1(fieldPtr1), m_d(d) {};
		~DilationVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		T m_d;
	};
	// float 
	template<>
	class DilationVolume<float> : public lux::Volume<float>
	{
	public:
		DilationVolume(std::shared_ptr< lux::Volume<float> > fieldPtr1, const float& d)
			:m_fieldPtr1(fieldPtr1), m_d(d) {};
		~DilationVolume() {};

		virtual const float eval(const lux::Vector& P) const override
		{
			return m_fieldPtr1->eval(P) + m_d;
		};

	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		float m_d;
	};

	// Color 
	template<>
	class DilationVolume<lux::Color> : public lux::Volume<lux::Color>
	{
	public:
		DilationVolume(std::shared_ptr< lux::Volume<lux::Color> > fieldPtr1, const lux::Color& d)
			:m_fieldPtr1(fieldPtr1), m_d(d) {};
		~DilationVolume() {};

		virtual const lux::Color eval(const lux::Vector& P) const override
		{
			return m_fieldPtr1->eval(P)+m_d ;
		};

	private:

		std::shared_ptr< lux::Volume<lux::Color> > m_fieldPtr1;
		lux::Color m_d;
	};


	template<typename T>
	std::shared_ptr<DilationVolume<T>> Dilation(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		const T& d)
	{
		return std::make_shared< DilationVolume<T> >(fieldPtr1, d);
	}

}
