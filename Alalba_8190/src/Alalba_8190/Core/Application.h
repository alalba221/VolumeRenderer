#pragma once
#include <memory>

namespace Alalba {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnInit();
		virtual void OnUpdate() {}
		virtual void OnShutdown();


		static inline Application& Get() { return *s_Instance; }
	

	private:

	private:

		bool m_Running = true;

		static Application* s_Instance;
	protected:

	};

	// to be defined in sandbox
	Application* CreateApplication();
}


