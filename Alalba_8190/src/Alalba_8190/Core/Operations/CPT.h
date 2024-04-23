#pragma once
#include "Volume.h"
#include "Fields.h"
#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
namespace Alalba
{
	class CPTVolume:public lux::Volume<lux::Vector>
	{
	public:
		CPTVolume(const std::shared_ptr< lux::Volume<float> >& sdf)
			:m_sdf(sdf) 
		{
			m_I = IdentityVectorField();
		};
		~CPTVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override 
		{ 
			return m_I.eval(P) - m_sdf->eval(P) * m_sdf->grad(P);
			
		};

	private:

		std::shared_ptr< lux::Volume<float> > m_sdf;
		IdentityVectorField m_I;
	};

	std::shared_ptr<lux::Volume<lux::Vector>> CPT(const std::shared_ptr< lux::Volume<float> >& sdf);
}
