#pragma once
#include "Camera.h"
#include "Volume.h"
#include "RayMarcher.h"
#include "Alalba_8190/Core/Light/PointLight.h"

namespace Alalba 
{
	class Renderer
	{
	public:
		Renderer(signed int width, signed int height, double deltaS = 0.02);
		~Renderer() {};
		
		void Render(const lux::Camera& camera, const ScalarField& densityField, const ColorField& colorField,
			const PointLight & keyLight, const PointLight& fillLight, const PointLight& rimLight);

		bool SaveImage(const char* outfilename);

	private:

		signed int m_width;
		signed int m_height;
		double  m_rayMarcherDeltaS;
		std::vector< lux::Color > m_frameBuffer;
		
		std::unique_ptr<RayMarcher> m_rayMarcher;

	};
}
