#pragma once
#include "Volume.h"

namespace Alalba
{
	struct Triangle {
		lux::Vector  p0, p1, p2;
		lux::Vector normal;

		Triangle(lux::Vector a, lux::Vector b, lux::Vector c) 
		:p0(a),p1(b),p2(c)
		{
			lux::Vector e1 = p1 - p0;
			lux::Vector e2 = p2 - p0;
			normal = (e1 ^ e2).unitvector();
		}
	};

	class Mesh
	{
	public:

		Mesh(std::string filepath);
		~Mesh() {};

		const std::vector<Triangle>& Triangles() const { return m_triangles; }

		lux::Vector LLRC;
		lux::Vector RUFC;
		lux::Vector center;
		lux::Vector dimension;

	private:
		void ReadOBJ(std::string filepath, std::vector<lux::Vector>& vertices, std::vector<int>& indices);
	public:
		std::vector<Triangle> m_triangles;

	};
}


