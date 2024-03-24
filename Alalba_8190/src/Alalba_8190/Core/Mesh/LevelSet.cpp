#include "pch.h"
#include "LevelSet.h"
namespace Alalba
{

	typedef struct Ray {
		lux::Vector startPoint = lux::Vector(0, 0, 0);    // 起点
		lux::Vector direction = lux::Vector(0, 0, 0);     // 方向
	}Ray;

	bool IsPointInTriangle(lux::Vector p, Triangle triangle)
	{
		lux::Vector e1 = triangle.p1 - triangle.p0;
		lux::Vector e2 = triangle.p2 - triangle.p0;

		lux::Vector p0_p = p - triangle.p0;

		lux::Vector cross_e2e1 = e2 ^ e1;

		float L2_e2e1 = cross_e2e1.X() * cross_e2e1.X() + cross_e2e1.Y() * cross_e2e1.Y() + cross_e2e1.Z() * cross_e2e1.Z();

		float u = (e2 ^ p0_p) * cross_e2e1 / L2_e2e1;
		float v = -(e1 ^ p0_p) * cross_e2e1 / L2_e2e1;

		return (u >= 0) && (v >= 0) && (u + v < 1);

	}

	float DistanceToTriangle(lux::Vector p, Triangle triangle)
	{
		lux::Vector p_p0 = triangle.p0 - p;
		//lux::Vector p_p0 = p - triangle.p0;

		float distance = p_p0 * triangle.normal;
		
		return distance;

		//return std::abs(distance);
		//lux::Vector projectedP = p + triangle.normal * distance;


		//// if can be projected onto triangle
		//if (IsPointInTriangle(projectedP, triangle))
		//{
		//	return std::abs(distance);
		//	//return 1000.0;
		//}
		//else
		//{
		//	//TODO: return distance to edge
		//}
		//
		//return 1000.0;
	}



	std::shared_ptr<SparseGridVolume<float>> LevelSet(const Mesh& mesh, lux::Vector griddimesion,INT3 resolution, int partionSize, int bandwidth)
	{
		float gridInitValue = 10000.0;

		

		std::shared_ptr<SparseGridVolume<float>> grid
		= std::make_shared< SparseGridVolume<float> >(mesh.center, griddimesion, resolution, partionSize);

		lux::Vector llc = grid->Grid()->LLRC;
		lux::Vector dimesion = griddimesion;
		lux::Vector precision = lux::Vector(dimesion.X() / resolution.i, dimesion.Y() / resolution.j, dimesion.Z() / resolution.k);

		// Process A
		//#pragma omp parallel for
		// TODO: PARALLEL THIS PART
		for(int i =0;i< mesh.Triangles().size();i++)
		{
			// 1. Find the rectangular set of grid points bounding the triangle
			Triangle Triangle = mesh.Triangles()[i];

			INT3 cell_p0 = grid->Grid()->Index(Triangle.p0);
	
			INT3 cell_p1 = grid->Grid()->Index(Triangle.p1);
			
			INT3 cell_p2 = grid->Grid()->Index(Triangle.p2);
		

			int min_i = 0, min_j = 0, min_k = 0;
			int max_i = resolution.i, max_j = resolution.j, max_k = resolution.k;

			min_i = std::min(std::min(cell_p0.i, cell_p1.i), cell_p2.i);
			min_j = std::min(std::min(cell_p0.j, cell_p1.j), cell_p2.j);
			min_k = std::min(std::min(cell_p0.k, cell_p1.k), cell_p2.k);

			max_i = std::max(std::max(cell_p0.i, cell_p1.i), cell_p2.i);
			max_j = std::max(std::max(cell_p0.j, cell_p1.j), cell_p2.j);
			max_k = std::max(std::max(cell_p0.k, cell_p1.k), cell_p2.k);

			if (!(max_i < resolution.i && max_j < resolution.j && max_k < resolution.k && min_i >= 0 && min_j >= 0 && min_k >= 0))
			{
				//ALALBA_ERROR("INVALID INDEC {0}",i);
				continue;
			}
			else
			{
				//ALALBA_INFO("VALID INDEC {0}", i);
			}
			
			// 2. Expand the rectangular set by the size of the bandwidth in each direction
			
			INT3 boundingBoxLLCell = INT3(min_i, min_j, min_k);
			INT3 boundingBoxRUCell = INT3(max_i, max_j, max_k);

			INT3 ext_boundingBoxLLCell = INT3(min_i - bandwidth, min_j - bandwidth, min_k - bandwidth);
			INT3 ext_boundingBoxRUCell = INT3(max_i + bandwidth, max_j + bandwidth, max_k + bandwidth);
			//ALALBA_ERROR("STEP 3");
			// 3 .For each grid point in the set, compute the minimum distance from that point to the triangle
			std::vector<INT3> grid_indeices;

			if (!(ext_boundingBoxRUCell.i < resolution.i && ext_boundingBoxRUCell.j < resolution.j
				&& ext_boundingBoxRUCell.k < resolution.k 
				&& ext_boundingBoxLLCell.i >= 0 
				&& ext_boundingBoxLLCell.j >= 0 
				&& ext_boundingBoxLLCell.k >= 0))
			{
				//ALALBA_ERROR("INVALID INDEC {0}", i);
				continue;
			}


			for (int i = ext_boundingBoxLLCell.i; i <= ext_boundingBoxRUCell.i; i++)

				for (int j = ext_boundingBoxLLCell.j; j <= ext_boundingBoxRUCell.j; j++)
					for (int k = ext_boundingBoxLLCell.k; k <= ext_boundingBoxRUCell.k; k++)
					{
						
						if (i > boundingBoxLLCell.i && i< boundingBoxRUCell.i
							&& j> boundingBoxLLCell.j && j < boundingBoxRUCell.j
							&& k> boundingBoxLLCell.k && k < boundingBoxRUCell.k)
							continue;

						if (grid->Grid()->isInside(i, j, k))
						{
							//ALALBA_INFO("{0},{1},{2}", i, j, k);
							grid_indeices.push_back({ i,j,k });
						}
					}

			//ALALBA_ERROR("STEP 4");
			//init all grid to default value
			for (auto index : grid_indeices)
			{
				grid->Grid()->Set(index, gridInitValue);
			}
			//ALALBA_ERROR("STEP 5");
			for (auto index : grid_indeices)
			{

				lux::Vector p = lux::Vector(llc.X()+index.i* precision.X(),
					llc.Y() + index.j * precision.Y(),
					llc.Z() + index.k * precision.Z());
				
				float distance = DistanceToTriangle(p, Triangle);
				float gridvalue = grid->Grid()->Get(index);

				if (std::abs(distance) < std::abs(gridvalue))
				{	
					grid->Grid()->Set(index, std::abs( distance));
				}
			}

		} // end  for each triangle

		/// PROCESS B

//		INT3 block_dim = grid->Grid()->m_blockDimension;
//		int partitionSize = grid->Grid()->m_partionSize;
//		int num_blocks = block_dim.i * block_dim.j * block_dim.k;
//		int num_grids_per_block = partitionSize * partitionSize * partitionSize;
//
//		lux::Vector llc = grid->Grid()->LLRC;
//
//		lux::Vector precision = grid->Grid()->m_precision;
//
//		for (int block = 0; block < num_blocks; block++)
//		{
//			if (grid->Grid()->data()[block] == nullptr)
//				continue;
//			int i_block = block % block_dim.i;
//			int j_block = (block / block_dim.i) % block_dim.j;
//			int k_block = block / (block_dim.i * block_dim.j);
//
//			int i_base = i_block * partitionSize;
//			int j_base = j_block * partitionSize;
//			int k_base = k_block * partitionSize;
//#pragma omp parallel for
//			for (int grid = 0; grid < num_grids_per_block; grid++)
//			{
//				int i_grid = grid % partitionSize;
//				int j_grid = (grid / partitionSize) % partitionSize;
//				int k_grid = grid / (partitionSize * partitionSize);
//
//				INT3 index = { i_base + i_grid, j_base + j_grid ,k_base + k_grid };
//
//				lux::Vector offset = lux::Vector(index.i * precision.X(), index.j * precision.Y(), index.k * precision.Z());
//				lux::Vector pos = llc + offset;
//
//
//				//MatchSingleRay(index, densityField, deltaS, Kappa);
//			}
//		}

		return grid;
	}
}
