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

  bool isLoaded() { return is_loaded; }
  bool isUploaded() { return is_uploaded; }

  void loaded(bool l) { is_loaded = l; }
  void uploaded(bool u) { is_uploaded = u; }

  bool load(std::string path);
  bool upload();

  bool is1D() { return rows == 1; }

  size_t width()
  {
    return bytes_elem == sizeof(GLubyte) ? 256 : 4096;
  }

  void bind(GLenum tex_unit = GL_TEXTURE0);

private:
  bool is_loaded;
  bool is_uploaded;

};

#endif // TFUNC_H
