#pragma once
#include"Volume.h"

#include "Fields.h"

typedef std::shared_ptr<lux::Volume<float>> ScalarField;
typedef std::shared_ptr < lux::Volume<lux::Color>> ColorField;

namespace Alalba
{
	class Object
	{
	public:
		Object() {};
		
		Object(const ScalarField& sdf, const ColorField& color_field);
		
		~Object() 
		{
			
		};
		
		
		const ScalarField& SDF() const { return m_sdf; }
		const ColorField& ColorFiled() const { return m_colorField; }
		const ScalarField& Density() const { return m_density; }

	protected:
		ScalarField m_sdf;
		ColorField m_colorField;

		ScalarField m_density;

	};

	Object Union(const Object& obj1, const Object& obj2);
	Object Translate(const Object& obj, const lux::Vector& dx);
}
