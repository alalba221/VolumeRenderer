#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"
#include "FSPN.h"

namespace Alalba
{
	class VelocityNoiseVolume : public lux::Volume < lux::Vector >
	{
	public:

		VelocityNoiseVolume(const Noise_t& param, 
			const lux::Vector& dx , const lux::Vector& dy, const lux::Vector& dz,
			float ampl);

		~VelocityNoiseVolume() {};

		Noise_t& Parameter() { return m_para; }

		virtual const lux::Vector eval(const lux::Vector& P) const override;
		//virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

	private:

		Noise_t m_para;
		std::unique_ptr<FSPN> m_fspn;
		lux::Vector m_dx;
		lux::Vector m_dy;
		lux::Vector m_dz;
		float m_amp;

	};

	template<typename T>
	std::shared_ptr<lux::Volume<T>> VelocityNoise(const Noise_t& param,
		const lux::Vector& dx, const lux::Vector& dy, const lux::Vector& dz,
		float ampl)
	{
		return std::make_shared< VelocityNoiseVolume >(param, dx,dy,dz, ampl);
	}
}

