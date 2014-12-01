#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <string>
#include <GL/glew.h>

namespace knl
{
  namespace io
  {
    /**
     * This namespace contains the functions that allocate/transfer the data to
     * the OpenGL context. The data is always allocated as a texture.
     */
    namespace transfer
    {
      /**
       * @brief Allocates an one dimensional texture.
       *
       * This function request a texture name to the OpenGL context using the
       * glGenTextures function, allocates the space and sets the parameters (MIN,
       * MAG filters and WRAP_S mode). The format and internal format used in the
       * glTexImage1D function are calculated using the number of channels given.
       *
       * The texture data is always GL_UNSIGNED_BYTE. This function is used mainly
       * to allocate transfer function data.
       *
       * @param w Width of the texture.
       * @param num_channels Number o channels used.
       * @return GLuint The texture id given by the OpenGL context (glGenTextures
       *         function).
       */
      GLuint Alloc1DTex(size_t w, size_t num_channels);

      /**
       * @brief Allocates a 2D texture.
       *
       * Same as the Alloc1DTex function. The OpenGL texture id given by the
       * glGenTextures function. The space is allocated and parameters (MIN, MAG,
       * WRAP_*) set in the same ways as the Alloc1DTex function.
       *
       * The texture data is always GL_UNSIGNED_BYTE, as this function was also
       * intended to be used to allocate multidimensional transfer function data.
       *
       * @param w The width of the image.
       * @param h The height of the image
       * @param num_channels The number of channels
       * @return GLuint The texture id given by the OpenGL context.
       */
      GLuint Alloc2DTex(size_t w, size_t h, size_t num_channels);

      /**
       * @brief Allocates a 3D texture.
       *
       * This function allocates a 3D texture in the OpenGL device. This is done
       * by getting a name from the context, setting the texture parameters (MIN,
       * MAG filters and WRAP_*) and calling the glTexImage3D with a NULL pointer.
       * The format and type of the data is determined using the bytes_elem
       * parameter. The type may be GL_UNSIGNED_(BYTE,SHORT,INT) and ther format
       * may be GL_R8, GL_R16 and GL_R32F respectively.
       *
       * This function was intended to be used to allocate the space for a dataset
       * in the GPU.
       *
       * @param w The width of the volume
       * @param h The height of the volume
       * @param slices The number of slices of the volume
       * @param bytes_elem The size in bytes of each voxel.
       *        Possible values are sizeof(GLubyte/GLushort/GLfloat).
       * @return GLuint The texture id given by the OpenGL context.
       */
      GLuint Alloc3DTex(size_t w, size_t h, size_t slices, size_t bytes_elem);

      /**
       * @brief Uploads a 1D texture to the OpenGL device.
       *
       * This function allocates and uploads a 1D texture to the OpenGL device.
       * This is done by receiving a valid texture name (via the tex_id parameter)
       * queriyng the internal format and channel format using the num_channels
       * parameter and uploading the data.
       *
       * The texture data is always GL_UNSIGNED_BYTE. This function is used mainly
       * to allocate transfer function data.
       *
       * @param w The width of the texture
       * @param num_channels The number of channels (1 for GL_RED, 4 for GL_RGBA)
       * @param data Pointer to the data
       * @param tex_id The texture name
       * @return bool True if the texture was uploaded successfully, false
       *         otherwise
       */
      bool Upload1DData(size_t w, size_t num_channels, void* data, GLuint tex_id);

      /**
       * @brief Uploads a 2D texture to the OpenGL device.
       *
       * This function allocates and uploads a 2D texture to the OpenGL device.
       * This is done by receiving the texture name (via the tex_id parameter)
       * setting the internal format and channel format from the num_channels
       * parameter and then the data is uploaded.
       *
       * The texture data is always GL_UNSIGNED_BYTE, since this function is used
       * mainly to allocate multidimensional transfer function data.
       *
       * @param w The width of the texture
       * @param h The height of the texture
       * @param num_channels Number of channels (1 for GL_RED, 4 for GL_RGBA)
       * @param data Pointer to the texture data
       * @param tex_id Texture name given by the OpenGL context
       * @return bool True if the texture was uploaded successfully,
       *         false otherwise.
       */
      bool Upload2DData(size_t w, size_t h, size_t num_channels, void* data, GLuint tex_id);

      /**
       * @brief Uploads a 3D texture to the OpenGL device.
       *
       * This function allocates and uploads a 3D texture to the OpenGL device.
       * This is done by receiving the texture name given by the OpenGL context,
       * setting the voxel data type and formad from the number of bytes passed
       * as a parameter and then uploading the data to the device.
       *
       * This function is intended to be used to upload volumetric data to be
       * rendered by a volume raycasting algorithm. The number of channels is
       * always one, since each voxel has only one intensity value.
       *
       * @param w The width of the texture
       * @param h The height of the texture
       * @param slices The number of slices (depth) of the texture
       * @param bytes_elem The voxel size in bytes (1 for GL_UNSIGNED_BYTE, 2 for
       *       GL_UNSIGNED_SHORT and 4 for GL_FLOAT)
       * @param data The pointer to the data
       * @param tex_id Texture name given by the OpenGL context
       * @return bool True if the texture was uploaded successfully,
       *         false otherwise
       */
      bool Upload3DData(size_t w, size_t h, size_t slices, size_t bytes_elem, void* data, GLuint tex_id);
    }
  }
}
#endif // DATATRANSFER_H
