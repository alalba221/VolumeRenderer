#include "pch.h"
#include "Transform.h"
#include "LinearAlgebra.h"
namespace Alalba
{
  const float SFTranslate::eval(const lux::Vector& p)const
  {
    return m_f->eval(p-m_dX);
  }

  const lux::Color CFTranslate::eval(const lux::Vector& p) const
  {
      return m_f->eval(p - m_dX);
  }

  const float SFRotation::eval(const lux::Vector& p) const
  {
    lux::Matrix rotationMatrix = lux::rotation(m_axis, m_rad);

    return  m_f->eval(rotationMatrix.inverse() * p);
  }
  const lux::Color CFRotation::eval(const lux::Vector& p) const
  {
    lux::Matrix rotationMatrix = lux::rotation(m_axis, m_rad);

    return  m_f->eval(rotationMatrix.inverse() * p);
  }
  const float SFScale::eval(const lux::Vector& p) const
  {
    return m_f->eval(lux::Vector(p[0]/m_scale[0],p[1]/m_scale[1],p[2]/m_scale[2]));
  }
  const lux::Color CFScale::eval(const lux::Vector& p) const
  {
    return m_f->eval(lux::Vector(p[0] / m_scale[0], p[1] / m_scale[1], p[2] / m_scale[2]));
  }
}