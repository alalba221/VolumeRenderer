#include "pch.h"
#include "PointLight.h"
#include "Alalba_8190/Core/Operations/Operations.h"
#include "omp.h"
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
		auto start = std::chrono::system_clock::now();
		ALALBA_INFO("Allocate Sparse Grid for light");
		m_dsm->Grid()->Allocate(densityField);
		auto end = std::chrono::system_clock::now();
		double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Sparse Grid Allocate Done {0}s", elapsed);
		
		// 2 parllely compute transmitity for each points
		start = std::chrono::system_clock::now();
		
		
		INT3 block_dim = m_dsm->Grid()->m_blockDimension;
		int partitionSize = m_dsm->Grid()->m_partionSize;
		int num_blocks = block_dim.i * block_dim.j * block_dim.k;
		int num_grids_per_block = partitionSize * partitionSize * partitionSize;

		for (int block = 0; block < num_blocks; block++)
		{
			if (m_dsm->Grid()->data()[block] == nullptr)
				continue;
			int i_block = block % block_dim.i;
			int j_block = (block / block_dim.i) % block_dim.j;
			int k_block = block / (block_dim.i * block_dim.j);

			int i_base = i_block * partitionSize;
			int j_base = j_block * partitionSize;
			int k_base = k_block * partitionSize;
#pragma omp parallel for
			for (int grid = 0; grid < num_grids_per_block; grid++)
			{
				int i_grid = grid % partitionSize;
				int j_grid = (grid / partitionSize) % partitionSize;
				int k_grid = grid / (partitionSize * partitionSize);

				INT3 index = { i_base + i_grid, j_base + j_grid ,k_base + k_grid };

				MatchSingleRay(index, densityField, deltaS, Kappa);
			}
		}
//#pragma omp parallel for
		/*for (int index1D = 0; index1D < resolution.i * resolution.j * resolution.k; index1D++)
		{
			INT3 index3d = m_dsm->Grid()->Index(index1D);
			MatchSingleRay(index3d, densityField, deltaS, Kappa);
		}*/

		TL = EXP<float>(m_dsm);

		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("DSM Done {0}s", elapsed);
	}


	void PointLight::MatchSingleRay(INT3 index, const ScalarField& densityField, double deltaS, double Kappa)
	{
		//ALALBA_ERROR(index);

		lux::Vector grid_precision = m_dsm->Grid()->m_precision;
		lux::Vector offset = lux::Vector(index.i * grid_precision.X(), index.j * grid_precision.Y(), index.k * grid_precision.Z());
		lux::Vector pos = m_dsm->Grid()->LLRC + offset;

		double smax = (m_position - pos).magnitude();
		//ALALBA_ERROR("smax:{0}",smax);
		lux::Vector direction = (m_position - pos).unitvector();

		double TL = 1.0;
		double arg = 0;

		double s = 0;
		float density = densityField->eval(pos);
		
		if (density <= 0.0)
			return;

		while (s < smax)
		{
			//ALALBA_INFO("s: {1}, smax:{0}", smax,s);
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