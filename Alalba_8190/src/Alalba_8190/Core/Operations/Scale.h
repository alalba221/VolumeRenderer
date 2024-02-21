#pragma once
#include "Volume.h"
#include "Fields.h"
#include<algorithm>

#include "Alalba_8190/Core/Grid/SparseGrid.inl"

namespace Alalba
{

	template<typename T>
	class ScaleVolume : public lux::Volume<T>
	{
	public:
		ScaleVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const lux::Vector& scale )
			:m_fieldPtr1(fieldPtr1), m_scale(scale) {};
		~ScaleVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T(); };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		lux::Vector m_scale;
	};

	// Color
	template<>
	virtual const float ScaleVolume<float>::eval(const lux::Vector& P) const 
	{
		lux::Vector scaled = lux::Vector(P.X() / m_scale.X(), P.Y() / m_scale.Y(), P.Z() / m_scale.Z());
		return m_fieldPtr1->eval(scaled);
	};
	


	//template<typename T>
	//std::shared_ptr<SparseGridVolume<T>> ScaleGrid(const std::shared_ptr< SparseGridVolume<T> >& fieldPtr1,
	//	const lux::Vector& scale)
	//{
	//	ALALBA_ERROR("SCALE SPARSE GRID");
	//	lux::Vector dimension = lux::Vector(fieldPtr1->Grid()->m_dimension.X() * scale.X(),
	//		fieldPtr1->Grid()->m_dimension.Y() * scale.Y(),
	//		fieldPtr1->Grid()->m_dimension.Z() * scale.Z());

	//	lux::Vector center = fieldPtr1->Grid()->m_center;
	//	INT3 resolution = fieldPtr1->Grid()->m_resolution;
	//	int partionSize = fieldPtr1->Grid()->m_partionSize;

	//	std::shared_ptr<SparseGridVolume<T>> result
	//		= std::make_shared< SparseGridVolume<T> >(center, dimension, resolution, partionSize);

	//	// copy data
	//	INT3 BlockDim = fieldPtr1->Grid()->m_blockDimension;

	//	for (int i = 0; i < BlockDim.i * BlockDim.j * BlockDim.k; i++)
	//	{
	//		if (fieldPtr1->Grid()->m_data[i] != nullptr)
	//		{
	//			result->Grid()->m_data[i] = new T[partionSize * partionSize * partionSize];
	//			for (int j = 0; j < partionSize * partionSize * partionSize; j++)
	//			{
	//				result->Grid()->m_data[i][j] = fieldPtr1->Grid()->m_data[i][j];
	//			}
	//		}
	//	}

	////	fieldPtr1->Grid()->CleanData();
	//	return result;
	//}

	template<typename T>
	std::shared_ptr<lux::Volume<T>> Scale(const std::shared_ptr< lux::Volume<T> >& fieldPtr1,
		const lux::Vector& scale)
	{
			return std::make_shared< ScaleVolume<T> >(fieldPtr1, scale);	
	}


}
