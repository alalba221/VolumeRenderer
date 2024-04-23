#include "pch.h"
#include "Pyroclastic.h"
#include "Alalba_8190/Core/Operations/CPT.h"
namespace Alalba
{
	PyroclasticVolume::PyroclasticVolume(const Noise_t& param, ScalarField sdf, float ampl)
		:m_para(param),m_sdf(sdf),m_amp(ampl)
	{
		m_fspn.reset(new FSPN(m_para));
		m_cpt = CPT(m_sdf);
	}

	const float PyroclasticVolume::eval(const lux::Vector& P) const
	{

		float f = m_sdf->eval(P);
		
		//lux::Vector cpt = P - m_sdf->eval(P)*m_sdf->grad(P);
		lux::Vector cpt = m_cpt->eval(P);
		return f+m_amp*std::abs(m_fspn->eval(cpt));
	}


}
