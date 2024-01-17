#pragma once
#include"Volume.h"
#include"Alalba_8190/Core/Transform/Transform.h"
namespace Alalba
{
	class Object
	{
	public:
		Object() {};
		Object(const std::shared_ptr<lux::Volume<float>>& sdf, lux::Color color);
		
		~Object() 
		{
			
		};
		
		const float sdfEval(const lux::Vector& p) const { return m_sdf->eval(p); };
		const float densityEval(const lux::Vector& p) const { return m_densityField->eval(p); };
		const lux::Color dcolorEval(const lux::Vector& p) const { return m_colorField->eval(p); };

		Object& Union(const Object& obj);
		Object& Intersection(const Object& obj);
		Object& CutOut(const Object& obj);
		
		Object& Translate(const lux::Vector& dX);
		Object& Rotate(const lux::Vector& axis, float rad);
		Object& Scale(const lux::Vector& scale);

		const lux::Volume<float>& SDF() const { return *m_sdf; }
		const lux::Volume<float>& Density() const{ return *m_densityField; }
		const lux::Volume<lux::Color>& Color() const { return *m_colorField; }

	protected:
		std::shared_ptr<lux::Volume<float>> m_sdf;
		std::shared_ptr<lux::Volume<float>> m_densityField;
		std::shared_ptr<lux::Volume<lux::Color> > m_colorField;

		lux::Color m_baseColor;

		std::shared_ptr<lux::Volume<float>> m_sdfHistory;
		std::shared_ptr<lux::Volume<float>> m_densityHistory;
		std::shared_ptr<lux::Volume<lux::Color>> m_colorHistory;
	};
}
