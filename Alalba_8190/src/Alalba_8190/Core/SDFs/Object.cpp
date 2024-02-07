#include "pch.h"
#include "Object.h"

#include "Alalba_8190/Core/Operations/Operations.h"
namespace Alalba
{
	Object::Object(const ScalarField& sdf, const ColorField& color_field)
		:m_sdf(sdf), m_colorField(color_field)
	{
		m_density = Alalba::Clamp<float>(m_sdf, 0.0f, 1.0f);
	}


	Object Translate(const Object& obj, const lux::Vector& dx)
	{
		ScalarField new_sdf = Alalba::Translate<float>(obj.SDF(),dx);
		
		ColorField new_color = Alalba::Translate<lux::Color>(obj.ColorFiled(), dx);

		ScalarField mask = Alalba::Mask<float>(new_sdf);

		new_color = Alalba::Multiply<lux::Color>(new_color, mask);

		return Object(new_sdf, new_color);

	}

	Object Union(const Object& obj1, const Object& obj2)
	{
		ScalarField new_sdf = Alalba::Union<float>(obj1.SDF(), obj2.SDF());

		ColorField new_color = Alalba::Union<lux::Color>(obj1.ColorFiled(), obj2.ColorFiled());

		ScalarField mask = Alalba::Mask<float>(new_sdf);

		new_color = Alalba::Multiply<lux::Color>(new_color, mask);

		return Object(new_sdf, new_color);

	}
	
}