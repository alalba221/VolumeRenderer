#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{

	template<typename T>
	class CutoutVolume : public lux::Volume<T>
	{
	public:
		CutoutVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, std::shared_ptr< lux::Volume<T> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~CutoutVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr2;
	};
	/// Scalar
	template<>
	class CutoutVolume<float> : public lux::Volume<float>
	{
	public:
		CutoutVolume(std::shared_ptr< lux::Volume<float> > fieldPtr1, std::shared_ptr< lux::Volume<float> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~CutoutVolume() {};

		virtual const float eval(const lux::Vector& P) const override
		{
			return std::min(m_fieldPtr1->eval(P), -m_fieldPtr2->eval(P));
		};

	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<float> > m_fieldPtr2;
	};

	/// Color 
	template<>
	class CutoutVolume<lux::Color> : public lux::Volume<lux::Color>
	{
	public:
		CutoutVolume(std::shared_ptr< lux::Volume<lux::Color> > fieldPtr1, std::shared_ptr< lux::Volume<lux::Color> > fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~CutoutVolume() {};

		virtual const lux::Color eval(const lux::Vector& P) const override
		{
			return m_fieldPtr1->eval(P) - m_fieldPtr2->eval(P);
		};

	private:

		std::shared_ptr< lux::Volume<lux::Color> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<lux::Color> > m_fieldPtr2;
	};


	template<typename T>
	std::shared_ptr<CutoutVolume<T>> Cutout(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		std::shared_ptr< lux::Volume<T> > fieldPtr2)
	{
		return std::make_shared< CutoutVolume<T> >(fieldPtr1, fieldPtr2);
	}

}
