#include "mesh.h"
#include "tglconfig.h"
#include <GL/glew.h>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

//static Mesh* createCube(vertex_type vt);

//Mesh* Mesh::CreateMesh(mesh_type mt, vertex_type vt)
//{
//  switch(mt) {
//  case AXIS:
//    return createCube(vt);
//    break;
//  case QUAD:
//    return createCube(vt);
//    break;
//  case CUBE:
//    return createCube(vt);
//    break;
//  case SPHERE:
//    return createCube(vt);
//    break;
//  case GRID:
//  default:
//    return createCube(vt);
//    break;
//  }
//}

Mesh::Mesh()
{
  glGenVertexArrays(1, &m_vao);
}

Mesh::~Mesh()
{
  for (unsigned int i = 0; i < m_buffers.size(); i++)
    delete m_buffers[i];
  m_buffers.clear();

  glDeleteVertexArrays(1, &m_vao);
}

void Mesh::attachBuffer(BufferObject* buff)
{
  m_buffers.push_back(buff);
}

void Mesh::draw()
{
  glBindVertexArray(m_vao);
  m_drawCb(m_numPoints);
  glBindVertexArray(0);
}

//Mesh* createCube(vertex_type vt)
//{
//  switch(vt) {
//  default:
//    break;
//  }

//  return NULL;
//}
