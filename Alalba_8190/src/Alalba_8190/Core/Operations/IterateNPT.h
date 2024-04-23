#pragma once
#include "Volume.h"
#include "Fields.h"
#include "Warp.h"
#include "Alalba_8190/Core/ImplicitFunction/Fields.h"
#include "Multiply.h"
#include "Divide.h"
#include "Subtract.h"
#include "Add.h"
#include "Gradient.h"
namespace Alalba
{
	template<int N = 5>
	class NPTVolume :public lux::Volume < lux::Vector >
	{
	public:
		NPTVolume(const std::shared_ptr< lux::Volume<float> >& IF)
			:m_IF(IF)
		{
			m_I.reset(new IdentityVectorField());

			NPT.reset(new IdentityVectorField());

			gradient = GradientVector(m_IF);

			Numerator = Multiply < lux::Vector>(gradient, m_IF);

			Denominator = Multiply< lux::Vector, lux::Vector>(gradient, gradient);

			secondTerm = Divide<lux::Vector, float>(Numerator, Denominator);
			NPT = Subtract <lux::Vector>(NPT, secondTerm);

			result.reset(new IdentityVectorField());

			for (int i = 0; i < N; i++)
			{
				result = Warp(result, NPT);
			}
		}
		~NPTVolume() {};

		virtual const volumeDataType eval(const lux::Vector& P) const override
		{
			
			return result->eval(P);

		};
	private:
		std::shared_ptr< lux::Volume<float> > m_IF;
		IdentityVectorFieldPtr m_I;
		VectorField NPT;

		VectorField gradient;

		VectorField Numerator;
		ScalarField Denominator;
		VectorField secondTerm;

		VectorField result;
	};

	template<int N = 5>
	std::shared_ptr<lux::Volume<float>> NPT(const std::shared_ptr< lux::Volume<float> >& IF)
	{
		return std::make_shared< NPTVolume<N> >(IF);
	}

}