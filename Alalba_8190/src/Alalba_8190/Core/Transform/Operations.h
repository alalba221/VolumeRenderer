#pragma once
#include "Volume.h"
namespace Alalba
{
	/// <summary>
	///  scalar field
	/// </summary>
	class SFUnion : public lux::Volume<float>
	{
	public:
		SFUnion(const std::shared_ptr<lux::Volume<float>>& f1, 
			const std::shared_ptr<lux::Volume<float>>& f2)
			:m_f1(f1),m_f2(f2)
		{}
		~SFUnion() {};

		virtual const float eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<float>> m_f1;
		std::shared_ptr<lux::Volume<float>> m_f2;
	};

	class SFIntersection : public lux::Volume<float>
	{
	public:
		SFIntersection(const std::shared_ptr<lux::Volume<float>>& f1,
			const std::shared_ptr<lux::Volume<float>>& f2)
			:m_f1(f1), m_f2(f2)
		{}
		~SFIntersection() {};

		virtual const float eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<float>> m_f1;
		std::shared_ptr<lux::Volume<float>> m_f2;
	};

	class SFCutOut : public lux::Volume<float>
	{
	public:
		SFCutOut(const std::shared_ptr<lux::Volume<float>>& f1,
			const std::shared_ptr<lux::Volume<float>>& f2)
			:m_f1(f1), m_f2(f2)
		{}
		~SFCutOut() {};

		virtual const float eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<float>> m_f1;
		std::shared_ptr<lux::Volume<float>> m_f2;
	};

	/// <summary>
	/// color field
	/// </summary>
	class CFUnion : public lux::Volume<lux::Color>
	{
	public:
		CFUnion(const std::shared_ptr<lux::Volume<lux::Color>>& f1, 
			const std::shared_ptr<lux::Volume<lux::Color>>& f2)
			:m_f1(f1), m_f2(f2)
		{}
		~CFUnion() {};

		virtual const lux::Color eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<lux::Color>> m_f1;
		std::shared_ptr<lux::Volume<lux::Color>> m_f2;
	};

	class CFIntersection : public lux::Volume<lux::Color>
	{
	public:
		CFIntersection(const std::shared_ptr<lux::Volume<lux::Color>>& f1,
			const std::shared_ptr<lux::Volume<lux::Color>>& f2)
			:m_f1(f1), m_f2(f2)
		{}
		~CFIntersection() {};

		virtual const lux::Color eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<lux::Color>> m_f1;
		std::shared_ptr<lux::Volume<lux::Color>> m_f2;
	};

	class CFCutOut : public lux::Volume<lux::Color>
	{
	public:
		CFCutOut(const std::shared_ptr<lux::Volume<lux::Color>>& f1,
			const std::shared_ptr<lux::Volume<lux::Color>>& f2)
			:m_f1(f1), m_f2(f2)
		{}
		~CFCutOut() {};

		virtual const lux::Color eval(const lux::Vector& p)const  override;
	private:
		std::shared_ptr<lux::Volume<lux::Color>> m_f1;
		std::shared_ptr<lux::Volume<lux::Color>> m_f2;
	};

}

