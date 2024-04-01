#pragma once
#include "Noise.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"
namespace Alalba
{
	struct WispPara
	{
		int numchilderen;
		float den;
		Noise_t fspn1;
		Noise_t fspn2;
		float clump = 0.3;
		lux::Vector dP = lux::Vector(0.1,0.1,0.1);
		lux::Vector P;
		float slocal;
	};

	class Wisp
	{
	public:
		Wisp(const GridPara& gridPara, const WispPara& wispPara);
		~Wisp() {}

		GridPara m_gridPara;
		WispPara m_para;

		std::shared_ptr<SparseGridVolume<float>> m_grid;
		//std::shared_ptr<FSPNVolume> m_fspn1;
		//std::shared_ptr<FSPNVolume> m_fspn2;
	};
}


