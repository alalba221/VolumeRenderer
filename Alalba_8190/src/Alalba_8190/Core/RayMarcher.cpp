#include "pch.h"
#include "RayMarcher.h"
#include "omp.h"
namespace Alalba
{
	RayMarcher::RayMarcher(const lux::Camera& camera, 
		signed int width, signed int height, double deltaS, double K, double minT)
		:m_camera(camera), m_width(width), m_height(height), m_deltaS(deltaS), m_K(K), m_minTransmisity(minT)
	{
		m_Lps.resize(width * height);
		m_Transmissivities.resize(width * height);

		m_Positions.resize(width * height);
		m_distances.resize(width * height);

		m_Directions.resize(width * height);
		m_nearPlanes.resize(width * height);
		m_farPlanes.resize(width * height);

#pragma omp parallel for
		for (signed int pixel = 0; pixel < m_width * m_height; pixel++)
		{
			// initialize m_Directions
			double x = (double)(pixel % m_width) / (double)m_width;
			double y = (double)(pixel / m_width) / (double)m_height;
			m_Directions[pixel] = m_camera.view(x, y);

			// initialize the distance for each ray to near and far plane
			double cosine = m_Directions[pixel] * m_camera.view();
			m_nearPlanes[pixel] = camera.nearPlane() / cosine;
			m_farPlanes[pixel] = camera.farPlane() / cosine;
		}
	}
	
	void RayMarcher::MarchSigleRay(signed int index,
		const ScalarField& densityField,
		const ColorField& colorField,
		const PointLight& keyLight, const PointLight& fillLight, const PointLight& rimLight)
	{
		lux::Vector direction = m_Directions[index];
		double nearDistance = m_nearPlanes[index];
		double farDistance = m_farPlanes[index];

		m_Lps[index] = lux::Color(0.0, 0.0, 0.0, 0.0);
		
		m_Transmissivities[index] = 1.0;

		m_Positions[index] = m_camera.eye() + nearDistance * direction;
		m_distances[index] = nearDistance;

		while (m_distances[index] < farDistance && m_Transmissivities[index] > m_minTransmisity)
		{
			
			float desity = densityField->eval(m_Positions[index]);

			double deltaT = 0.0;

			if (desity >= 0)
			{
			
				lux::Color color_material = colorField->eval(m_Positions[index]);
				
				float keyT = (keyLight.TL)->eval(m_Positions[index]);
				float rimT = (rimLight.TL)->eval(m_Positions[index]);
				float fillT = (fillLight.TL)->eval(m_Positions[index]);

				//ALALBA_ERROR("Transmisity: {0},{1},{2}", keyT, rimT, fillT);
				lux::Color key = keyLight.Color() * keyT;
				lux::Color rim = rimLight.Color() * rimT;
				lux::Color fill = fillLight.Color() * fillT;
	
				lux::Color color = color_material *( key + rim + fill);
				//lux::Color color = color_material ;

				deltaT = exp(-m_K * m_deltaS * desity);

				m_Lps[index] += color * (1.0 - deltaT) * m_Transmissivities[index] / m_K;

				m_Transmissivities[index] *= deltaT;
			} // end if
			else
			{
				break;
			}
			m_Positions[index] += direction*m_deltaS;
			m_distances[index] += m_deltaS;
		}//end while

		m_Lps[index][3] = 1.0 - m_Transmissivities[index];

	}

	void RayMarcher::RayMarch(const ScalarField& densityFiled, const ColorField& colorFiled,
		const PointLight& keyLight, const PointLight& fillLight, const PointLight& rimLight)
	{

#pragma omp parallel for
		for (signed int pixel = 0; pixel < m_width * m_height; pixel++)
		{
			MarchSigleRay(pixel, densityFiled, colorFiled, keyLight, fillLight, rimLight);
		}// end for

		ALALBA_INFO("Ray Marching Done");
	}

}
