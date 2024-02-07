#include <Alalba.h>
#include "tinyexr.h"
#include "LinearAlgebra.h"
class Sandbox : public Alalba::Application
{

//typedef std::shared_ptr < lux::Volume<lux::Color>> ColorField;
public:
	Sandbox()
	{
		s_sphere = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);
		s_ellipse = std::make_shared<Alalba::Ellipse>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(1.0, 0.0, 0.0), 2, 0.5);
		s_box = std::make_shared<Alalba::Box>(lux::Vector(0.0, 0.0, 0.0), 2);
		s_torus = std::make_shared<Alalba::Torus>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(0.0, 0.0, 1.0), 2, 0.2);
		s_cone = std::make_shared<Alalba::Cone>(lux::Vector(0.0, 0.0, 0.0), lux::Vector(0.0, 1.0, 0.0), 2, M_PI/6.0);
		s_icosahedron = std::make_shared<Alalba::Icosahedron>(lux::Vector(0.0, 0.0, 0.0));
		s_steiner = std::make_shared<Alalba::Steiner>(lux::Vector(0.0, 0.0, 0.0));

	}


	~Sandbox()
	{

	}

	virtual void OnUpdate() override
	{
		float step = 2*M_PI / 120.0;

		for (int i = 0; i < 120; i++)
		{
			ALALBA_ERROR("{0}th start", i);
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

			m_renderer->Render(*m_camera.get(), m_head);
			m_renderer->SaveImage(output.c_str());


			auto end = std::chrono::system_clock::now();
			double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			
			ALALBA_ERROR("{0} Done, {1}s", output, elapsed);

		}
		
	}

	virtual void OnInit() override
	{

		m_camera.reset(new lux::Camera());
		m_camera->setEyeViewUp(lux::Vector(0.0, 0.0, 7.0), lux::Vector(0.0, 0.0, -1.0), lux::Vector(0.0, 1.0, 0.0));
		m_camera->setNearPlane(5.);
		m_camera->setFarPlane(9.);
		

		m_renderer.reset(new Alalba::Renderer(1920, 1080));
		
		Alalba::ColorField redColor;
		redColor.reset(new Alalba::ConstantColor(lux::Color(1.0, 0.0, 0.0, 1.0)));

		Alalba::ColorField greenColor;
		greenColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 1.0, 0.0, 1.0)));

		Alalba::ColorField blueColor;
		blueColor.reset(new Alalba::ConstantColor(lux::Color(0.0, 0.0, 1.0, 1.0)));

		

		s_sphere = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);
		s_sphere2 = std::make_shared<Alalba::Sphere>(lux::Vector(0.0, 0.0, 0.0), 2);
		

		
		
		/// <summary>
		///TODO:  Why can't pass the shared pointer of subclass to a shared pointer of base class 
		/// https://stackoverflow.com/questions/13403490/passing-shared-ptrderived-as-shared-ptrbase
		/// like:
		///  Alalba::ColorField makedCOLOR = Alalba::Multiply(unioncolor, Alalba::Union<float>(s_translate, s_translate2));
		/// but :
		/// Alalba::ColorField makedCOLOR = Alalba::Multiply(unioncolor, std::dynamic_pointer_cast<lux::Volume<float>, Alalba::UnionVolume<float> >( Alalba::Union<float>(s_translate, s_translate2)));
		/// </summary>
	
		
		Alalba::ColorField woodColor;
		woodColor.reset(new Alalba::ConstantColor(lux::Color(86.0/255, 47.0/255.0, 14.0/255.0, 1.0)));
		
		m_bag = Alalba::Translate(Alalba::Object(s_sphere, woodColor), lux::Vector(1.0, 0.0, 0.0));
		m_body = Alalba::Translate(Alalba::Object(s_sphere2, greenColor), lux::Vector(-1.0, 0.0, 0.0));
		
		m_head = Alalba::Union(m_bag, m_body);


		
	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:



	// primitives
	std::shared_ptr<Alalba::Sphere> s_sphere2;
	std::shared_ptr<Alalba::Sphere> s_sphere;
	std::shared_ptr<Alalba::Ellipse> s_ellipse;
	std::shared_ptr<Alalba::Box> s_box;
	std::shared_ptr<Alalba::Torus> s_torus;
	std::shared_ptr<Alalba::Cone> s_cone;
	std::shared_ptr<Alalba::Icosahedron> s_icosahedron;
	std::shared_ptr<Alalba::Steiner> s_steiner;


	Alalba::Object m_boundingbox;
	Alalba::Object m_body;
	Alalba::Object m_head;
	Alalba::Object m_rleg;
	Alalba::Object m_lleg;
	Alalba::Object m_larm;
	Alalba::Object m_rarm;
	Alalba::Object m_bracelet;
	Alalba::Object m_glasses;
	Alalba::Object m_bag;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;
};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}