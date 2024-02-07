#include "pch.h"
#include "Shapes.h"
namespace Alalba
{
	const float Sphere::eval(const lux::Vector& P) const
	{
		return m_radius - (P - m_center).magnitude();
	}

	const float Ellipse::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);
		float Z = x * m_normal;
		lux::Vector x_vertical = x - m_normal * Z;

		return 1.0 - Z * Z / (m_radiusMajor * m_radiusMajor) - x_vertical.magnitude() * x_vertical.magnitude() / (m_radiusMinor * m_radiusMinor);
	}
	const float Box::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);

		return pow(m_radius, 8) - pow(x[0], 8) - pow(x[1], 8) - pow(x[2], 8);
	}
	const float Torus::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);
		lux::Vector x_vertical = x - m_normal * (x * m_normal);

		return 4 * pow(m_radiusMajor, 2) * pow(x_vertical.magnitude(), 2) - pow((x * x + m_radiusMajor * m_radiusMajor - m_radiusMinor * m_radiusMinor), 2);
	}
	const float Cone::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);

		float result;
		if (x == lux::Vector())
		{
			result = 0;
		}
		else if (x * m_normal > m_height)
		{
			result = m_height - x * m_normal;
		}
		else if (x * m_normal < 0.0)
		{
			result = x * m_normal;
		}
		else {
			result = x * m_normal - x.magnitude() * cos(m_rad);
		}
		return result;
	}
	const float Icosahedron::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);
		float T = 1.61803399;

		float result;

		if (x.magnitude() > 1.8 * M_PI)
		{
			result = -1.8 * M_PI;
		}
		else
		{
			result = cos(x[0] + T * x[1]) + cos(x[0] - T * x[1]) +
				cos(x[1] + T * x[2]) + cos(x[1] - T * x[2]) +
				cos(x[2] + T * x[0]) + cos(x[2] - T * x[0]) - 2.0;
		}

		return result;
	}
	const float Steiner::eval(const lux::Vector& P) const
	{
		lux::Vector x = (P - m_center);

		return -(x[0] * x[0] * x[1] * x[1] + x[0] * x[0] * x[2] * x[2] + x[1] * x[1] * x[2] * x[2] - x[0] * x[1] * x[2]);
	}
}
