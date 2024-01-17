#include "pch.h"
#include "RayMarcher.h"
#include "omp.h"
namespace Alalba
{
	RayMarcher::RayMarcher(const lux::Camera& camera, signed int width, signed int height)
		:m_camera(camera), m_width(width), m_height(height) 
	{
		m_result.resize(width * height);
		m_Lps.resize(width * height);
		m_Ts.resize(width * height);

		m_Xs.resize(width * height);
		m_Ss.resize(width * height);
	}
	
	void RayMarcher::MarchSigleLight(signed int index, const lux::Volume<float>& sdf,
		const lux::Volume<float>& densityField,
		const lux::Volume<lux::Color>& colorField)
	{
		double x = (double)(index % m_width) / (double)m_width;
		double y = (double)(index / m_width) / (double)m_height;
		lux::Vector direction = m_camera.view(x, y);

		m_Lps[index] = lux::Color(0.0, 0.0, 0.0, 0.0);
		
		m_Ts[index] = 1.0;
		m_Xs[index] = m_camera.eye() + m_camera.nearPlane() * direction;
		m_Ss[index] = m_camera.nearPlane();

		while (m_Ss[index] < m_camera.farPlane())
		{

			m_Xs[index] += m_deltaS * direction;
			m_Ss[index] += m_deltaS;

			//float desity = sdf.eval(m_Xs[index])<=0?0.0:1.0;
			float desity = densityField.eval(m_Xs[index]);
			double deltaT = 0.0;

			if (desity >= 0)
			{
				//const lux::Color color = lux::Color(1.0,0.0,0.0,0.0);
				const lux::Color color = colorField.eval(m_Xs[index]);
				deltaT = exp(-m_K * m_deltaS * desity);

				m_Lps[index] += color * (1.0 - deltaT) * m_Ts[index] / m_K;

				m_Ts[index] *= deltaT;
			} // end if
		}//end while

		m_Lps[index][3] = 1.0 - m_Ts[index];

		m_result[index] = m_Lps[index];
	}

	void RayMarcher::RayMarch(const lux::Volume<float>& sdf,
		const lux::Volume<float>& densityFiled,
		const lux::Volume<lux::Color>& colorFiled)
	{
#pragma omp parallel for
		for (signed int pixel = 0; pixel < m_width * m_height; pixel++)
		{
			//ALALBA_TRACE(": {0} / {1}", pixel, m_width * m_height);
			//double x = (double)(pixel % m_width)/(double)m_width;
			//double y = (double)(pixel / m_width)/(double)m_height;

			//lux::Vector direction = m_camera.view(x, y);
		
			MarchSigleLight(pixel, sdf, densityFiled, colorFiled);
			//m_result[pixel] = m_Lp;
		}// end for

		
		ALALBA_INFO("Ray Marching Done");
	}

}
