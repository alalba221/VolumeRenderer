#include<algorithm>
#include "Volume.h"
#include "Fields.h"
namespace Alalba
{
	template<typename T>
	class MaskVolume : public lux::Volume<T>
	{
	public:
		MaskVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
			:m_fieldPtr1(fieldPtr1) {};
		~MaskVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T; };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;

	};

	/// Scalar
	template<>
	virtual const float MaskVolume<float> ::eval(const lux::Vector& P) const
	{
		return m_fieldPtr1->eval(P) > 0.0 ? 1 : 0;
	};
	

	template<typename T>
	std::shared_ptr<MaskVolume<T>> Mask(const std::shared_ptr< lux::Volume<T> >& fieldPtr1)
	{
		ALALBA_INFO("MASK VOLUME");
		return std::make_shared< MaskVolume<T> >(fieldPtr1);
	}
}
