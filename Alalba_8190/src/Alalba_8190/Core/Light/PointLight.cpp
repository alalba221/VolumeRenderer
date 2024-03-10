#include "pch.h"
#include "PointLight.h"


namespace Alalba
{
	PointLight::PointLight(const lux::Color& color, const lux::Vector& postion)
		:m_color(color),m_position(postion)
	{
	}
	
	
	void PointLight::GenerateDSM(const ScalarField& densityField, const lux::Vector& center, 
		const lux::Vector& dimesion, INT3 resolution, int partionSize, double deltaS, double Kappa)
	{
		m_dsm = std::make_shared< SparseGridVolume<float> >(center, dimesion, resolution, partionSize);
	
		// 1. allocate memory dsm's blocks according to densityField, 
		// so the grid->set() function won't new memory for blocks
		m_dsm->Grid()->Allocate(densityField);
		
		// 2 parllely compute transmitity for each points
#pragma omp for
		for (int index1D = 0; index1D < resolution.i * resolution.j * resolution.k; index1D++)
		{
			INT3 index3d = m_dsm->Grid()->Index(index1D);
			MatchSingleRay(index3d, densityField, deltaS, Kappa);
		}

		//m_dsm = Alalba::EXP (m_dsm);

	}


	void PointLight::MatchSingleRay(INT3 index, const ScalarField& densityField, double deltaS, double Kappa)
	{
		lux::Vector grid_precision = m_dsm->Grid()->m_precision;
		lux::Vector offset = lux::Vector(index.i * grid_precision.X(), index.j * grid_precision.Y(), index.k * grid_precision.Z());
		lux::Vector pos = m_dsm->Grid()->LLRC + offset;

		double smax = (m_position - pos).magnitude();
		lux::Vector direction = (m_position - pos).unitvector();

		double TL = 1.0;
		double arg = 0;

		double s = 0;
		float density = densityField->eval(pos);
		
		if (density <= 0.0)
			return;

		while (s < smax)
		{
			density = densityField->eval(pos);
			if (density > 0)
			{
				arg += densityField->eval(pos) * deltaS;
				pos += direction * deltaS;
				s += deltaS;
			}
			else
			{
				break;
			}
		}
		//m_dsm->Grid()->Set(index, std::exp( - Kappa * arg));
		m_dsm->Grid()->Set(index, -Kappa * arg);
	}
}