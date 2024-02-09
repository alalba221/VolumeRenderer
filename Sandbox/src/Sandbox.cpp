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

			m_renderer->Render(*m_camera.get(), density_field,color_field);
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
		
		ScalarField mask = Alalba::Mask<float>(headSDF);
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
		

		density_field = Alalba::Union<float>(headDensity, bodyDensity);
		density_field = Alalba::Union<float>(density_field, dressDensity);
		density_field = Alalba::Union<float>(density_field, braceletDensity);


		color_field = Alalba::Union<lux::Color>(headColor, bodyColor);
		color_field = Alalba::Union<lux::Color>(color_field, dressColor);
		color_field = Alalba::Union<lux::Color>(color_field, braceletColor);
		
	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:



	// primitives
	/*std::shared_ptr<Alalba::Sphere> s_sphere2;
	std::shared_ptr<Alalba::Sphere> s_sphere;
	std::shared_ptr<Alalba::Ellipse> s_ellipse;
	std::shared_ptr<Alalba::Box> s_box;
	std::shared_ptr<Alalba::Torus> s_torus;
	std::shared_ptr<Alalba::Cone> s_cone;
	std::shared_ptr<Alalba::Icosahedron> s_icosahedron;
	std::shared_ptr<Alalba::Steiner> s_steiner;*/


	Alalba::ScalarField density_field;
	Alalba::ColorField color_field;

	std::unique_ptr<lux::Camera> m_camera;
	std::unique_ptr<Alalba::Renderer> m_renderer;
};


Alalba::Application* Alalba::CreateApplication()
{
	return new Sandbox();
}