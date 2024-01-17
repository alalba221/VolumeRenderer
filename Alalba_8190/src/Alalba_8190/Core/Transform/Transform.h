#pragma once
#include "Volume.h"
namespace Alalba
{
	/// scalar field
	class SFTranslate : public lux::Volume<float>
	{
	public:
		SFTranslate(const std::shared_ptr<lux::Volume<float>>& f, const lux::Vector dX)
			:m_f(f),m_dX(dX){};

		~SFTranslate() {};

		virtual const float eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<float>> m_f;
		lux::Vector m_dX;
	};

	class SFRotation : public lux::Volume<float>
	{
	public:
		SFRotation(const std::shared_ptr<lux::Volume<float>>& f, const lux::Vector axis, const float rad)
			:m_f(f), m_axis(axis), m_rad(rad){};

		~SFRotation() {};

		virtual const float eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<float>> m_f;
		lux::Vector m_axis;
		float m_rad;
	};

	class SFScale : public lux::Volume<float>
	{
	public:
		SFScale(const std::shared_ptr<lux::Volume<float>>& f, const lux::Vector scale)
			:m_f(f), m_scale(scale) {};

		~SFScale() {};

		virtual const float eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<float>> m_f;
		lux::Vector m_scale;
	};


	/// color field
	class CFTranslate : public lux::Volume<lux::Color>
	{
	public:
		CFTranslate(const std::shared_ptr<lux::Volume<lux::Color>>& f, const lux::Vector dX)
			:m_f(f), m_dX(dX) {};

		~CFTranslate() {};

		virtual const lux::Color eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<lux::Color>> m_f;
		lux::Vector m_dX;
	};

	class CFRotation : public lux::Volume<lux::Color>
	{
	public:
		CFRotation(const std::shared_ptr<lux::Volume<lux::Color>>& f, const lux::Vector axis, const float rad)
			:m_f(f), m_axis(axis), m_rad(rad) {};

		~CFRotation() {};

		virtual const lux::Color eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<lux::Color>> m_f;
		lux::Vector m_axis;
		float m_rad;
	};

	class CFScale : public lux::Volume<lux::Color>
	{
	public:
		CFScale(const std::shared_ptr<lux::Volume<lux::Color>>& f, const lux::Vector scale)
			:m_f(f), m_scale(scale) {};

		~CFScale() {};

		virtual const lux::Color eval(const lux::Vector& p)const;
	private:

		std::shared_ptr<lux::Volume<lux::Color>> m_f;
		lux::Vector m_scale;
	};

}


