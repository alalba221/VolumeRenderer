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
		ScaleVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, const T& scale )
			:m_fieldPtr1(fieldPtr1), m_scale(scale) {};
		~ScaleVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		T m_scale;
	};

	// Scalar
	template<>
	class ScaleVolume<float> : public lux::Volume<float>
	{
	public:
		ScaleVolume(std::shared_ptr< lux::Volume<float> > fieldPtr1, const float& scale)
			:m_fieldPtr1(fieldPtr1), m_scale(scale) {};
		~ScaleVolume() {};

		virtual const float eval(const lux::Vector& P) const override
		{
			return m_fieldPtr1->eval(P/m_scale);
		};
	private:

		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		float m_scale;
	};


	template<typename T>
	std::shared_ptr<ScaleVolume<T>> Scale(std::shared_ptr< lux::Volume<T> > fieldPtr1,
		const T& scale)
	{
		return std::make_shared< ScaleVolume<T> >(fieldPtr1, scale);
	}
}
