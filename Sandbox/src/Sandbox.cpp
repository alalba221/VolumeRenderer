#include <Alalba.h>
#include "tinyexr.h"
#include "LinearAlgebra.h"
class Sandbox : public Alalba::Application
{
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

			
			lux::Matrix camRotate;
			camRotate = lux::rotation(lux::Vector(0.0, 1.0, 0.0), i*step);
			lux::Vector eye = camRotate * m_camera->eye();
			lux::Vector view = camRotate * m_camera->view();
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
		

		m_renderer.reset(new Alalba::Renderer(160, 90));
		
		

		////// design
		lux::Color boundingboxColor = lux::Color(.3, .3, .3, 0.0);
		m_boundingbox = Alalba::Object(s_box, boundingboxColor);
		
		// -6
		/// head
		lux::Color headColor = lux::Color(0.5, 0.5, 0., 0.0);
		m_head = Alalba::Object(s_sphere, headColor);
		m_head.Scale(lux::Vector(0.25, 0.25, 0.25)).Translate(lux::Vector(0.0, 1.5, 0.0));
	
		
		//m_boundingbox.Union(m_head);
		
		/// body
		lux::Color bodyColor = lux::Color(1., 0., 0., 0.0);
		m_body = Alalba::Object(s_box, bodyColor);
		m_body.Scale(lux::Vector(0.25,0.5,0.25));

		// skirt
		lux::Color skirtColor = lux::Color(0., 1., 0., 0.0);
		Alalba::Object skirt = Alalba::Object(s_cone, skirtColor);
		skirt.Scale(lux::Vector(1., 0.5, 1.)).Rotate(lux::Vector(0.0,0.0,1.0),M_PI);
	
		
		// //
		
		
		/// legs
		m_lleg = Alalba::Object(s_ellipse, headColor);
		m_lleg.Scale(lux::Vector(0.5, 0.5, 0.5));
		m_lleg.Rotate(lux::Vector(0.,0.,1.),M_PI*0.4).Translate(lux::Vector(-0.4, -1.4, 0.));

		m_rleg = Alalba::Object(s_ellipse, headColor);
		m_rleg.Scale(lux::Vector(0.5, 0.5, 0.5));
		m_rleg.Rotate(lux::Vector(0., 0., 1.), M_PI * (1-0.4)).Translate(lux::Vector(0.4, -1.4, 0.));

		
		///  arms
		m_larm = Alalba::Object(s_ellipse, headColor);
		m_larm.Scale(lux::Vector(0.3, 0.3, 0.3));
		m_larm.Translate(lux::Vector(-1.1, 0.5, 0.));

		m_rarm = Alalba::Object(s_ellipse, headColor);
		m_rarm.Scale(lux::Vector(0.3, 0.3, 0.3));
		m_rarm.Translate(lux::Vector(1.1, 0.5, 0.));
		// //
		
		
		///bracelet
		lux::Color otherColor = lux::Color(0., 0., 10., 0.0);
		m_bracelet = Alalba::Object(s_icosahedron, otherColor);
		m_bracelet.Scale(lux::Vector(0.05, 0.05, 0.05)).Translate(lux::Vector(0.0,0.0,0.6));

		/// glasses
		lux::Color glassColor = lux::Color(1., 1., 0., 0.0);
		m_glasses = Alalba::Object(s_torus, glassColor);
		m_glasses.Scale(lux::Vector(0.125, 0.125, 1.)).Translate(lux::Vector(0.2, 1.5, 0.0));
		
		Alalba::Object sphere1 = Alalba::Object(s_sphere, glassColor);
		sphere1.Scale(lux::Vector(0.125, 0.125, 0.125)).Translate(lux::Vector(0.2, 1.5, 0.0));

		Alalba::Object otherglass = Alalba::Object(s_torus, glassColor);
		otherglass.Scale(lux::Vector(0.125, 0.125, 1.)).Translate(lux::Vector (- 0.2, 1.5, 0.0));

		Alalba::Object sphere2 = Alalba::Object(s_sphere, glassColor);
		sphere2.Scale(lux::Vector(0.125, 0.125, 0.125)).Translate(lux::Vector(- 0.2, 1.5, 0.0));

		m_glasses.Union(otherglass);
		sphere1.Intersection(otherglass);
		sphere2.Intersection(m_glasses);
		sphere1.Union(sphere2);

		m_glasses.CutOut(sphere1).Translate(lux::Vector(0.0, 0.0, 0.6));

		/// m_bag 
		m_bag = Alalba::Object(s_steiner, otherColor);
		m_bag.Scale(lux::Vector(0.5, 0.5, 0.5)).Translate(lux::Vector(-0.6,0.0,0.0));
		
		// compose
		m_body.CutOut(skirt).Union(skirt);
		m_body.Union(m_lleg).Union(m_rleg).Union(m_rarm).Union(m_larm);
		m_head.Union(m_glasses).Union(m_body).Union(m_bracelet).Union(m_bag);

	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:
	// primitives
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
	//Alalba::Object m_sphere2;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;
};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}