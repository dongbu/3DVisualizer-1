#ifndef TFUNC_H
#define TFUNC_H

#include "datahandler.h"
#include "datatransfer.h"

class TFunction
{
public:
  GLuint tex_id;
  void* data;
  size_t bytes_elem;
  size_t rows;
  size_t num_channels;

  TFunction();
  TFunction(TFunction& rhs);
  ~TFunction();

  bool IsLoaded() { return is_loaded; }
  bool IsUploaded() { return is_uploaded; }

  void Loaded(bool l) { is_loaded = l; }
  void Uploaded(bool u) { is_uploaded = u; }

  bool Load(std::string path);
  bool UploadToGPU();

  bool Is1D() { return rows == 1; }

  size_t GetWidth()
  {
    return bytes_elem == sizeof(GLubyte) ? 256 : 4096;
  }

  void bind(GLenum tex_unit = GL_TEXTURE0);

private:
  bool is_loaded;
  bool is_uploaded;

};

#endif // TFUNC_H
