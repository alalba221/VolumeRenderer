#pragma once

#include "Camera.h"
#include "Volume.h"
namespace Alalba
{
	class RayMarcher
	{
	public:
		RayMarcher(const lux::Camera& camera, signed int width, signed int height);
		~RayMarcher() { Clean(); }
		void Clean() { };
		//void MarchSigleLight(const lux::Vector& np, const lux::Volume<float>& sdf);
		void MarchSigleLight(signed int index, const lux::Volume<float>& sdf,
			const lux::Volume<float>& densityField,
			const lux::Volume<lux::Color>& colorField);

		void RayMarch(const lux::Volume<float>& sdf, const lux::Volume<float>& densityFiled,
			const lux::Volume<lux::Color>& colorFiled);

		const std::vector<lux::Color>& GetResult()const { return m_result; };
	private:

		const class lux::Camera& m_camera;
		signed int m_width;
		signed int m_height;

		
		lux::Color m_Lp = lux::Color();
		std::vector<lux::Color> m_Lps;

		//double m_T = 1.0;
		std::vector<double> m_Ts;

		lux::Vector m_X;
		std::vector<lux::Vector> m_Xs;

		double m_S;
		std::vector<double> m_Ss;

		double m_deltaS = 0.1;

		double m_K = 2.0;

		std::vector<lux::Color> m_result;

	};
}


