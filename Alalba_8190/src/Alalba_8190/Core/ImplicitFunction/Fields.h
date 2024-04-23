#pragma once

#include "Volume.h"
namespace Alalba
{

	class IdentityVectorField :public lux::Volume<lux::Vector>
	{
	public:
		IdentityVectorField(){}

		virtual const lux::Vector eval(const lux::Vector& P)  const override
		{
			lux::Vector value = P;
			return value;
		}

		virtual const lux::Matrix grad(const lux::Vector& P)const override
		{
			return lux::Matrix(	1, 0, 0, 
													0, 1, 0, 
													0, 0, 1
												);
		}
	private:

	};

	class ConstantColor : public lux::Volume<lux::Color>
	{
	public:

		ConstantColor(const lux::Color& v) :
			value(v)
		{}

		~ConstantColor() {}


		virtual const lux::Color eval(const lux::Vector& P)  const override
		{
			return value;
		}


		virtual std::string typelabel()
		{
			std::string lbl = "Constant";
			return lbl;
		}


	private:

		lux::Color value;
	};


	typedef std::shared_ptr<lux::Volume<float>> ScalarField;
	typedef std::shared_ptr < lux::Volume<lux::Color>> ColorField;

	typedef std::shared_ptr < lux::Volume<lux::Vector>> VectorField;
	typedef std::shared_ptr < IdentityVectorField> IdentityVectorFieldPtr;
}

