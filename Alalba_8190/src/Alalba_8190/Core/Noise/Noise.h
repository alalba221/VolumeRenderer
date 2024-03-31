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
		float amplitude;
		int octaves;
	};


	class FSPNVolume : public lux::Volume<float>
	{
	public:
		
	/**
	* @brief
	* @tparam
	* @param sdf should be grided 
	* @return
	*/
		FSPNVolume(const Noise_t& param, ScalarField sdf);
		
		~FSPNVolume() {};
	
		Noise_t& Parameter() { return m_para; }

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

	private:
		Noise_t m_para;

		float normalized_term;
		std::vector<float> fjump_pow_n;
		std::vector<float> roughness_pow_n;

		ScalarField m_sdf;

	};



	/**
	 * @brief 
	 * @tparam T 
	 * @param grided_sdf 
	 * @param para 
	 * @return 
	*/
	template<typename T>
	std::shared_ptr<lux::Volume<T>> Noise(const std::shared_ptr< lux::Volume<T> >& grided_sdf, const Noise_t& para)
	{
		return std::make_shared(FSPNVolume (grided_sdf, para));
	}
}
