#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <string>
#include <GL/glew.h>

namespace knl
{
  namespace io
  {
    /**
     * This namespace contains the functions that allocate/transfer the data to the
     * OpenGL context.
     */
    namespace transfer
    {
      /*!
       \brief Allocates an one dimensional texture.

       This function request a texture name to the OpenGL context using the glGenTextures function,
       allocates the space and sets the parameters (MIN, MAG filterns and WRAP_S mode).
       The format and internal format used in the glTexImage1D function are calculated using the
       number of channels given.

       The texture data is always GL_UNSIGNED_BYTE. This function is used mainly to allocate transfer
       function data.

       \param w Width of the texture.
       \param num_channels Number o channels used.
       \return GLuint The texture id given by the OpenGL context (glGenTextures function).
      */
      GLuint Alloc1DTex(size_t w, size_t num_channels);

      /*!
       \brief Allocates a 2D texture.

       Same as the Alloc1DTex function. The OpenGL texture id given by the glGenTextures function.
       The space is allocated and parameters (MIN, MAG, WRAP_*) set in the same ways as the
       Alloc1DTex function.

       The texture data is always GL_UNSIGNED_BYTE, as this function was also intended to be used to
       allocate multidimensional transfer function data.

       \param w The width of the image.
       \param h The height of the image
       \param num_channels The number of channels
       \return GLuint The texture id given by the OpenGL context.
      */
      GLuint Alloc2DTex(size_t w, size_t h, size_t num_channels);

      /*!
       \brief Allocates a 3D texture.

       This function allocates a 3D texture in the OpenGL device. This is done by getting a name from
       the context, setting the texture parameters (MIN, MAG filters and WRAP_*) and calling the
       glTexImage3D with a NULL pointer. The format and type of the data is determined using the
       bytes_elem parameter. The type may be GL_UNSIGNED_(BYTE,SHORT,INT) and ther format may be
       GL_R8, GL_R16 and GL_R32F respectively.

       This function was intended to be used to allocate the space for a dataset in the GPU.

       \param w The width of the volume
       \param h The height of the volume
       \param slices The number of slices of the volume
       \param bytes_elem The size in bytes of each voxel.
              Possible values are sizeof(GLubyte/GLushort/GLfloat).
       \return GLuint The texture id given by the OpenGL context.
      */
      GLuint Alloc3DTex(size_t w, size_t h, size_t slices, size_t bytes_elem);

      /*!
       \brief

       \param w
       \param num_channels
       \param data
       \param tex_id
       \return bool
      */
      bool Upload1DData(size_t w, size_t num_channels, void* io, GLuint tex_id);

      /*!
       \brief

       \param w
       \param h
       \param num_channels
       \param data
       \param tex_id
       \return bool
      */
      bool Upload2DData(size_t w, size_t h, size_t num_channels, void* io, GLuint tex_id);

      /*!
       \brief

       \param w
       \param h
       \param slices
       \param bytes_elem
       \param data
       \param tex_id
       \return bool
      */
      bool Upload3DData(size_t w, size_t h, size_t slices, size_t bytes_elem, void* io, GLuint tex_id);
    }
  }
}
#endif // DATATRANSFER_H
