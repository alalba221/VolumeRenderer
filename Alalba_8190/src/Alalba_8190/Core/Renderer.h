#pragma once
#include "Camera.h"
#include "Volume.h"
#include "RayMarcher.h"


namespace Alalba 
{
	class Renderer
	{
	public:
		Renderer(signed int width, signed int height);
		~Renderer() {};
		
		void Render(const lux::Camera& camera, const ScalarField& densityField, const ColorField& colorField );

		bool SaveImage(const char* outfilename);

	private:

		signed int m_width;
		signed int m_height;
		std::vector< lux::Color > m_frameBuffer;
		
		std::unique_ptr<RayMarcher> m_rayMarcher;

	};
}
