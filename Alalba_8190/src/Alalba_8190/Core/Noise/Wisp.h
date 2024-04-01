#pragma once

#include "Alalba_8190/Core/Grid/SparseGrid.inl"
#include "FSPN.h"
namespace Alalba
{
	struct WispPara
	{
		int numchilderen = 5000000;
		float den = 1.0;
		Noise_t fspn1;
		Noise_t fspn2;
		float clump = 0.3;
		lux::Vector dP = lux::Vector(0.1,0.1,0.1);
		lux::Vector Plocal = lux::Vector(0.0,0.0,0.0);
		float slocal;
	};

	class WispVolume : public lux::Volume<float>
	{
	public:
		WispVolume(const GridPara& gridPara, const WispPara& wispPara);
		~WispVolume() {}

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };
	
	private:
		void CreateDots();
		void CreateDot(float x, float y, float z);
		void BakeDot(const lux::Vector& p);

		GridPara m_gridPara;
		WispPara m_para;

		std::shared_ptr<SparseGridVolume<float>> m_grid;
		std::unique_ptr<FSPN> m_fspn1;
		std::unique_ptr<FSPN> m_fspn2;
	};



	template<typename T>
	std::shared_ptr<lux::Volume<T>> Wisp(const GridPara& gridPara, const WispPara& wispPara)
	{
		return std::make_shared< WispVolume >(gridPara, wispPara);
	}
}


