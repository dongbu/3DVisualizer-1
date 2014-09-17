#include "cube.h"

Cube::Cube(void)
{
  //GLfloat vertices[] = {
  //  //BACK
  //  -0.5, -0.5, -0.5,
  //   0.5, -0.5, -0.5,
  //   0.5,  0.5, -0.5,
  //  -0.5, -0.5, -0.5,
  //   0.5,  0.5, -0.5,
  //  -0.5,  0.5, -0.5,
  //  //FRONT
  //  -0.5, -0.5, 0.5,
  //   0.5, -0.5, 0.5,
  //   0.5,  0.5, 0.5,
  //  -0.5, -0.5, 0.5,
  //   0.5,  0.5, 0.5,
  //  -0.5,  0.5, 0.5,
  //  //RIGHT
  //   0.5, -0.5,  0.5,
  //   0.5, -0.5, -0.5,
  //   0.5,  0.5, -0.5,
  //   0.5, -0.5,  0.5,
  //   0.5,  0.5, -0.5,
  //   0.5,  0.5,  0.5,
  //   //LEFT
  //   -0.5, -0.5, -0.5,
  //   -0.5, -0.5,  0.5,
  //   -0.5,  0.5,  0.5,
  //   -0.5, -0.5, -0.5,
  //   -0.5,  0.5,  0.5,
  //   -0.5,  0.5, -0.5,
  //   //TOP
  //   -0.5,  0.5,  0.5,
  //    0.5,  0.5,  0.5,
  //    0.5,  0.5, -0.5,
  //   -0.5,  0.5,  0.5,
  //    0.5,  0.5, -0.5,
  //   -0.5,  0.5, -0.5,
  //   //BOTTOM
  //   -0.5, -0.5,  0.5,
  //    0.5, -0.5,  0.5,
  //    0.5, -0.5, -0.5,
  //   -0.5, -0.5,  0.5,
  //    0.5, -0.5, -0.5,
  //   -0.5, -0.5, -0.5
  //};

  //GLfloat normals[] = {
  //  //BACK
  //  0, 0, -1,
  //  0, 0, -1,
  //  0, 0, -1,
  //  0, 0, -1,
  //  0, 0, -1,
  //  0, 0, -1,
  //  //FRONT
  //  0, 0, 1,
  //  0, 0, 1,
  //  0, 0, 1,
  //  0, 0, 1,
  //  0, 0, 1,
  //  0, 0, 1,
  //  //RIGHT
  //  1, 0, 0,
  //  1, 0, 0,
  //  1, 0, 0,
  //  1, 0, 0,
  //  1, 0, 0,
  //  1, 0, 0,
  //  //LEFT
  //  -1, 0, 0,
  //  -1, 0, 0,
  //  -1, 0, 0,
  //  -1, 0, 0,
  //  -1, 0, 0,
  //  -1, 0, 0,
  //  //TOP
  //  0, 1, 0,
  //  0, 1, 0,
  //  0, 1, 0,
  //  0, 1, 0,
  //  0, 1, 0,
  //  0, 1, 0,
  //  //BOTTOM
  //  0, -1, 0,
  //  0, -1, 0,
  //  0, -1, 0,
  //  0, -1, 0,
  //  0, -1, 0,
  //  0, -1, 0
  //};

  /*GLfloat cube_vertex[24] = {
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, 0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5
  };*/

  GLfloat cube_vertex[24] = {
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, 0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5
  };

  GLuint cube_index[36] = {
    1,5,7,
    7,3,1,
    0,2,6,
    6,4,0,
    0,1,3,
    3,2,0,
    7,5,4,
    4,6,7,
    2,3,7,
    7,6,2,
    1,0,4,
    4,5,1
  };
  
  //BufferObject* vbuff = new BufferObject(GL_ARRAY_BUFFER, sizeof(vertices), GL_STATIC_DRAW);
  BufferObject* vbuff = new BufferObject(GL_ARRAY_BUFFER, sizeof(cube_vertex), GL_STATIC_DRAW);
  vbuff->sendData(&cube_vertex[0]);
  attachBuffer(vbuff);

  /*BufferObject* nbuff = new BufferObject(GL_ARRAY_BUFFER, sizeof(normals), GL_STATIC_DRAW);
  nbuff->sendData(&normals[0]);
  attachBuffer(nbuff);*/

  bind();

  vbuff->bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  BufferObject* ibuff = new BufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), GL_STATIC_DRAW);
  ibuff->sendData(&cube_index[0]);
  attachBuffer(ibuff);

  /*nbuff->bind();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);*/

  glBindVertexArray(0);

  m_numPoints = sizeof(cube_index) / sizeof(GLfloat);
}


Cube::~Cube(void)
{
  for (unsigned int i = 0; i < m_buffers.size(); i++)
    delete m_buffers[i];
  m_buffers.clear();

  glDeleteVertexArrays(1, &m_vao);
}
