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

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "INT3: " << i << ", " << j << ", " << k;
			return ss.str();
		}

		int i;
		int j;
		int k;
	};

	struct GridPara
	{
		lux::Vector center;
		lux::Vector dimesion;
		INT3 resolution;
		int partionSize;
	};


	inline std::ostream& operator<<(std::ostream& os, const INT3& e)
	{
		return os << e.ToString();
	}

	template<class T>
	struct NotEqual 
	{
		T level;
		NotEqual(T _level) { level = _level; }
		bool operator ()(T num) const 
		{
			return num != level;
		}
	};

	template<class T>
	struct Larger
	{
		T level;
		Larger(T _level) { level = _level; }
		bool operator ()(T num) const
		{
			return num > level;
		}
	};

	template<class T>
	class SparseGrid
	{
	public:
		//SparseGrid(const lux::Vector& llc, const lux::Vector& ruc, INT3 resolution, int partionSize);
		
		SparseGrid(const lux::Vector& center, const lux::Vector& dimension, INT3 resolution, int partionSize);
		
		~SparseGrid() 
		{
			CleanData();
		};

		void CleanData()
		{
			//ALALBA_ERROR("Clean Grid");
			if (m_data != nullptr)
			{
				for (int i = 0; i < m_blockDimension.i * m_blockDimension.j * m_blockDimension.k; i++)
				{
					if (m_data[i] != nullptr)
						delete[] m_data[i];
				}
				delete[] m_data;
			}
		}

		template<class F>
		void Allocate(std::shared_ptr< lux::Volume<T> > field, F is);

		void Set(INT3 index3d, const T& value);
		
		const T& Get(int i,int j, int k) const;
		const T& Get(INT3 index3d) const;

		void StampGrid(std::shared_ptr< lux::Volume<T> > fieldptr);

		INT3 Index(int index1d);
		int Index(INT3 index3d);
		INT3 Index(const lux::Vector& P);
		
		T eval(const lux::Vector& P);

		bool isInside(const lux::Vector& P) const;
		bool isInside(int i, int j, int k) const;

		T** data() { return m_data; }
	
		const std::vector<int> assignedBolcks() 
		{ 
			std::vector<int> Index;
			for (int i = 0; i < m_blockDimension.i * m_blockDimension.j * m_blockDimension.k; i++)
			{
				if (m_data[i] != nullptr)
					Index.push_back(i);
			}

			return Index;

		}
		int blocksize() {  return m_partionSize * m_partionSize * m_partionSize;}
		int fullblocks() {
			return m_blockDimension.i * m_blockDimension.j * m_blockDimension.k
				;
		}
		
	//private:
	public:
		lux::Vector m_center;
		lux::Vector m_dimension;

		lux::Vector LLRC;
		lux::Vector RUFC;

		

		// how many points along each axis
		INT3 m_resolution;

		// how many points in the block along each axis
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
		SparseGridVolume(const lux::Vector& center, const lux::Vector& dimesion, INT3 resolution, int partionSize)
			:center(center), dimesion(dimesion), m_resolution(resolution), m_partionSize(partionSize)
		{
			sparseGridPtr = std::make_shared<SparseGrid<T>>(center, dimesion, m_resolution, m_partionSize);
			
		};
		~SparseGridVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return sparseGridPtr->eval(P); };

		// const &
		std::shared_ptr < SparseGrid<T> > Grid() { return sparseGridPtr; }

		const T& DefaultValue()const { return sparseGridPtr->m_defaultValue; }

	private:
		lux::Vector center;
		lux::Vector dimesion;

		INT3 m_resolution;
		int m_partionSize;
		
		std::shared_ptr < SparseGrid<T> > sparseGridPtr;

	};

	template<typename T>
	std::shared_ptr<lux::Volume<T>> Grid(const lux::Vector& center, const lux::Vector& dimesion, INT3 resolution, int partionSize,
		const std::shared_ptr< lux::Volume<T> >& fieldPtr)
	{

		std::shared_ptr<SparseGridVolume<T>> grid = std::make_shared< SparseGridVolume<T> >(center, dimesion, resolution, partionSize);
		
		// SHOULD allocate first, but for efficiency consideration, comment this out temporaly
		grid->Grid()->Allocate(fieldPtr, NotEqual<T>(T()));
		grid->Grid()->StampGrid(fieldPtr);

		return grid;
	}

}