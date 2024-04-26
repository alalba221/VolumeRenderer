#include "pch.h"
#include "VelocityNoise.h"
namespace Alalba
{
	VelocityNoiseVolume::VelocityNoiseVolume(const Noise_t& param, const lux::Vector& dx, const lux::Vector& dy, const lux::Vector& dz, float ampl)
		:m_para(param), m_dx(dx), m_dy(dy),m_dz(dz), m_amp(ampl)
	{
		m_fspn.reset(new FSPN(m_para));
	}
	const lux::Vector VelocityNoiseVolume::eval(const lux::Vector& P) const
	{
		float nx = m_fspn->eval(P + m_dx) * m_amp;
		float ny = m_fspn->eval(P + m_dy) * m_amp;
		float nz = m_fspn->eval(P + m_dz) * m_amp;

		return lux::Vector(nx, ny, nz);
	}
}
