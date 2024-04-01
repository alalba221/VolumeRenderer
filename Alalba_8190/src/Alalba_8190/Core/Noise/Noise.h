#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"
#include "FSPN.h"
	
namespace Alalba
{
	class NoiseVolume : public lux::Volume<float>
	{
	public:

		NoiseVolume(const Noise_t& param, ScalarField sdf, float ampl);

		~NoiseVolume() {};

		Noise_t& Parameter() { return m_para; }

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

	private:

		Noise_t m_para;
		std::unique_ptr<FSPN> m_fspn;
		ScalarField m_sdf;
		float m_amp;

	};

	template<typename T>
	std::shared_ptr<lux::Volume<T>> Noise(const std::shared_ptr< lux::Volume<T> >& grided_sdf, const Noise_t& para, float ampl)
	{
		return std::make_shared< NoiseVolume >(para, grided_sdf, ampl);
	}
}


