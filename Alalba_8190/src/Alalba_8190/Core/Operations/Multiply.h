#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>
namespace Alalba
{
	template<typename T1, typename T2 >
	class MultiVolume : public lux::Volume<T1>
	{
	public:
		MultiVolume(const std::shared_ptr< lux::Volume<T1> >& fieldPtr1, const std::shared_ptr< lux::Volume<T2> >& fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~MultiVolume() {};

		virtual const T1 eval(const lux::Vector& P) const override { return T1(); };

	private:

		std::shared_ptr< lux::Volume<T1> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<T2> > m_fieldPtr2;
	};

	/// T1 * Scalar: return T1
	template<typename T1>
	class MultiVolume<T1, float> : public lux::Volume<T1>
	{
	public:
		MultiVolume(const std::shared_ptr< lux::Volume<T1> >& fieldPtr1, const std::shared_ptr< lux::Volume<float> >& fieldPtr2)
			:m_fieldPtr1(fieldPtr1), m_fieldPtr2(fieldPtr2) {};
		~MultiVolume() {};

		virtual const T1 eval(const lux::Vector& P) const override { return m_fieldPtr1->eval(P) *m_fieldPtr2->eval(P); };

	private:

		std::shared_ptr< lux::Volume<T1> > m_fieldPtr1;
		std::shared_ptr< lux::Volume<float> > m_fieldPtr2;
	};

	template<typename T1,typename T2>
	std::shared_ptr<MultiVolume<T1,T2>> Multiply(const std::shared_ptr< lux::Volume<T1> >& fieldPtr1,
		const std::shared_ptr< lux::Volume<T2> >& fieldPtr2)
	{
		return std::make_shared< MultiVolume<T1,T2> >(fieldPtr1, fieldPtr2);
	}
}
