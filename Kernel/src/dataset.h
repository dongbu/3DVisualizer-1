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

    bool IsLoaded() { return is_loaded; }
    bool IsUploaded() { return is_uploaded; }

    void Loaded(bool l) { is_loaded = l; }
    void Uploaded(bool u) { is_uploaded = u; }

    virtual bool Load(std::string path);
    virtual bool UploadToGPU();

    virtual void bind(GLenum tex_unit = GL_TEXTURE0);

    static Dataset* CreatePyroclasticVolume(size_t sz, float r);

    virtual size_t Get(size_t idx)
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

    virtual void Set(size_t idx, size_t val)
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
      return Get(idx);
    }

    size_t operator()(int i, int j, int k)
    {
      return Get(ToLinear(i, j, k));
    }

  private:
    bool is_uploaded;
    bool is_loaded;

    inline size_t ToLinear(int i, int j, int k)
    {
      return (k * height + j) * width + i;
    }
  };

}

#endif // DATASET_H
