#include "pch.h"
#include "Mesh.h"
#include "tiny_obj_loader.h"
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

    

    LLRC = lux::Vector(minx, miny, minz);
    RUFC = lux::Vector(maxx, maxy, maxz);

    center = (LLRC + RUFC) / 2;
    dimension = lux::Vector(maxx - minx, maxy - miny, maxz - minz);
  }
  void Mesh::ReadOBJ(std::string filepath, std::vector<lux::Vector>& vertices, std::vector<int>& indices)
  {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, reader_config))
    {
      if (!reader.Error().empty())
      {
        std::cerr << "TinyObjReader: " << reader.Error();
      }
      exit(1);
    }
    if (!reader.Warning().empty())
    {
      ALALBA_WARN("TinyObjReader: {0}", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
      tinyobj::real_t vx = attrib.vertices[i + 0];
      tinyobj::real_t vy = attrib.vertices[i + 1];
      tinyobj::real_t vz = attrib.vertices[i + 2];
      lux::Vector v = lux::Vector(vx, vy, vz);
      vertices.push_back(v);
    }

    for (size_t s = 0; s < shapes.size(); s++)
    {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {
        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
        int i0 = shapes[s].mesh.indices[index_offset + 0].vertex_index;
        int i1 = shapes[s].mesh.indices[index_offset + 1].vertex_index;
        int i2 = shapes[s].mesh.indices[index_offset + 2].vertex_index;
        indices.push_back(i0);
        indices.push_back(i1);
        indices.push_back(i2);
        index_offset += fv;
      }
    }

  }
}
