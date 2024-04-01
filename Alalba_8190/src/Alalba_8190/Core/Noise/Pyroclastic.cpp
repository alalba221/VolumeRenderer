#include "pch.h"
#include "Pyroclastic.h"

namespace Alalba
{
	PyroclasticVolume::PyroclasticVolume(const Noise_t& param, ScalarField sdf, float ampl)
		:m_para(param),m_sdf(sdf),m_amp(ampl)
	{
		m_fspn.reset(new FSPN(m_para));

	}

	const float PyroclasticVolume::eval(const lux::Vector& P) const
	{
		// for circlr only 
		float radius = 2.0;
		lux::Vector center = lux::Vector(0, 0, 0);

		float f = m_sdf->eval(P);
		lux::Vector cpt = (P - center).unitvector() * radius;

		return f+m_amp*std::abs(m_fspn->eval(cpt));
	}


}
