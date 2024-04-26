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
		float step = 2 * M_PI / 120.0;
		for (int i = 0; i < 120; i++)
		{
			ALALBA_INFO("{0}th start", i);

			auto start = std::chrono::system_clock::now();
			auto startframe = std::chrono::system_clock::now();
			
			std::string index = std::to_string(i);
			size_t n = 3;
			int precision = n - std::min(n, index.size());
			index.insert(0, precision, '0');
			
			std::string output = "image/" + index + ".exr";

			
			////
			lux::Matrix camRotateMatrix;

			camRotateMatrix = lux::rotation(lux::Vector(0.0, 1.0, 0.0), step);
			lux::Vector eye = camRotateMatrix * m_camera->eye();
			lux::Vector view = lux::Vector(0.0, 0.0, 0.0) - eye;
			m_camera->setEyeViewUp(eye, view, lux::Vector(0, 1, 0));
				
			
			/////

	
			m_renderer->Render(*m_camera.get(), density_grid, color_grid,*m_key,*m_fill,*m_rim);
			//m_renderer->Render(*m_camera.get(), bunny_grid, bunny_color_grid);
			m_renderer->SaveImage(output.c_str());


			auto end = std::chrono::system_clock::now();
			double elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - startframe).count();
			
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
		

		m_renderer.reset(new Alalba::Renderer(1920, 1080, rayStepSize));
		
		Alalba::ColorField redColor;
		redColor.reset(new Alalba::ConstantColor(lux::Color(1.0, 0.0/255, 127.0/255.0, 1.0)));

		Alalba::ColorField greenColor;
		greenColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 1.0, 0.0, 1.0)));

		Alalba::ColorField yellowColor;
		yellowColor.reset(new Alalba::ConstantColor(lux::Color(1.0, 1.0, 0.0, 1.0)));

		Alalba::ColorField blueColor;
		blueColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 0.0, 1.0, 1.0)));

		
		woodColor.reset(new Alalba::ConstantColor(lux::Color(86.0 / 255, 47.0 / 255.0, 14.0 / 255.0, 1.0)));


		//// Ajax
		Alalba::Mesh ajax = Alalba::Mesh("model/ajax.obj");

		ALALBA_INFO(ajax.m_triangles.size());
		ALALBA_INFO("Ajax Dimension:{0}", ajax.dimension);
		ALALBA_INFO("Ajax Center:{0}", ajax.center);
		ALALBA_INFO("Level Set Ajax");

		auto start = std::chrono::system_clock::now();

		ajax_grid = Alalba::LevelSet(ajax, ajax.dimension * 1.1, { 513,1025,513 }, 8, 4);

		

		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Level Set ajax Done {0}s\n", elapsed);



		//headSDF = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 1);
		headSDF = ajax_grid;
		// NOISE
		Alalba::Noise_t parm;
		//parm.amplitude = 1.5;
		parm.frequency = 0.5;
		parm.octaves = 4;
		parm.roughness = 0.8;
		parm.translate = lux::Vector(0.0, 0.0, 0.0);

		parm.center = lux::Vector(0.0, 1.0, 0.0);
		parm.radius = 1.0;
		parm.fade = 0.8;

		lux::Vector dx(0,0,0);
		lux::Vector dy(1, 1, 1);
		lux::Vector dz(-1, -1, -1);

		ALALBA_INFO("Velocity noise Field");
		start = std::chrono::system_clock::now();
		velocity = Alalba::VelocityNoise < lux::Vector > (parm, dx, dy, dz, 8.5);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Velocity noise Field Field Done {0}s", elapsed);
		
		ALALBA_INFO("Grid Velocity Field");
		velocity = Alalba::Grid < lux::Vector > (lux::Vector(0.0, 1.0, 0.0), lux::Vector(2.0, 2.0, 2.0), { 513,513,513 }, 4, velocity);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Grid Velocity Field Done {0}s", elapsed);
		
		//velocity = Alalba::Grid < lux::Vector >(lux::Vector(0.0, 1.0, 0.0), lux::Vector(4, 1.5, 4), { 513,513,513 }, 4, velocity);
		headDensity = Alalba::Clamp<float>(headSDF, 0.0f, 1.0f);

		float dt = 3.0;
		int N = 5;

		float deltaT = dt / N;

		for (int i = 0; i < N; i++)
		{
			headDensity = Alalba::Addvect(headDensity, velocity, deltaT);
		}
		 
		Alalba::ScalarField mask = Alalba::Mask<float>(headDensity);
		headColor = Alalba::Multiply<lux::Color>(woodColor, mask);

		
		ALALBA_INFO("Grid Humanoid Density Field");
		start = std::chrono::system_clock::now();
		density_grid = Alalba::Grid<float>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, headDensity);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
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
	
	Alalba::VectorField velocity;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;

	std::unique_ptr<Alalba::PointLight> m_key;
	std::unique_ptr<Alalba::PointLight> m_fill;
	std::unique_ptr<Alalba::PointLight> m_rim;

	Alalba::ScalarField ajax_grid;

};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}