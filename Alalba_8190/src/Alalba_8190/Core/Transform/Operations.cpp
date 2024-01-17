#include "pch.h"
#include "Operations.h"
namespace Alalba
{
	// TODO: need more flexible color field boolean operation
	const float SFUnion::eval(const lux::Vector& p) const
	{
		return max(m_f1->eval(p), m_f2->eval(p));
	}
	const lux::Color CFUnion::eval(const lux::Vector& p) const
	{
		return m_f1->eval(p)+m_f2->eval(p);
	}
	const float SFIntersection::eval(const lux::Vector& p) const
	{
		return min(m_f1->eval(p), m_f2->eval(p));
	}
	const lux::Color CFIntersection::eval(const lux::Vector& p) const
	{
		return m_f1->eval(p) + m_f2->eval(p);
	}
	const float SFCutOut::eval(const lux::Vector& p) const
	{
		return min(m_f1->eval(p), -m_f2->eval(p));
	}
	const lux::Color CFCutOut::eval(const lux::Vector& p) const
	{
		//return m_f1->eval(p);
		return m_f1->eval(p)- m_f2->eval(p);

	}
}
