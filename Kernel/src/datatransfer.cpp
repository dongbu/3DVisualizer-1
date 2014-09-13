#include "datatransfer.h"
#include <assert.h>

namespace data
{
  namespace transfer
  {
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

      GLenum type;
      switch(bytes_elem) {
      case sizeof(GLushort):
	type = GL_UNSIGNED_SHORT;
	break;
      case sizeof(GLubyte):
      default:
	type = GL_UNSIGNED_BYTE;
	break;
      }

      glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, w, h, slices, 0, GL_RED, type, NULL);
      glBindTexture(GL_TEXTURE_3D, 0);
      return tex;
    }

    bool Upload3DData(size_t w, size_t h, size_t slices, size_t bytes_elem, bool size_changed, void* data, GLuint tex_id)
    {
      assert(w > 0 && h > 0 && slices > 0 && bytes_elem > 0 && data != NULL && tex_id != 0);

      //TODO: Get the currently bound texture and restore the state before the return.
      glBindTexture(GL_TEXTURE_3D, tex_id);

      GLenum type;
      switch(bytes_elem) {
      case sizeof(GLushort):
	type = GL_UNSIGNED_SHORT;
	break;
      case sizeof(GLubyte):
      default:
	type = GL_UNSIGNED_BYTE;
	break;
      }

      if(size_changed) {
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, w, h, slices, 0, GL_RED, type, data);
      } else {
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, w, h, slices, GL_RED, type, data);
      }

      glBindTexture(GL_TEXTURE_3D, 0);

      return true;
    }
  }
}
