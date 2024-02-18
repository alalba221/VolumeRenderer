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
		return std::abs(distance);
		//lux::Vector projectedP = p + triangle.normal * distance;


		//// if can be projected onto triangle
		//if (IsPointInTriangle(projectedP, triangle))
		//{
		//	return std::abs(distance);
		//	//return 1000.0;
		//}
		//
		////TODO: return distance to edge
		//return 1000.0;
	}



	std::shared_ptr<SparseGridVolume<float>> LevelSet(const Mesh& mesh, INT3 resolution, int partionSize, int bandwidth)
	{
		float gridInitValue = 10000.0;

		std::shared_ptr<SparseGridVolume<float>> grid 
			= std::make_shared< SparseGridVolume<float> >(mesh.center, mesh.dimension, resolution, partionSize);


		lux::Vector llc = mesh.LLFC;
		lux::Vector dimesion = mesh.dimension;
		lux::Vector precision = lux::Vector(dimesion.X()/ resolution.i, dimesion.Y() / resolution.j, dimesion.Z() / resolution.k);



		// Process A
//#pragma omp parallel for
		// for each triangle
		for (auto Triangle : mesh.Triangles())
		{
			// 1. Find the rectangular set of grid points bounding the triangle

			INT3 llc_p0 = grid->Grid()->Index(Triangle.p0);
			INT3 ruc_p0 = INT3( llc_p0.i + 1,llc_p0.j + 1,llc_p0.k + 1 );

			INT3 llc_p1 = grid->Grid()->Index(Triangle.p1);
			INT3 ruc_p1 = INT3(llc_p1.i + 1, llc_p1.j + 1, llc_p1.k + 1);

			INT3 llc_p2 = grid->Grid()->Index(Triangle.p2);
			INT3 ruc_p2 = INT3(llc_p2.i + 1, llc_p2.j + 1, llc_p2.k + 1);

			int min_i = 0, min_j = 0, min_k = 0;
			int max_i = resolution.i, max_j = resolution.j, max_k = resolution.k;

			min_i = std::min(std::min(llc_p0.i, llc_p1.i), llc_p2.i);
			min_j = std::min(std::min(llc_p0.j, llc_p1.j), llc_p2.j);
			min_k = std::min(std::min(llc_p0.k, llc_p1.k), llc_p2.k);

			max_i = std::max(std::max(ruc_p0.i, ruc_p1.i), ruc_p2.i);
			max_j = std::max(std::max(ruc_p0.j, ruc_p1.j), ruc_p2.j);
			max_k = std::max(std::max(ruc_p0.k, ruc_p1.k), ruc_p2.k);

			// 2. Expand the rectangular set by the size of the bandwidth in each direction
			INT3 boundingBoxLLCIndex = INT3(min_i, min_j, min_k);
			INT3 boundingBoxRUCIndex = INT3(max_i, max_j, max_k);

			INT3 ext_boundingBoxLLCIndex = INT3(min_i-bandwidth, min_j-bandwidth, min_k-bandwidth);
			INT3 ext_boundingBoxRUCIndex = INT3(max_i+bandwidth, max_j+bandwidth, max_k+bandwidth);

			// 3 .For each grid point in the set, compute the minimum distance from that point to the triangle
			std::vector<INT3> indeices;
			for (int i = ext_boundingBoxLLCIndex.i; i < ext_boundingBoxRUCIndex.i; i++)
				for (int j = ext_boundingBoxLLCIndex.j; j < ext_boundingBoxRUCIndex.j; j++)
					for (int k = ext_boundingBoxLLCIndex.k; k < ext_boundingBoxRUCIndex.k; k++)
					{
						if (i > boundingBoxLLCIndex.i && i< boundingBoxRUCIndex.i
							&& j> boundingBoxLLCIndex.j && j < boundingBoxRUCIndex.j
							&& k> boundingBoxLLCIndex.k && k < boundingBoxRUCIndex.k)
							continue;
						
						indeices.push_back({ i,j,k });

					}

			//init all grid to default value
			for (auto index : indeices)
			{
				grid->Grid()->Set(index, gridInitValue);
			}

			for (auto index : indeices)
			{

				lux::Vector p = lux::Vector(llc.X()+index.i* precision.X(),
					llc.Y() + index.j * precision.Y(),
					llc.Z() - index.k * precision.Z());
				
				float distance = DistanceToTriangle(p, Triangle);
				//ALALBA_ERROR(distance);
				float gridvalue = grid->Grid()->Get(index);

				if (distance < gridvalue)
				{	
					grid->Grid()->Set(index, distance);
				}
			}

		} // end  for each triangle

		/// PROCESS B







//		// TODO:: clean sparse block
//		std::vector<int> blocks = grid->Grid()->assignedBolcks();
//
//		int blocksize = grid->Grid()->blocksize();
//		int fullblocks = grid->Grid()->fullblocks();
//		float epsilon = 0.0001f;
//
////#pragma omp parallel for
//
//		for (int i = 0; i < fullblocks; i++)
//		{
//			if ((grid->Grid()->data())[i] != nullptr)
//			{
//				for (int j = 0; j < blocksize; j++)
//				{
//					float gridvalue = (grid->Grid()->data())[i][j];
//					if (std::fabs(gridvalue - gridInitValue) < epsilon)
//					{
//						ALALBA_ERROR(gridInitValue);
//						(grid->Grid()->data())[i][j] = 0.0f;
//					}
//					else
//					{
//						//ALALBA_ERROR(gridvalue);
//					}
//						
//				}
//			}
//		}

		//for (int i=0; i < blocks.size(); i++)
		//{
		//	//ALALBA_ERROR("{0} OF {1}", i, blocks.size());
		//	if ((grid->Grid()->data())[blocks[i]] != nullptr)
		//	{
		//		for (int j = 0; j < blocksize; j++)
		//		{
		//			float gridvalue = (grid->Grid()->data())[blocks[i]][j];

		//			if (std::fabs(gridvalue - gridInitValue) < epsilon)
		//				(grid->Grid()->data())[blocks[i]][j] = 0.0f;
		//		}
		//	}
		//}


		return grid;
	}
}
