#ifndef DATASET_H
#define DATASET_H

#include "datahandler.h"
#include "datatransfer.h"
#include <GL/glew.h>

namespace knl
{

  class Dataset
  {
  public:
    GLuint tex_id;
    void* data;
    size_t width;
    size_t height;
    size_t slices;
    size_t bytes_elem;

    Dataset();
    Dataset(Dataset& rhs);
    virtual ~Dataset();

    bool isLoaded() { return is_loaded; }
    bool isUploaded() { return is_uploaded; }

    void loaded(bool l) { is_loaded = l; }
    void uploaded(bool u) { is_uploaded = u; }

    virtual bool load(std::string path);
    virtual bool upload();

    virtual void bind(GLenum tex_unit = GL_TEXTURE0);

    static Dataset* createPyroclasticVolume(size_t sz, float r);

    virtual size_t get(size_t idx)
    {
      if(bytes_elem == sizeof(GLushort)) {
        unsigned short* ptr = (unsigned short*) data + idx;
        return (size_t) *ptr;
      } else if(bytes_elem == sizeof(GLubyte)) {
        unsigned char* ptr = (unsigned char*) data + idx;
        return (size_t) *ptr;
      } else {
        size_t* ptr = (size_t*) data + idx;
        return *ptr;
      }
    }

    virtual void set(size_t idx, size_t val)
    {
      if(bytes_elem == sizeof(GLushort)) {
        GLushort* ptr = (GLushort*) data + idx;
        *ptr = (GLushort) val;
      } else if(bytes_elem == sizeof(GLubyte)) {
        GLubyte* ptr = (GLubyte*) data + idx;
        *ptr = (GLubyte) val;
      } else {
        size_t* ptr = (size_t*) data + idx;
        *ptr = val;
      }
    }

    size_t operator()(int idx)
    {
      return get(idx);
    }

    size_t operator()(int i, int j, int k)
    {
      return get(linearAddr(i, j, k));
    }

  private:
    bool is_uploaded;
    bool is_loaded;

    inline size_t linearAddr(int i, int j, int k)
    {
      return (k * height + j) * width + i;
    }
  };

}

#endif // DATASET_H
