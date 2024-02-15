#pragma once
#include "SparseGrid.h"
#include "omp.h"
namespace Alalba
{
	template <class T>
	SparseGrid<T>::SparseGrid(const lux::Vector& llc, const lux::Vector& ruc, INT3 resolution, int partionSize)
		:LLFC(llc), RURC(ruc), m_resolution(resolution), m_partionSize(partionSize)
	{
		m_blockDimension = INT3(resolution.i / partionSize, resolution.j / partionSize, resolution.k / partionSize);

		m_data = new T * [m_blockDimension.i * m_blockDimension.j * m_blockDimension.k];
		for (int i = 0; i < m_blockDimension.i * m_blockDimension.j * m_blockDimension.k; i++)
		{
			m_data[i] = nullptr;
		}

		lux::Vector difference = RURC - LLFC;
		lux::Vector distance = difference.abs();
		m_precision = lux::Vector(distance.X() / m_resolution.i, distance.Y() / m_resolution.j, distance.Z() / m_resolution.k);

	};


	template <class T>
	void SparseGrid<T>::Set(INT3 index3d, const T& value)
	{
		
		//0. todo: check if in the grid
		if (!isInside(index3d.i, index3d.j, index3d.k))
			return;
		int ib = index3d.i / m_partionSize;
		int jb = index3d.j / m_partionSize;
		int kb = index3d.k / m_partionSize;
		int block_index = ib + m_blockDimension.i * (jb + m_blockDimension.j * kb);

		// check whether this block has been allocated. If not, allocate it and set to default value
		if (m_data[block_index] == nullptr && value != m_defaultValue)
		{
			m_data[block_index] = new T[m_partionSize * m_partionSize * m_partionSize];
			for (int i = 0; i < m_partionSize * m_partionSize * m_partionSize; i++) 
			{ 
				m_data[block_index][i] = m_defaultValue;
			}
		}
		// find cell inside the block
		if(m_data[block_index]!=nullptr && value != m_defaultValue)
		{
			int icell = index3d.i % m_partionSize;
			int jcell = index3d.j % m_partionSize;
			int kcell = index3d.k % m_partionSize;
			int cell_index = icell + m_partionSize * (jcell + m_partionSize * kcell);
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

		double minX = LLFC.X();
		double minY = LLFC.Y();
		double maxZ = LLFC.Z();
		
		double maxX = RURC.X();
		double maxY = RURC.Y();
		double minZ = RURC.Z();

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

		lux::Vector difference = (P - LLFC);
		
		lux::Vector distancetoLLC = difference.abs();

		int i = distancetoLLC.X() / m_precision.X();
		int j = distancetoLLC.Y() / m_precision.Y();
		int k = distancetoLLC.Z() / m_precision.Z();

		if(isInside(i,j,k))
			return { i, j, k };
		return { -1,-1,-1 };
	}


	template <class T>
	void SparseGrid<T>::StampGrid(std::shared_ptr< lux::Volume<T> > fieldptr)
	{
		
#pragma omp parallel for
		for (int index1D = 0; index1D < m_resolution.i * m_resolution.j * m_resolution.k; index1D++)
		{
			INT3 index3d = Index(index1D);
		

			lux::Vector P = lux::Vector(LLFC.X() + m_precision.X() * index3d.i,
				LLFC.Y() + m_precision.Y() * index3d.j,
				LLFC.Z() - m_precision.Z() * index3d.k);
		
			T value= fieldptr->eval(P);
	
			Set(index3d, value);
		}
	}
	template <class T>
	bool SparseGrid<T>::isInside(int i, int j, int k) const
	{
		if (i < m_resolution.i && j < m_resolution.j && k < m_resolution.k && i >= 0 && j >= 0 && k >= 0)
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
		lux::Vector difference = P - LLFC;
		lux::Vector distancetoLLC = difference.abs();
		lux::Vector weight = lux::Vector(distancetoLLC.X() / m_precision.X() - closet_llc.i,
			distancetoLLC.Y() / m_precision.Y() - closet_llc.j,
			distancetoLLC.Z() / m_precision.Z() - closet_llc.k);

												
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