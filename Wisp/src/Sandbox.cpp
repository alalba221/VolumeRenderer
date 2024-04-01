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
		for (int i = 0; i < 500; i++)
		{
			ALALBA_INFO("{0}th start", i);

			auto start = std::chrono::system_clock::now();

			std::string index = std::to_string(i);
			size_t n = 3;
			int precision = n - std::min(n, index.size());
			index.insert(0, precision, '0');

			std::string output = "image/" + index + ".exr";


			//lux::Matrix camRotateMatrix;

			//camRotateMatrix = lux::rotation(lux::Vector(0.0, 1.0, 0.0), step);
			//lux::Vector eye = camRotateMatrix * m_camera->eye();
			//lux::Vector view = lux::Vector(0.0, 0.0, 0.0) - eye;
			//m_camera->setEyeViewUp(eye, view, lux::Vector(0, 1, 0));


			//////////////////////////////////
					// NOISE
			Alalba::Noise_t fspn1_parm;
			fspn1_parm.frequency = 2.5;
			fspn1_parm.octaves = 1;
			fspn1_parm.roughness = 0.8;
			fspn1_parm.translate = lux::Vector(0.0, 0.0, 0.0);

			Alalba::Noise_t fspn2_parm;
			fspn2_parm.frequency = 2.5 + i*0.1;
			fspn2_parm.octaves = 2;
			fspn2_parm.roughness = 0.8;
			fspn2_parm.translate = lux::Vector(0.0, 0.0, 0.0);

			Alalba::WispPara wisp_para;
			wisp_para.fspn1 = fspn1_parm;
			wisp_para.fspn2 = fspn2_parm;
			wisp_para.numchilderen = 5000000;
			wisp_para.den = 1.0;
			wisp_para.clump = 0.3;
			wisp_para.dP = lux::Vector(0.1, 0.1, 0.1);
			wisp_para.Plocal = lux::Vector(0.0, 0.0, 0.0);
			wisp_para.slocal = 1.5;

			Alalba::GridPara gridpara;
			gridpara.center = lux::Vector(0.0, 0.0, 0.0);
			gridpara.dimesion = lux::Vector(4.0, 4.0, 4.0);
			gridpara.resolution = Alalba::INT3(513, 513, 513);
			gridpara.partionSize = 4;

			ALALBA_INFO(" Wisping");
			start = std::chrono::system_clock::now();
			wisp = Alalba::Wisp<float>(gridpara, wisp_para);
			auto end = std::chrono::system_clock::now();
			double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			ALALBA_TRACE("Wisping Done {0}s", elapsed);

			Alalba::ScalarField mask = Alalba::Mask<float>(wisp);

			wispColor = Alalba::Multiply<lux::Color>(woodColor, mask);

			// lights
			lux::Vector keyPos = lux::Vector(0, 4, 0);
			m_key.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 10.0, keyPos));

			lux::Vector fillPos = lux::Vector(0, -4, 0);
			m_fill.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 2.0, fillPos));

			lux::Vector rimPos = lux::Vector(0, 0, -4);
			m_rim.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 4.0, rimPos));

			ALALBA_TRACE("Key dsm");
			m_key->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
			ALALBA_TRACE("Fill dsm");
			m_fill->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
			ALALBA_TRACE("Rim dsm");
			m_rim->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);


			/////////////////////////////////////////


			m_renderer->Render(*m_camera.get(), wisp, wispColor, *m_key, *m_fill, *m_rim);
			//m_renderer->Render(*m_camera.get(), bunny_grid, bunny_color_grid);
			m_renderer->SaveImage(output.c_str());


			end = std::chrono::system_clock::now();
			elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

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


		/// 
		// NOISE
		Alalba::Noise_t fspn1_parm;
		fspn1_parm.frequency = 2.5;
		fspn1_parm.octaves = 1;
		fspn1_parm.roughness = 0.8;
		fspn1_parm.translate = lux::Vector(0.0, 0.0, 0.0);

		Alalba::Noise_t fspn2_parm;
		fspn2_parm.frequency = 0.5;
		fspn2_parm.octaves = 2;
		fspn2_parm.roughness = 0.8;
		fspn2_parm.translate = lux::Vector(0.0, 0.0, 0.0);

		Alalba::WispPara wisp_para;
		wisp_para.fspn1 = fspn1_parm;
		wisp_para.fspn2 = fspn2_parm;
		wisp_para.numchilderen = 5000000;
		wisp_para.den = 1.0;
		wisp_para.clump = 0.3;
		wisp_para.dP = lux::Vector(0.1, 0.1, 0.1);
		wisp_para.Plocal = lux::Vector(0.0, 0.0, 0.0);
		wisp_para.slocal = 1.5;

		Alalba::GridPara gridpara;
		gridpara.center = lux::Vector(0.0, 0.0, 0.0);
		gridpara.dimesion = lux::Vector(4.0, 4.0, 4.0);
		gridpara.resolution = Alalba::INT3(513, 513, 513);
		gridpara.partionSize = 4;
		
		ALALBA_INFO(" Wisping");
		auto start = std::chrono::system_clock::now();
		wisp = Alalba::Wisp<float>(gridpara, wisp_para);
		auto end = std::chrono::system_clock::now();
		double  elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		ALALBA_TRACE("Wisping Done {0}s", elapsed);

		Alalba::ScalarField mask = Alalba::Mask<float>(wisp);

		wispColor = Alalba::Multiply<lux::Color>(woodColor, mask);

		// lights
		lux::Vector keyPos = lux::Vector(0, 4, 0);
		m_key.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 10.0, keyPos));

		lux::Vector fillPos = lux::Vector(0, -4, 0);
		m_fill.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 2.0, fillPos));

		lux::Vector rimPos = lux::Vector(0, 0, -4);
		m_rim.reset(new Alalba::PointLight(lux::Color(1.0, 1.0, 1.0, 1.0) * 4.0, rimPos));

		ALALBA_TRACE("Key dsm");
		m_key->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
		ALALBA_TRACE("Fill dsm");
		m_fill->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
		ALALBA_TRACE("Rim dsm");
		m_rim->GenerateDSM(wisp, lux::Vector(0, 0, 0), lux::Vector(4, 4, 4), { 200,200,200 }, 8, 0.1, 2.0);
		
	}

	virtual void OnShutdown() override
	{

		Alalba::Application::OnShutdown();

	}

private:

	// test noise
	Alalba::ColorField woodColor;

	Alalba::ScalarField wisp;
	Alalba::ScalarField headDensity;
	Alalba::ColorField wispColor;
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