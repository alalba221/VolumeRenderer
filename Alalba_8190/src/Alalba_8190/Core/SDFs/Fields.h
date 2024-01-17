#pragma once

#include "Volume.h"
namespace Alalba
{

	class DensityField : public lux::Volume<float>
	{
	public:
		DensityField(const std::shared_ptr<lux::Volume<float>>& sdf)
			:m_sdf(sdf) {};
		~DensityField() {};
		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };
	private:
		std::shared_ptr<lux::Volume<float>> m_sdf;
	};


	class ColorField : public lux::Volume<lux::Color>
	{
	public:
		ColorField(const std::shared_ptr<lux::Volume<float>>& sdf, const lux::Color& color)
			:m_sdf(sdf), m_baseColor(color) {};
		~ColorField() {};
		virtual const lux::Color eval(const lux::Vector& P) const override;
		//virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };
	private:
		std::shared_ptr<lux::Volume<float>> m_sdf;
		lux::Color m_baseColor;
	};


	class Sphere : public lux::Volume<float>
	{
	public:
		Sphere(const lux::Vector&center, float r) :m_center(center),m_radius(r) {};
		~Sphere() {};

		virtual const float eval(const lux::Vector& P) const override ;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Sphere& Get() const { return *this; };
	private:
		float m_radius;
		lux::Vector m_center;
	};


	class Ellipse : public lux::Volume<float>
	{
	public:
		Ellipse(const lux::Vector& center,
			const lux::Vector& normal,
			float major, float minor) :m_center(center), m_normal(normal), m_radiusMajor(major), m_radiusMinor(minor) {};
		~Ellipse() {};

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Ellipse& Get() const { return *this; };
	private:
		
		float m_radiusMajor;
		float m_radiusMinor;
		lux::Vector m_center;
		lux::Vector m_normal;
	};


	class Box : public lux::Volume<float>
	{
	public:
		Box(const lux::Vector& center,
			float R) :m_center(center), m_radius(R) {};
		~Box() {};
	
		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };
	
		const Box& Get() const { return *this; };
	private:
	
		float m_radius;
		lux::Vector m_center;
	};

	class Torus : public lux::Volume<float>
	{
	public:
		Torus(const lux::Vector& center,
			const lux::Vector& normal,
			float major, float minor) :m_center(center), m_normal(normal), m_radiusMajor(major), m_radiusMinor(minor) {};
		~Torus() {};

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Torus& Get() const { return *this; };
	private:

		float m_radiusMajor;
		float m_radiusMinor;
		lux::Vector m_center;
		lux::Vector m_normal;
	};

	class Cone :public lux::Volume<float>
	{
	public:
		Cone(const lux::Vector& center,
			const lux::Vector& normal,
			float height, float rad) :m_center(center), m_normal(normal), m_height(height), m_rad(rad) {};
		~Cone() {};

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Cone& Get() const { return *this; };
	private:

		float m_height;
		float m_rad;
		lux::Vector m_center;
		lux::Vector m_normal;
	};

	class Icosahedron :public lux::Volume<float>
	{
	public:
		Icosahedron(const lux::Vector& center) 
			:m_center(center){};
		~Icosahedron() {};

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Icosahedron& Get() const { return *this; };
	private:

		lux::Vector m_center;
	};


	class Steiner :public lux::Volume<float>
	{
	public:
		Steiner(const lux::Vector& center)
			:m_center(center) {};
		~Steiner() {};

		virtual const float eval(const lux::Vector& P) const override;
		virtual const lux::Vector grad(const lux::Vector& P) const override { return lux::Vector(); };

		const Steiner& Get() const { return *this; };
	private:

		lux::Vector m_center;
	};
}

