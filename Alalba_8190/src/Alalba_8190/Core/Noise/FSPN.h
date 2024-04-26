#pragma once
#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
#include "Alalba_8190/Core/Grid/SparseGrid.inl"
#include "db_perlin.hpp"

namespace Alalba
{
	struct Noise_t
	{
		float frequency;
		float roughness;
		lux::Vector translate = lux::Vector();
		float fjump = 2.0;
		//float amplitude = 1.0;
		int octaves;
		lux::Vector center = lux::Vector();
		float fade = 0.8;
		float radius = 1.0;
	};


	class FadeFSPN
	{
	public:

		FadeFSPN(const Noise_t& param);

		~FadeFSPN() {};

		Noise_t& Parameter() { return m_para; }

		const float eval(const lux::Vector& P) const;
		//virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

	private:
		Noise_t m_para;


		float normalized_term;
		std::vector<float> fjump_pow_n;
		std::vector<float> roughness_pow_n;

		ScalarField m_sdf;
	};

	class FSPN
	{
	public:

		FSPN(const Noise_t& param);

		~FSPN() {};

		Noise_t& Parameter() { return m_para; }

		const float eval(const lux::Vector& P) const;
		//virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

	private:
		Noise_t m_para;

		float normalized_term;
		std::vector<float> fjump_pow_n;
		std::vector<float> roughness_pow_n;

		ScalarField m_sdf;

	};
}
