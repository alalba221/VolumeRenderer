#include <Alalba.h>
#include "tinyexr.h"
#include "LinearAlgebra.h"
class Sandbox : public Alalba::Application
{

//typedef std::shared_ptr < lux::Volume<lux::Color>> ColorField;
public:
	Sandbox()
	{

	}


	~Sandbox()
	{

	}

	virtual void OnUpdate() override
	{
		for (int i = 0; i < 500; i++)
		{
			ALALBA_INFO("{0}th start", i);

			auto start = std::chrono::system_clock::now();
			
			std::string index = std::to_string(i);
			size_t n = 3;
			int precision = n - std::min(n, index.size());
			index.insert(0, precision, '0');
			
			std::string output = "image/" + index + ".exr";

			
//////////////////

			headSDF = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);

			// NOISE
			Alalba::Noise_t parm;
			parm.amplitude = 1.5;
			parm.frequency = 0.5;
			parm.octaves = 4;
			parm.roughness = 0.8;
			parm.translate = lux::Vector(0.0, 0.0, -0.1*i);

			//Alalba::ScalarField fspn;
			fspn.reset(new Alalba::FSPNVolume(parm, headSDF));
			
			headSDF = Alalba::Add<float>(headSDF, fspn);

			
			Alalba::ScalarField mask = Alalba::Mask<float>(headSDF);
			headColor = Alalba::Multiply<lux::Color>(woodColor, mask);




			density_grid = Alalba::Grid<float>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, headDensity);
	
			color_grid = Alalba::Grid<lux::Color>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, headColor);




			///////////////////////

	
			m_renderer->Render(*m_camera.get(), density_grid, color_grid,*m_key,*m_fill,*m_rim);
			//m_renderer->Render(*m_camera.get(), bunny_grid, bunny_color_grid);
			m_renderer->SaveImage(output.c_str());


			auto end = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			
			ALALBA_ERROR("{0} Done, {1}s", output, elapsed);

		}
		
	}

	virtual void OnInit() override
	{

		double rayStepSize = 0.03;

		m_camera.reset(new lux::Camera());
		m_camera->setEyeViewUp(lux::Vector(0.0, 0.0, 7.0), lux::Vector(0.0, 0.0, -1.0), lux::Vector(0.0, 1.0, 0.0));
		m_camera->setNearPlane(5.);
		m_camera->setFarPlane(9.);
		

		m_renderer.reset(new Alalba::Renderer(960, 540, rayStepSize));
		
		Alalba::ColorField redColor;
		redColor.reset(new Alalba::ConstantColor(lux::Color(1.0, 0.0/255, 127.0/255.0, 1.0)));

		Alalba::ColorField greenColor;
		greenColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 1.0, 0.0, 1.0)));

		Alalba::ColorField yellowColor;
		yellowColor.reset(new Alalba::ConstantColor(lux::Color(1.0, 1.0, 0.0, 1.0)));

		Alalba::ColorField blueColor;
		blueColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 0.0, 1.0, 1.0)));

		
		woodColor.reset(new Alalba::ConstantColor(lux::Color(86.0 / 255, 47.0 / 255.0, 14.0 / 255.0, 1.0)));


		/// 0. circle
		

		headSDF = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);

		// NOISE
		Alalba::Noise_t parm;
		parm.amplitude = 1.5;
		parm.frequency = 0.5;
		parm.octaves = 4;
		parm.roughness = 0.8;
		parm.translate = lux::Vector(0.0, 0.0, 0.0);

		//Alalba::ScalarField fspn;
		fspn.reset(new Alalba::FSPNVolume(parm, headSDF));

		headSDF = Alalba::Add<float>(headSDF, fspn);

		headDensity = Alalba::Clamp<float>(headSDF, 0.0f, 1.0f);
		
		Alalba::ScalarField mask = Alalba::Mask<float>(headSDF);
		headColor = Alalba::Multiply<lux::Color>(woodColor, mask);

		


		ALALBA_INFO("Grid Humanoid Density Field");
		auto start = std::chrono::system_clock::now();
		density_grid = Alalba::Grid<float>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, headDensity);
		auto end = std::chrono::system_clock::now();
		double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Grid Humanoid Density Field Done {0}s", elapsed);
		
		ALALBA_INFO("Grid Humanoid Color Field");
		start = std::chrono::system_clock::now();
		color_grid = Alalba::Grid<lux::Color>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, headColor);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Grid Humanoid Color Field Done {0}s", elapsed);

	

	

		

		// lights
		lux::Vector keyPos = lux::Vector(0, 4, 0);
		m_key.reset( new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0)*10.0, keyPos));

		lux::Vector fillPos = lux::Vector(0, -4, 0);
		m_fill.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0)*2.0, fillPos));

		lux::Vector rimPos = lux::Vector(0, 0, -4);
		m_rim.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0)*4.0, rimPos));
		
		ALALBA_TRACE("Key dsm");
		m_key->GenerateDSM(density_grid, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), {200,200,200 },8, 0.1,2.0);
		ALALBA_TRACE("Fill dsm");
		m_fill->GenerateDSM(density_grid, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
		ALALBA_TRACE("Rim dsm");
		m_rim->GenerateDSM(density_grid, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
		
	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:

	// test noise
	Alalba::ColorField woodColor;

	Alalba::ScalarField headSDF;
	Alalba::ScalarField headDensity;
	Alalba::ColorField headColor;
	// 

	Alalba::ScalarField density_grid;
	Alalba::ColorField color_grid;
	
	Alalba::ScalarField fspn;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;

	std::unique_ptr<Alalba::PointLight> m_key;
	std::unique_ptr<Alalba::PointLight> m_fill;
	std::unique_ptr<Alalba::PointLight> m_rim;



};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}