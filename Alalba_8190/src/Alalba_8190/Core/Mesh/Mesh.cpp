#include "pch.h"
#include "Mesh.h"
namespace Alalba
{

  Mesh::Mesh(std::string filepath)
  {
    std::vector<lux::Vector> vertices;     // 顶点坐标
    std::vector<int> indices;    // 顶点索引

    ReadOBJ(filepath, vertices, indices);

    for (int i = 0; i < indices.size(); i += 3) {
      m_triangles.push_back(Triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]));
    }

    // find aabb and center
    double minx=1000, miny=1000, minz=1000, maxx = -1000, maxy=-1000, maxz=-1000, sumx=0, sumy=0, sumz=0;

//#pragma omp parallel for
    for (auto vertex : vertices)
    {
      double x = vertex.X();
      double y = vertex.Y();
      double z = vertex.Z();
      
      minx = (x < minx) ? x : minx;
      miny = (y < miny) ? y : miny;
      minz = (z < minz) ? z : minz;
      maxx = (x > maxx) ? x : maxx;
      maxy = (y > maxy) ? y : maxy;
      maxz = (z > maxz) ? z : maxz;
      
      sumx += x;
      sumy += y;
      sumz += z;
    }
    int totalvertex = vertices.size();

    LLFC = lux::Vector(minx, miny, maxz);
    RURC = lux::Vector(maxx, maxy, minz);

    center = lux::Vector(sumx, sumy, sumz);
    center /= totalvertex;

    dimension = lux::Vector(maxx - minx, maxy - miny, maxz - minz);
  }

  void Mesh::ReadOBJ(std::string filepath, std::vector<lux::Vector>& vertices, std::vector<int>& indices)
	{
    // 打开文件流
    std::ifstream fin(filepath);
    std::string line;
    if (!fin.is_open()) {
      std::cout << "File " << filepath << " Open failed" << std::endl;
      exit(-1);
    }

    // 增量读取
    int offset = vertices.size();

    // 按行读取
    while (std::getline(fin, line)) {
      std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
      std::string type;
      double x, y, z;
      int v0, v1, v2;

      // 读取obj文件
      sin >> type;
      if (type == "v") {
        sin >> x >> y >> z;
        vertices.push_back(lux::Vector(x, y, z));
      }
      if (type == "f") {
        sin >> v0 >> v1 >> v2;
        indices.push_back(v0 - 1 + offset);
        indices.push_back(v1 - 1 + offset);
        indices.push_back(v2 - 1 + offset);
      }
    }
	}
}
