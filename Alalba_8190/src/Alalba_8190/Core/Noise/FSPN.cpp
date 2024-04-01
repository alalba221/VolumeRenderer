#include "pch.h"
#include "FSPN.h"
namespace Alalba
{

	FSPN::FSPN(const Noise_t& param)
		:m_para(param)
	{
		normalized_term = (1 - m_para.roughness) / (1 - std::pow(m_para.roughness, m_para.octaves));

		fjump_pow_n.resize(m_para.octaves);
		roughness_pow_n.resize(m_para.octaves);

		for (int i = 0; i < m_para.octaves; i++)
		{
			fjump_pow_n[i] = std::pow(m_para.fjump, i);
			roughness_pow_n[i] = std::pow(m_para.roughness, i);
		}
	}

	const float FSPN::eval(const lux::Vector& postion) const
	{
		//// for circlr only 
		//float radius = 2.0;

		//float distance = m_sdf->eval(postion);
		//lux::Vector cpt = (postion - lux::Vector(0, 0, 0)).unitvector() * radius;

		lux::Vector p_with_freq = (postion - m_para.translate) * m_para.frequency;

		float result = 0;

		for (int q = 0; q < m_para.octaves; q++)
		{
			lux::Vector temp_fjump = p_with_freq * fjump_pow_n[q];
			result += roughness_pow_n[q] * db::perlin(temp_fjump.X(), temp_fjump.Y(), temp_fjump.Z());
		}

		result *= normalized_term;

		return result;
	}

}