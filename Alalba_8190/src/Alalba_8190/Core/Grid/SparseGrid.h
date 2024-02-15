#pragma once
#include "Volume.h"
#include "Fields.h"

namespace Alalba
{

	

	struct INT3
	{
		INT3(int x, int y, int z) 
		:i(x),j(y),k(z) {}
		INT3():i(0), j(0), k(0) {}
		int i;
		int j;
		int k;
	};


	template<class T>
	class SparseGrid
	{
	public:
		SparseGrid(const lux::Vector& llc, const lux::Vector& ruc, INT3 resolution, int partionSize);

		~SparseGrid() 
		{
			for (int i = 0; i < m_blockDimension.i * m_blockDimension.j * m_blockDimension.k; i++)
			{
				if (m_data[i] != nullptr)
					delete[] m_data[i];
			}
			delete[] m_data;
		};

		void Set(INT3 index3d, const T& value);
		
		const T& Get(int i,int j, int k) const;

		void StampGrid(std::shared_ptr< lux::Volume<T> > fieldptr);

		INT3 Index(int index1d);
		int Index(INT3 index3d);
		INT3 Index(const lux::Vector& P);
		
		T eval(const lux::Vector& P);

		bool isInside(const lux::Vector& P) const;
		bool isInside(int i, int j, int k) const;

	private:

		lux::Vector LLFC;
		lux::Vector RURC;

		// how many cells along each axis
		INT3 m_resolution;

		// how many cells in the block along each axis
		int m_partionSize;

		// how many blocks along each axis
		INT3 m_blockDimension;

		// size of each cell
		lux::Vector m_precision;

		T m_defaultValue = T();

		T** m_data;
	};


	typedef std::shared_ptr<SparseGrid<float>> ScalarSparseGrid;
	typedef std::shared_ptr < SparseGrid<lux::Color>> ColorSparseGrid;
	
	/// SparseGridVolume
	template<typename T>
	class SparseGridVolume : public lux::Volume<T>
	{
	public:
		SparseGridVolume(const lux::Vector& llc, const lux::Vector& ruc, INT3 resolution, int partionSize, 
			std::shared_ptr< lux::Volume<T> > fieldPtr)
			:LLFC(llc), RURC(ruc), m_resolution(resolution), m_partionSize(partionSize), m_fieldPtr(fieldPtr)
		{
			sparseGridPtr = std::make_shared<SparseGrid<T>>(LLFC, RURC, m_resolution, m_partionSize);
			//sparseGridPtr->StampGrid(m_fieldPtr);
		};
		~SparseGridVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return sparseGridPtr->eval(P); };

		// const &
		std::shared_ptr < SparseGrid<T> > Grid() { return sparseGridPtr; }

	private:

		lux::Vector LLFC;
		lux::Vector RURC;

		INT3 m_resolution;
		int m_partionSize;
		std::shared_ptr< lux::Volume<T> > m_fieldPtr;

		std::shared_ptr < SparseGrid<T> > sparseGridPtr;

	};

	template<typename T>
	std::shared_ptr<SparseGridVolume<T>> Grid(const lux::Vector& llc, const lux::Vector& ruc, INT3 resolution, int partionSize,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr)
	{

		std::shared_ptr<SparseGridVolume<T>> grid = std::make_shared< SparseGridVolume<T> >(llc,ruc, resolution, partionSize, fieldPtr);
		grid->Grid()->StampGrid(fieldPtr);

		return grid;
	}

}