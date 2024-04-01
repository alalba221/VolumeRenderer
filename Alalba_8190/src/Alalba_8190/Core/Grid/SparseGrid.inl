#pragma once
#include "SparseGrid.h"
#include "omp.h"
namespace Alalba
{

	template <class T>
	SparseGrid<T>::SparseGrid(const lux::Vector& center, const lux::Vector& dimension, INT3 resolution, int partionSize)
		:m_center(center), m_dimension(dimension), m_resolution(resolution), m_partionSize(partionSize)
	{

		m_blockDimension = INT3(resolution.i / partionSize + 1, resolution.j / partionSize + 1, resolution.k / partionSize + 1);

		//ALALBA_ERROR(m_blockDimension);

		LLRC = lux::Vector(m_center.X() - m_dimension.X() / 2, m_center.Y() - m_dimension.Y() / 2, m_center.Z() - m_dimension.Z() / 2);
		RUFC = lux::Vector(m_center.X() + m_dimension.X() / 2, m_center.Y() + m_dimension.Y() / 2, m_center.Z() + m_dimension.Z() / 2);

		m_data = new T * [m_blockDimension.i * m_blockDimension.j * m_blockDimension.k];
		for (int i = 0; i < m_blockDimension.i * m_blockDimension.j * m_blockDimension.k; i++)
		{
			m_data[i] = nullptr;
		}

		lux::Vector difference = LLRC - RUFC;
		lux::Vector distance = difference.abs();
		m_precision = lux::Vector(distance.X() / (m_resolution.i-1), distance.Y() / (m_resolution.j-1), distance.Z() / (m_resolution.k-1));

	};

	template<class T>
	bool DefaultCondition(T value)
	{
		if (value != T())
			return true;
		return false;
	}

	template<class T>
		template<class F>
	inline void SparseGrid<T>::Allocate(std::shared_ptr<lux::Volume<T>> field, F is)
	{
		int num_block = m_blockDimension.i * m_blockDimension.j * m_blockDimension.k;
		int num_grid = m_partionSize * m_partionSize * m_partionSize;
#pragma omp parallel for
		for (int block = 0; block < num_block; block++)
		{
			int i_block = block % m_blockDimension.i;
			int j_block = (block / m_blockDimension.i) % m_blockDimension.j;
			int k_block = block / (m_blockDimension.i * m_blockDimension.j);

			int i_base = i_block * m_partionSize;
			int j_base = j_block * m_partionSize;
			int k_base = k_block * m_partionSize;

			bool validBlock = false;
			
			for (int grid = 0; grid < num_grid; grid++)
			{
				//0. get position
				int i_grid = grid % m_partionSize;
				int j_grid = (grid / m_partionSize) % m_partionSize;
				int k_grid = grid / (m_partionSize * m_partionSize);

				INT3 index = { i_base + i_grid, j_base + j_grid ,k_base + k_grid };

				lux::Vector offset = lux::Vector(index.i * m_precision.X(), index.j * m_precision.Y(), index.k * m_precision.Z());
				lux::Vector pos = LLRC + offset;
				//ALALBA_ERROR(pos);
				if (is(field->eval(pos)))
				//if ((field->eval(pos)) != T())
				{
					
					validBlock = true;
					//ALALBA_ERROR(block);
					break;
				}
			}

			if (validBlock)
			{
				
				m_data[block] = new T[m_partionSize * m_partionSize * m_partionSize];

				for (int i = 0; i < m_partionSize * m_partionSize * m_partionSize; i++)
				{
					m_data[block][i] = m_defaultValue;
				}
			}
		}
		
	
	}

	template <class T>
	void SparseGrid<T>::Set(INT3 index3d, const T& value)
	{
		//ALALBA_ERROR("SET");
		
		//0. todo: check if in the grid
		if (!isInside(index3d.i, index3d.j, index3d.k))
			return;
		int ib = index3d.i / m_partionSize;
		int jb = index3d.j / m_partionSize;
		int kb = index3d.k / m_partionSize;
		int block_index = ib + m_blockDimension.i * (jb + m_blockDimension.j * kb);

		// check whether this block has been allocated. If not, allocate it and set to default value
//#pragma omp critical
		{
			if (m_data[block_index] == nullptr && value != m_defaultValue)
			{
				m_data[block_index] = new T[m_partionSize * m_partionSize * m_partionSize];

				for (int i = 0; i < m_partionSize * m_partionSize * m_partionSize; i++)
				{
					m_data[block_index][i] = m_defaultValue;
				}
			}
		}
		// find cell inside the block

		if((m_data[block_index]!=nullptr) && (value != m_defaultValue))
		{
			
			//
			int icell = index3d.i % m_partionSize;
			int jcell = index3d.j % m_partionSize;
			int kcell = index3d.k % m_partionSize;
			//ALALBA_ERROR("cell {0} {1} {2}", icell, jcell, kcell);
			int cell_index = icell + m_partionSize * (jcell + m_partionSize * kcell);
			//ALALBA_ERROR("{0},{1}",block_index, cell_index);
			m_data[block_index][cell_index] = value;
		}
		
	}



	template <class T>
	const T& SparseGrid<T>::Get(int i, int j, int k) const
	{
		if (!isInside(i, j, k))
			return m_defaultValue;

		// find which block i,j,k is in
		int ib = i / m_partionSize;
		int jb = j / m_partionSize;
		int kb = k / m_partionSize;
		int block_index = ib + m_blockDimension.i * (jb + m_blockDimension.j * kb);
		// check whether this block has been allocated. If not, return default value
		if (m_data[block_index] == nullptr)
		{
			return m_defaultValue;
		}
		// find cell inside the block
		int icell = i % m_partionSize;
		int jcell = j % m_partionSize;
		int kcell = k % m_partionSize;
		int cell_index = icell + m_partionSize * (jcell + m_partionSize * kcell);
		return m_data[block_index][cell_index];
	}

	template <class T>
	const T& SparseGrid<T>::Get(INT3 index3d) const
	{
		return Get(index3d.i, index3d.j, index3d.k);
	}

	template <class T>
	INT3 SparseGrid<T>::Index(int index1d)
	{
		int width = m_resolution.i;
		int height = m_resolution.j;

		int z = index1d / (width * height); // Calculate the depth index
		index1d %= (width * height);    // Remainder after removing depth contribution
		int y = index1d / width;            // Calculate the height index
		int x = index1d % width;            // Calculate the width index

		return { x, y, z };
	}
	
	template <class T>
	int SparseGrid<T>::Index(INT3 index3d)
	{
		int width = m_resolution.i;
		int height = m_resolution.j;
		int index1D = index3d.k * (width * height) + index3d.j * width + index3d.i;

		return index1D;
	}
	template <class T>
	bool SparseGrid<T>::isInside(const lux::Vector& P) const
	{
		double x = P.X();
		double y = P.Y();
		double z = P.Z();

		double minX = LLRC.X();
		double minY = LLRC.Y();
		double minZ = LLRC.Z();
		
		double maxX = RUFC.X();
		double maxY = RUFC.Y();
		double maxZ = RUFC.Z();

		if (x <= maxX && x >= minX
			&& y <= maxY && y >= minY
			&& z <= maxZ && z >= minZ)
			return true;

		return false;
		
	}


	template <class T>
	INT3 SparseGrid<T>::Index(const lux::Vector& P)
	{
		if(!isInside(P))
			return { -1,-1,-1 };
		
		lux::Vector difference = (P - LLRC);
		// Debug
		//ALALBA_ERROR("dIFF {0}",difference);
		lux::Vector distancetoLLC = difference.abs();

		int i = distancetoLLC.X() / m_precision.X();
		int j = distancetoLLC.Y() / m_precision.Y();
		int k = distancetoLLC.Z() / m_precision.Z();


		if(isInside(i,j,k))
			return { i, j, k };
		//return { -1,-1,-1 };
	}


	template <class T>
	void SparseGrid<T>::StampGrid(std::shared_ptr< lux::Volume<T> > fieldptr)
	{	
#pragma omp parallel for
		for (int index1D = 0; index1D < m_resolution.i * m_resolution.j * m_resolution.k; index1D++)
		{
			INT3 index3d = Index(index1D);
		

			lux::Vector P = lux::Vector(LLRC.X() + m_precision.X() * index3d.i,
				LLRC.Y() + m_precision.Y() * index3d.j,
				LLRC.Z() + m_precision.Z() * index3d.k);
		
			T value= fieldptr->eval(P);
	
			Set(index3d, value);
		}
	}
	template <class T>
	bool SparseGrid<T>::isInside(int i, int j, int k) const
	{
		if (i <m_resolution.i && j < m_resolution.j && k < m_resolution.k && i >= 0 && j >= 0 && k >= 0)
		{
			return true;
		}
		return false;
	}
	template <class T>
	T SparseGrid<T>::eval(const lux::Vector& P)
	{

		INT3 closet_llc = Index(P);
		int i = closet_llc.i;
		int j = closet_llc.j;
		int k = closet_llc.k;

		//ALALBA_ERROR("{0},{1},{2}", i, j, k);

		// weight
		lux::Vector difference = P - LLRC;
		lux::Vector distancetoLLC = difference.abs();
		lux::Vector weight = lux::Vector(distancetoLLC.X() / m_precision.X() - closet_llc.i,
			distancetoLLC.Y() / m_precision.Y() - closet_llc.j,
			distancetoLLC.Z() / m_precision.Z() - closet_llc.k);

		double weightX = weight.X();
		double weightY = weight.Y();
		double weightZ = weight.Z();

		T result = Get(i, j, k) * (1 - weight.X()) * (1 - weight.Y()) * (1 - weight.Z())
			+ Get(i + 1, j, k) * weight.X() * (1 - weight.Y()) * (1 - weight.Z())
			+ Get(i, j + 1, k) * (1 - weight.X()) * weight.Y() * (1 - weight.Z())
			+ Get(i, j, k + 1) * (1 - weight.X()) * (1 - weight.Y()) * weight.Z()
			+ Get(i + 1, j + 1, k) * weight.X() * weight.Y() * (1 - weight.Z())
			+ Get(i + 1, j, k + 1) * weight.X() * (1 - weight.Y()) * weight.Z()
			+ Get(i, j + 1, k + 1) * (1 - weight.X()) * weight.Y() * weight.Z()
			+ Get(i + 1, j + 1, k + 1) * weight.X() * weight.Y() * weight.Z();

		
		return result;

	}
}