#ifndef DATASET_H
#define DATASET_H

#include "datahandler.h"
#include "datatransfer.h"
#include <GL/glew.h>

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
  ~Dataset();

  bool IsLoaded() { return is_loaded; }
  bool IsUploaded() { return is_uploaded; }

  void Loaded(bool l) { is_loaded = l; }
  void Uploaded(bool u) { is_uploaded = u; }

  bool Load(std::string path);
  bool UploadToGPU();

  void bind(GLenum tex_unit = GL_TEXTURE0);

  static Dataset* CreatePyroclasticVolume(size_t sz, float r);

private:
  bool is_uploaded;
  bool is_loaded;

};

#endif // DATASET_H
