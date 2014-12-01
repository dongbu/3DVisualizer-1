#include "datatransfer.h"
#include <assert.h>

namespace knl
{
  namespace io
  {
    namespace transfer
    {
      GLuint Alloc1DTex(size_t w, size_t num_channels)
      {
        assert(w > 0 && num_channels > 0);

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_1D, tex);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLenum int_format;
        GLenum format;

        switch(num_channels) {
        case 1:
          int_format = GL_R8;
          format = GL_RED;
          break;
        case 3:
          int_format = GL_RGB8;
          format = GL_RGB;
          break;
        case 4:
          int_format = GL_RGBA8;
          format = GL_RGBA;
          break;
        }

        //glTexImage1D(GL_TEXTURE_1D, 0, int_format, w, 0, format, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_1D, 0);

        return tex;
      }

      GLuint Alloc2DTex(size_t w, size_t h, size_t num_channels)
      {
        assert(w > 0 && h > 0 && num_channels > 0);

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLenum int_format;
        GLenum format;

        switch(num_channels) {
        case 1:
          int_format = GL_R8;
          format = GL_RED;
          break;
        case 3:
          int_format = GL_RGB8;
          format = GL_RGB;
          break;
        case 4:
          int_format = GL_RGBA8;
          format = GL_RGBA;
          break;
        }

        //glTexImage2D(GL_TEXTURE_2D, 0, int_format, w, h, 0, format, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
      }

      GLuint Alloc3DTex(size_t w, size_t h, size_t slices, size_t bytes_elem)
      {
        assert(w > 0 && h > 0 && slices > 0 && bytes_elem > 0);

        //TODO: Get the currently bound texture and restore the state before the return.
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_3D, tex);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLenum type;
        GLenum format;
        switch(bytes_elem) {
        case sizeof(GLushort):
          type = GL_UNSIGNED_SHORT;
          format = GL_R16;
          break;
        case sizeof(GLuint):
          type = GL_FLOAT;
          format = GL_R32F;
          break;
        case sizeof(GLubyte):
        default:
          type = GL_UNSIGNED_BYTE;
          format = GL_R8;
          break;
        }

        //glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, slices, 0, GL_RED, type, NULL);
        glBindTexture(GL_TEXTURE_3D, 0);
        return tex;
      }

      bool Upload1DData(size_t w, size_t num_channels, void* data, GLuint tex_id)
      {
        assert(w > 0 && num_channels > 0 && data != NULL && tex_id != 0);

        GLenum int_format;
        GLenum format;

        switch(num_channels) {
        case 1:
          int_format = GL_R8;
          format = GL_RED;
          break;
        case 3:
          int_format = GL_RGB8;
          format = GL_RGB;
          break;
        case 4:
          int_format = GL_RGBA8;
          format = GL_RGBA;
          break;
        }

        glBindTexture(GL_TEXTURE_1D, tex_id);
        glTexImage1D(GL_TEXTURE_1D, 0, int_format, w, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_1D, 0);

        return true;
      }

      bool Upload2DData(size_t w, size_t h, size_t num_channels, void* data, GLuint tex_id)
      {
        assert(w > 0 && h > 0 && num_channels > 0 && data != NULL && tex_id != 0);

        GLenum int_format;
        GLenum format;

        switch(num_channels) {
        case 1:
          int_format = GL_R8;
          format = GL_RED;
          break;
        case 3:
          int_format = GL_RGB8;
          format = GL_RGB;
          break;
        case 4:
          int_format = GL_RGBA8;
          format = GL_RGBA;
          break;
        }

        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, int_format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
      }

      bool Upload3DData(size_t w, size_t h, size_t slices, size_t bytes_elem, void* data, GLuint tex_id)
      {
        assert(w > 0 && h > 0 && slices > 0 && bytes_elem > 0 && data != NULL && tex_id != 0);

        GLenum type;
        GLenum format;
        switch(bytes_elem) {
        case sizeof(GLushort):
          type = GL_UNSIGNED_SHORT;
          format = GL_R16;
          break;
        case sizeof(GLuint):
          type = GL_FLOAT;
          format = GL_R32F;
          break;
        case sizeof(GLubyte):
        default:
          type = GL_UNSIGNED_BYTE;
          format = GL_R8;
          break;
        }

        glBindTexture(GL_TEXTURE_3D, tex_id);
        glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, slices, 0, GL_RED, type, data);
        glBindTexture(GL_TEXTURE_3D, 0);

        return true;
      }
    }
  }
}
