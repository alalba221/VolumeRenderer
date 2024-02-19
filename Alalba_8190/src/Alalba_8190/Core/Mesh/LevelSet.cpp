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

		float distance = p_p0 * triangle.normal;
		//ALALBA_ERROR(distance);
		return std::abs(distance);
		lux::Vector projectedP = p + triangle.normal * distance;


		// if can be projected onto triangle
		if (IsPointInTriangle(projectedP, triangle))
		{
			return std::abs(distance);
			//return 1000.0;
		}
		
		//TODO: return distance to edge
		return 1000.0;
	}



	std::shared_ptr<SparseGridVolume<float>> LevelSet(const Mesh& mesh, INT3 resolution, int partionSize, int bandwidth)
	{
		float gridInitValue = 10000.0;

		

		std::shared_ptr<SparseGridVolume<float>> grid
			 //= std::make_shared< SparseGridVolume<float> >(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), resolution, partionSize);
		//= std::make_shared< SparseGridVolume<float> >(mesh.center, mesh.dimension*2.0, resolution, partionSize);
		= std::make_shared< SparseGridVolume<float> >(mesh.center, lux::Vector(4.0, 4.0, 4.0), resolution, partionSize);


		lux::Vector llc = mesh.LLRC;
		lux::Vector dimesion = mesh.dimension;
		lux::Vector precision = lux::Vector(dimesion.X()/ resolution.i, dimesion.Y() / resolution.j, dimesion.Z() / resolution.k);

		//lux::Vector llc = grid->Grid()->LLRC;
		//lux::Vector dimesion = grid->Grid()->m_dimension;
		//lux::Vector precision = lux::Vector(dimesion.X() / resolution.i, dimesion.Y() / resolution.j, dimesion.Z() / resolution.k);



		// Process A
#pragma omp parallel for
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

			// 2. Expand the rectangular set by the size of the bandwidth in each direction
			INT3 boundingBoxLLCell = INT3(min_i, min_j, min_k);
			INT3 boundingBoxRUCell = INT3(max_i, max_j, max_k);

			INT3 ext_boundingBoxLLCell = INT3(min_i - bandwidth, min_j - bandwidth, min_k - bandwidth);
			INT3 ext_boundingBoxRUCell = INT3(max_i + bandwidth, max_j + bandwidth, max_k + bandwidth);

			// 3 .For each grid point in the set, compute the minimum distance from that point to the triangle
			std::vector<INT3> cell_indeices;
			for (int i = ext_boundingBoxLLCell.i; i <= ext_boundingBoxLLCell.i; i++)
				for (int j = ext_boundingBoxLLCell.j; j <= ext_boundingBoxLLCell.j; j++)
					for (int k = ext_boundingBoxLLCell.k; k <= ext_boundingBoxLLCell.k; k++)
					{
						if (i > boundingBoxLLCell.i && i< boundingBoxLLCell.i
							&& j> boundingBoxLLCell.j && j < boundingBoxLLCell.j
							&& k> boundingBoxLLCell.k && k < boundingBoxLLCell.k)
							continue;

						if(grid->Grid()->isInside(i,j,k))
							cell_indeices.push_back({ i,j,k });

					}

			//init all grid to default value
			for (auto index : cell_indeices)
			{
				//ALALBA_INFO("{0},{1},{2}", index.i, index.j, index.k);
				grid->Grid()->Set(index, gridInitValue);
			}

			for (auto index : cell_indeices)
			{

				lux::Vector p = lux::Vector(llc.X()+index.i* precision.X(),
					llc.Y() + index.j * precision.Y(),
					llc.Z() + index.k * precision.Z());
				
				float distance = DistanceToTriangle(p, Triangle);
				//ALALBA_ERROR(distance);
				float gridvalue = grid->Grid()->Get(index);

				if (distance < gridvalue)
				{	
					grid->Grid()->Set(index, distance*15.0);
				}
			}

		} // end  for each triangle

		/// PROCESS B


		return grid;
	}
}
