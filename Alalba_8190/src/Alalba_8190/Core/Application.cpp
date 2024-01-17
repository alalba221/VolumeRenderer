#include "pch.h"
#include "Application.h"

//

namespace Alalba
{
#define BIND_ENVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		s_Instance = this;

	}

	void Application::OnInit()
	{

	}

	void Application::OnShutdown()
	{
		
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Load model in OnInit();
		OnInit();

		//while (m_Running)
		//{
			OnUpdate();
	//	}
		OnShutdown();
	}

}

