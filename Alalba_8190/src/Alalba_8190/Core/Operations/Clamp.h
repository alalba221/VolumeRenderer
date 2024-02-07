#include<algorithm>


namespace Alalba
{
	template<typename T>
	class ClampVolume : public lux::Volume<T>
	{
	public:
		ClampVolume(std::shared_ptr< lux::Volume<T> > fieldPtr1, const T& min, const T& max)
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
	class ClampVolume<float> : public lux::Volume<float>
	{
	public:
		ClampVolume(std::shared_ptr< lux::Volume<float> > fieldPtr1, const float& min, const float& max)
			:m_fieldPtr1(fieldPtr1), m_min(min), m_max(max) {};
		~ClampVolume() {};

		virtual const float eval(const lux::Vector& P) const override
		{
			if (m_fieldPtr1->eval(P) <= m_min)
			{
				return m_min;
			}
			else if (m_fieldPtr1->eval(P) >= m_max)
			{
				return m_max;
			}
			else
			{
				return m_fieldPtr1->eval(P);
			}
		};

	private:
		std::shared_ptr< lux::Volume<float> > m_fieldPtr1;
		float m_min;
		float m_max;
	};




	template<typename T>
	std::shared_ptr<ClampVolume<T>> Clamp(std::shared_ptr< lux::Volume<T> > fieldPtr1, const T& min, const T& max)
	{
		return std::make_shared< ClampVolume<T> >(fieldPtr1, min,max);
	}
}
