#include "pch.h"
#include "Object.h"
#include "Alalba_8190/Core/Transform/Operations.h"
#include "Alalba_8190/Core/Transform/Transform.h"
#include "Alalba_8190/Core/SDFs/Fields.h"
namespace Alalba
{
	Object::Object(const std::shared_ptr<lux::Volume<float>>& sdf, lux::Color color)
		:m_sdf(sdf), m_baseColor(color)
	{
		m_densityField.reset( new DensityField(m_sdf));
		m_colorField.reset(new ColorField(m_sdf, m_baseColor));
	}

	Object& Object::Union(const Object& obj)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset( new SFUnion(m_sdfHistory, obj.m_sdf));

		// density 
		m_densityField.reset(new DensityField(m_sdf));
		
		// color 
		m_colorHistory = m_colorField;
		m_colorField.reset(new CFUnion(m_colorHistory, obj.m_colorField));

		return *this;
	}

	Object& Object::Intersection(const Object& obj)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset(new SFIntersection(m_sdfHistory, obj.m_sdf));

		// density 
		m_densityField.reset(new DensityField(m_sdf));

		// color 
		m_colorHistory = m_colorField;
		m_colorField.reset(new CFIntersection(m_colorHistory, obj.m_colorField));
		return *this;
	}

	Object& Object::CutOut(const Object& obj)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset(new SFCutOut(m_sdfHistory, obj.m_sdf));

		// density 
		m_densityField.reset(new DensityField(m_sdf));

		// color 
		m_colorHistory = m_colorField;
		/// TODO: decide which one to use
		//m_colorField.reset(new CFCutOut(m_colorHistory, obj.m_colorField));
		m_colorField.reset(new ColorField(m_sdf, m_baseColor));
		return *this;
	}

	Object& Object::Translate(const lux::Vector& dX)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset(new SFTranslate(m_sdfHistory, dX));

		// color
		//m_colorHistory = m_colorField;
		//m_colorField.reset(new CFTranslate(m_colorHistory, dX));
		m_colorField.reset(new ColorField(m_sdf, m_baseColor));
		// density
		m_densityField.reset(new DensityField(m_sdf));
		return *this;
	}
	Object& Object::Rotate(const lux::Vector& axis, float rad)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset(new SFRotation(m_sdfHistory, axis ,rad));

		// color
		//m_colorHistory = m_colorField;
		//m_colorField.reset(new CFRotation(m_colorHistory, axis, rad));
		m_colorField.reset(new ColorField(m_sdf, m_baseColor));

		// density
		m_densityField.reset(new DensityField(m_sdf));
		return *this;
	}
	Object& Object::Scale(const lux::Vector& scale)
	{
		// sdf
		m_sdfHistory = m_sdf;
		m_sdf.reset(new SFScale(m_sdfHistory, scale));

		// color
		//m_colorHistory = m_colorField;
		//m_colorField.reset(new CFScale(m_colorHistory, scale));
		m_colorField.reset(new ColorField(m_sdf, m_baseColor));
		// density
		m_densityField.reset(new DensityField(m_sdf));
		return *this;
	}
}