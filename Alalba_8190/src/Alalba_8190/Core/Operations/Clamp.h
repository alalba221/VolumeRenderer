#include<algorithm>


namespace Alalba
{
	template<typename T>
	class ClampVolume : public lux::Volume<T>
	{
	public:
		ClampVolume(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const T& min, const T& max)
			:m_fieldPtr1(fieldPtr1),m_min(min),m_max(max) {};
		~ClampVolume() {};

		virtual const T eval(const lux::Vector& P) const override { return T; };

	private:

		std::shared_ptr< lux::Volume<T> > m_fieldPtr1;
		T m_min;
		T m_max;

	};

	/// Scalar
	template<>
	virtual const float ClampVolume<float>::eval(const lux::Vector& P) const override
	{
		float value = m_fieldPtr1->eval(P);
		
		if (value <= m_min)
		{
			return m_min;
		}
		else if (value >= m_max)
		{
			return m_max;
		}
		else
		{
			return value;
		}
	};

	
	template<typename T>
	std::shared_ptr<ClampVolume<T>> Clamp(const std::shared_ptr< lux::Volume<T> >& fieldPtr1, const T& min, const T& max)
	{
		return std::make_shared< ClampVolume<T> >(fieldPtr1, min,max);
	}
}
