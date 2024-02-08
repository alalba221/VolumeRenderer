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
		CutoutVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~CutoutVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr2;
	};

	/// Scalar
	template<>
	virtual const float CutoutVolume<float>::eval(const lux::Vector& P) const override
	{
		return std::min(m_fieldPtr1->eval(P), -m_fieldPtr2->eval(P));
	};
	

	/// Color 
	template<>
	virtual const lux::Color CutoutVolume<lux::Color>::eval(const lux::Vector& P) const override
	{
		return m_fieldPtr1->eval(P) - m_fieldPtr2->eval(P);
	};
	

	template<typename T>
	std::shared_ptr<CutoutVolume<T>> Cutout(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr2)
	{
		return std::make_shared< CutoutVolume<T> >(fieldPtr1, fieldPtr2);
	}

}
