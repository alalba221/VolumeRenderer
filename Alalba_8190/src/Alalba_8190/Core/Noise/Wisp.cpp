#include "pch.h"
#include "Wisp.h"
#include <random>

namespace Alalba
{
	WispVolume::WispVolume(const GridPara& gridPara, const WispPara& wispPara)
		:m_gridPara(gridPara),m_para(wispPara)
	{

		m_fspn1.reset(new FSPN(m_para.fspn1));
		m_fspn2.reset(new FSPN(m_para.fspn2));

		m_grid = std::make_shared< SparseGridVolume<float> >(gridPara.center, gridPara.dimesion, gridPara.resolution, gridPara.partionSize);


		CreateDots();
	}
	const float WispVolume::eval(const lux::Vector& P) const
	{
		return m_grid->eval(P);
	}
	void WispVolume::CreateDots()
	{
		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(-1.0, 1.0);
	
		for (int i = 0; i < m_para.numchilderen; i++)
		{
			// 1. Select a random position
			float x = distribution(generator);
			float y = distribution(generator);
			float z = distribution(generator);
			//ALALBA_ERROR(i);
			CreateDot(x,y,z);
		}
	}
	void WispVolume::CreateDot(float x, float y, float z)
	{
		// 1. Select a random position
		//std::default_random_engine generator;
		//std::uniform_real_distribution<double> distribution(-1.0, 1.0);
		//float x = distribution(generator);
		//float y = distribution(generator);
		//float z = distribution(generator);



		lux::Vector p0(x, y, z);
		// 2. Project the position to the unit sphere
		lux::Vector p1 = p0/p0.magnitude();

		// 3. Create a radius from FSPN
		float r = std::pow(std::fabs(m_fspn1->eval(p0)), m_para.clump);

		// 4. Move position to that radius
		lux::Vector p3 = p1*r;

		// 5. Apply local-to-global coordinate transform
		lux::Vector p4 = m_para.Plocal + p3*m_para.slocal;

		// 6. Generate a displacement vector
		float dx = m_fspn2->eval(p3);
		float dy = m_fspn2->eval(p3+m_para.dP);
		float dz = m_fspn2->eval(p3-m_para.dP);

		lux::Vector d(dx, dy, dz);

		// 7.Move to displaced position
		lux::Vector p5 = p4 + d;
		//ALALBA_ERROR(p5);
		// 8. Bake density value den at the grid location p5
		BakeDot(p5);
	}

	void WispVolume::BakeDot(const lux::Vector& p)
	{
		lux::Vector LLRC = m_grid->LLRC();
		lux::Vector precision = m_grid->Precision();

		lux::Vector index_float(
			(p.X() - LLRC.X()) / precision.X(),
			(p.Y() - LLRC.Y()) / precision.Y(),
			(p.Z() - LLRC.Z()) / precision.Z()
		);
		
		INT3 closet_ijk = m_grid->Grid()->Index(p);

		lux::Vector weight(
			index_float.X() - closet_ijk.i, 
			index_float.Y() - closet_ijk.j, 
			index_float.Z() - closet_ijk.k
		);

		// i,j,k
		INT3 current_index = INT3(closet_ijk.i, closet_ijk.j ,closet_ijk.k );
		float current_value = m_grid->Get(current_index);
		float current_weight = (1.0 - weight.X()) * (1.0 - weight.Y()) * (1.0 - weight.Z());
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i+1,j,k
		current_index = INT3(closet_ijk.i+1, closet_ijk.j, closet_ijk.k);
		current_value = m_grid->Get(current_index);
		current_weight = weight.X() * (1.0 - weight.Y()) * (1.0 - weight.Z());
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i,j+1,k
		current_index = INT3(closet_ijk.i, closet_ijk.j+1, closet_ijk.k);
		current_value = m_grid->Get(current_index);
		current_weight = (1.0 - weight.X()) * weight.Y() * (1.0 - weight.Z());
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i,j,k+1
		current_index = INT3(closet_ijk.i, closet_ijk.j, closet_ijk.k+1);
		current_value = m_grid->Get(current_index);
		current_weight = (1.0 - weight.X()) * (1.0 - weight.Y()) * weight.Z();
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i+1,j+1,k
		current_index = INT3(closet_ijk.i+1, closet_ijk.j+1, closet_ijk.k);
		current_value = m_grid->Get(current_index);
		current_weight = weight.X() * weight.Y() * (1.0-weight.Z());
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i+1,j,k+1
		current_index = INT3(closet_ijk.i+1, closet_ijk.j, closet_ijk.k + 1);
		current_value = m_grid->Get(current_index);
		current_weight = weight.X() * (1.0 - weight.Y()) * weight.Z();
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i,j+1,k+1
		current_index = INT3(closet_ijk.i, closet_ijk.j + 1, closet_ijk.k + 1);
		current_value = m_grid->Get(current_index);
		current_weight = (1.0 - weight.X()) * weight.Y() * weight.Z();
		m_grid->Set(current_index, current_value + m_para.den * current_weight);

		// i+1,j+1,k+1
		current_index = INT3(closet_ijk.i + 1, closet_ijk.j + 1, closet_ijk.k + 1);
		current_value = m_grid->Get(current_index);
		current_weight = weight.X() * weight.Y() * weight.Z();
		m_grid->Set(current_index, current_value + m_para.den * current_weight);
	}
}
