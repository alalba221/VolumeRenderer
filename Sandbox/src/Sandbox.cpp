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
		float step = 2*M_PI / 120.0;

		for (int i = 0; i < 120; i++)
		{
			ALALBA_INFO("{0}th start", i);

			auto start = std::chrono::system_clock::now();
			
			std::string index = std::to_string(i);
			size_t n = 3;
			int precision = n - std::min(n, index.size());
			index.insert(0, precision, '0');
			
			std::string output = "image/" + index + ".exr";

			
			lux::Matrix camRotateMatrix;
		
			camRotateMatrix = lux::rotation(lux::Vector(0.0, 1.0, 0.0), step);
			lux::Vector eye = camRotateMatrix * m_camera->eye();
			lux::Vector view = lux::Vector(0.0, 0.0, 0.0) - eye;
			m_camera->setEyeViewUp(eye, view, lux::Vector(0, 1, 0));

	
			m_renderer->Render(*m_camera.get(), density_grid, color_grid);
			//m_renderer->Render(*m_camera.get(), bunny_grid, bunny_color_grid);
			m_renderer->SaveImage(output.c_str());


			auto end = std::chrono::system_clock::now();
			double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			
			ALALBA_ERROR("{0} Done, {1}s", output, elapsed);

		}
		
	}

	virtual void OnInit() override
	{

		double rayStepSize = 0.0001;

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

		Alalba::ColorField woodColor;
		woodColor.reset(new Alalba::ConstantColor(lux::Color(86.0 / 255, 47.0 / 255.0, 14.0 / 255.0, 1.0)));


		/// 0. head
		Alalba::ScalarField headSDF;
		Alalba::ScalarField headDensity;
		Alalba::ColorField headColor;

		headSDF = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);
		headSDF = Alalba::Scale<float>(headSDF, lux::Vector(0.25,0.25,0.25));
		headSDF = Alalba::Translate(headSDF, lux::Vector(0.0, 1.5, 0.0));
		
		headDensity = Alalba::Clamp<float>(headSDF, 0.0f, 1.0f);
		
		Alalba::ScalarField mask = Alalba::Mask<float>(headSDF);
		headColor = Alalba::Multiply<lux::Color>(woodColor, mask);

		/// 1. body
		Alalba::ScalarField bodySDF;
		Alalba::ScalarField bodyDensity;
		Alalba::ColorField	bodyColor;
		
		bodySDF = std::make_shared<Alalba::Box>(lux::Vector(0.0, 0.0, 0.0), 1);
		bodySDF = Alalba::Scale<float>(bodySDF, lux::Vector(0.5, 1.0, 0.5));

		bodyDensity = Alalba::Clamp<float>(bodySDF, 0.0f, 1.0f);

		mask = Alalba::Mask<float>(bodySDF);
		bodyColor = Alalba::Multiply<lux::Color>(redColor, mask);


		/// 2. skirt
		Alalba::ScalarField dressSDF;
		Alalba::ScalarField dressDensity;
		Alalba::ColorField	dressColor;

		dressSDF = std::make_shared<Alalba::Cone>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(0.0, 1.0, 0.0), 2, M_PI / 6.0);
		dressSDF = Alalba::Rotate<float>(dressSDF,lux::Vector(0.0, 0.0, 1.0), M_PI);
		// 2.1 remove the intersection between body and skirt
		Alalba::ScalarField intersectionSDF;
		intersectionSDF = Alalba::Intersection<float>(bodySDF, dressSDF);
		dressSDF = Alalba::Cutout<float>(dressSDF, intersectionSDF);
		
		dressDensity = Alalba::Clamp<float>(dressSDF, 0.0f, 1.0f);
		
		mask = Alalba::Mask<float>(dressSDF);
		dressColor = Alalba::Multiply<lux::Color>(yellowColor, mask);
		
		/// 3. bracelet
		Alalba::ScalarField braceletSDF;
		Alalba::ScalarField braceletDensity;
		Alalba::ColorField	braceletColor;

		braceletSDF = std::make_shared<Alalba::Icosahedron>(lux::Vector(0.0, 0.0, 0.0));
		braceletSDF = Alalba::Scale<float>(braceletSDF, lux::Vector(0.08, 0.08, 0.08));
		braceletSDF = Alalba::Translate<float>(braceletSDF, lux::Vector(0.0, 0.0, 1.0));

		braceletDensity = Alalba::Clamp<float>(braceletSDF, 0.0f, 1.0f);

		mask = Alalba::Mask<float>(braceletSDF);
		braceletColor = Alalba::Multiply<lux::Color>(greenColor, mask);


		/// <summary>
		///TODO:  Why can't pass the shared pointer of subclass to a shared pointer of base class 
		/// https://stackoverflow.com/questions/13403490/passing-shared-ptrderived-as-shared-ptrbase
		/// like:
		///  Alalba::ColorField makedCOLOR = Alalba::Multiply(unioncolor, Alalba::Union<float>(s_translate, s_translate2));
		/// but :
		/// Alalba::ColorField makedCOLOR = Alalba::Multiply(unioncolor, std::dynamic_pointer_cast<lux::Volume<float>, Alalba::UnionVolume<float> >( Alalba::Union<float>(s_translate, s_translate2)));
		/// </summary>
		

		humanoid_density_field = Alalba::Union<float>(headDensity, bodyDensity);
		humanoid_density_field = Alalba::Union<float>(humanoid_density_field, dressDensity);
		humanoid_density_field = Alalba::Union<float>(humanoid_density_field, braceletDensity);


		humanoid_color_field = Alalba::Union<lux::Color>(headColor, bodyColor);
		humanoid_color_field = Alalba::Union<lux::Color>(humanoid_color_field, dressColor);
		humanoid_color_field = Alalba::Union<lux::Color>(humanoid_color_field, braceletColor);

		ALALBA_INFO("Grid Humanoid Density Field");
		auto start = std::chrono::system_clock::now();
		density_grid = Alalba::Grid<float>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, humanoid_density_field);
		auto end = std::chrono::system_clock::now();
		double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Grid Humanoid Density Field Done {0}s", elapsed);
		
		ALALBA_INFO("Grid Humanoid Color Field");
		start = std::chrono::system_clock::now();
		color_grid = Alalba::Grid<lux::Color>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(4.0, 4.0, 4.0), { 513,513,513 }, 4, humanoid_color_field);
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Grid Humanoid Color Field Done {0}s", elapsed);


		//// bunny 
		Alalba::Mesh bunny = Alalba::Mesh("model/bunny.obj");		
		ALALBA_INFO(bunny.m_triangles.size());
		ALALBA_INFO("Bunny Dimension:{0}", bunny.dimension);
		ALALBA_INFO("Bunny Center:{0}", bunny.center);
		ALALBA_INFO("Level Set Bunny");

		start = std::chrono::system_clock::now();

		Alalba::ScalarField bunny_grid = Alalba::LevelSet(bunny, bunny.dimension * 1.1, { 513,513,513 }, 8 , 4);
		
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Level Set Bunny Done {0}s", elapsed);

		bunny_grid = Alalba::Scale<float>(bunny_grid, lux::Vector(10.0, 10.0, 10.0));
		bunny_grid = Alalba::Translate<float>(bunny_grid, lux::Vector(-1.5, 0.0, 0.0));
		mask = Alalba::Mask<float>(bunny_grid);

		Alalba::ColorField bunny_color_grid = Alalba::Multiply<lux::Color>(greenColor, mask);

		
		//// Ajax
		Alalba::Mesh ajax = Alalba::Mesh("model/smallajax.obj");

		ALALBA_INFO(ajax.m_triangles.size());
		ALALBA_INFO("Ajax Dimension:{0}", ajax.dimension);
		ALALBA_INFO("Ajax Center:{0}", ajax.center);
		ALALBA_INFO("Level Set Ajax");

		start = std::chrono::system_clock::now();

		Alalba::ScalarField ajax_grid = Alalba::LevelSet(ajax, ajax.dimension * 1.1, { 513,513,513 }, 8, 4);

		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Level Set ajax Done {0}s", elapsed);

		ajax_grid = Alalba::Scale<float>(ajax_grid, lux::Vector(0.05, 0.05, 0.05));;
		ajax_grid = Alalba::Translate<float>(ajax_grid, lux::Vector(1.5, 0.0, 0.0));;
		mask = Alalba::Mask<float>(ajax_grid);

		Alalba::ColorField ajax_color_grid = Alalba::Multiply<lux::Color>(yellowColor, mask);


		density_grid = Alalba::Union<float>(density_grid, bunny_grid);
		density_grid = Alalba::Union<float>(density_grid, ajax_grid);
		density_grid = Alalba::Clamp<float>(density_grid, 0, 0.01);

		color_grid = Alalba::Union<lux::Color>(color_grid, bunny_color_grid);
		color_grid = Alalba::Union<lux::Color>(color_grid, ajax_color_grid);

	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:

	Alalba::ScalarField density_grid;
	Alalba::ColorField color_grid;
	
	Alalba::ScalarField humanoid_density_field;
	Alalba::ColorField humanoid_color_field;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;
};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}