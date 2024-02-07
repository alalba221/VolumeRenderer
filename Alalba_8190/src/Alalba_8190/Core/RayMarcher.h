#pragma once

#include "Camera.h"
#include "Volume.h"
#include "SDFs/Fields.h"
namespace Alalba
{
	class RayMarcher
	{
	public:
		RayMarcher(const lux::Camera& camera, signed int width, signed int height, double deltaS = 0.1, double K = 2.0, double minT=0.0);
		~RayMarcher() { Clean(); }
		void Clean() { };
		//void MarchSigleLight(const lux::Vector& np, const lux::Volume<float>& sdf);
		void MarchSigleLight(signed int index, const ScalarField& densityField, const ColorField& colorField);

		void RayMarch( const ScalarField& densityFiled, const ColorField& colorFiled);

		const std::vector<lux::Color>& GetResult()const { return m_Lps; };
	private:

		const class lux::Camera& m_camera;
		signed int m_width;
		signed int m_height;

		
		lux::Color m_Lp = lux::Color();

		// record irradiance of each pixel
		std::vector<lux::Color> m_Lps;

		std::vector<double> m_Transmissivities;
		
		// record current position along each ray
		std::vector<lux::Vector> m_Positions;

		// record current distance along each ray
		std::vector<double> m_distances;
		

		// directions for each ray
		std::vector<lux::Vector> m_Directions;
		
		// the distance for each ray to near and far plane
		std::vector<double> m_nearPlanes;
		std::vector<double> m_farPlanes;


		double m_deltaS ;

		double m_K;

		double m_minTransmisity;

	};
}


