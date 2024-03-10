#pragma once
#include "Volume.h"
#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"

namespace Alalba
{
	class PointLight
	{
	public:
		PointLight(const lux::Color& color, const lux::Vector& postion);
		~PointLight() {  };
	
		void GenerateDSM(const ScalarField& densityField, const lux::Vector& center, 
			const lux::Vector& dimesion, INT3 resolution, int partionSize, double deltaS, double Kappa);
		
		const std::shared_ptr<SparseGridVolume<float>>& DSM() const { return m_dsm; };

		const lux::Color& Color()const {return m_color; }
		ScalarField TL;
	private:
		void MatchSingleRay(INT3 index, const ScalarField& densityField, double deltaS, double Kappa);
	private:

		std::shared_ptr<SparseGridVolume<float>> m_dsm;
		
		lux::Color m_color;
		lux::Vector m_position;
	};
}
