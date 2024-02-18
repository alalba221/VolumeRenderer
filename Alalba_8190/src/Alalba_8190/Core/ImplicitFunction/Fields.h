#pragma once

#include "Volume.h"
namespace Alalba
{
	typedef std::shared_ptr<lux::Volume<float>> ScalarField;
	typedef std::shared_ptr < lux::Volume<lux::Color>> ColorField;



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

}

