#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <string>
#include <GL/glew.h>

/**
 * This namespace contains any functions related to loading/saving data from/to
 * the disk.
 */
namespace data
{
  namespace transfer
  {
    GLuint Alloc3DTex(size_t w, size_t h, size_t slices, size_t bytes_elem);

    bool Upload3DData(size_t w, size_t h, size_t slices, size_t bytes_elem, void* data, GLuint tex_id);
  }
}

#endif // DATATRANSFER_H
