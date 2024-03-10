#pragma once

#include "Camera.h"
#include "Volume.h"
#include "ImplicitFunction/Fields.h"
#include "Alalba_8190/Core/Light/PointLight.h"
namespace Alalba
{
	class RayMarcher
	{
	public:
		RayMarcher(const lux::Camera& camera,
			signed int width, signed int height, double deltaS = 0.01, double K = 2.0, double minT=0.0);
		~RayMarcher() { Clean(); }
		void Clean() { };
		//void MarchSigleLight(const lux::Vector& np, const lux::Volume<float>& sdf);
		void MarchSigleRay(signed int index, const ScalarField& densityField, const ColorField& colorField, 
			const PointLight& keyLight, const PointLight& fillLight, const PointLight& rimLight);

		void RayMarch( const ScalarField& densityFiled, const ColorField& colorFiled,
			const PointLight& keyLight, const PointLight& fillLight, const PointLight& rimLight);

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


