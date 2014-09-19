#include "dataset.h"
#include "perlin.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <GL/glew.h>

Dataset* Dataset::CreatePyroclasticVolume(size_t sz, float r)
{
  Dataset* dataset = new Dataset;
  dataset->width = dataset->height = dataset->slices = sz;
  dataset->bytes_elem = sizeof(GLubyte);

  dataset->data = new GLubyte[sz * sz * sz];
  GLubyte* ptr = (GLubyte*) dataset->data;

  float frequency = 3.0f / sz;
  float center = sz / 2.0f + 0.5f;

  for(size_t x = 0; x < sz; ++x) {
    for(size_t y = 0; y < sz; ++y) {
      for(size_t z = 0; z < sz; ++z) {
        float dx = center - x;
        float dy = center - y;
        float dz = center - z;

        float off = fabsf((float) PerlinNoise3D(x * frequency, y * frequency, z * frequency, 5, 6, 3));

        float d = sqrtf(dx*dx + dy*dy + dz*dz) / sz;
        bool isFilled = (d - off) < r;
        *ptr++ = isFilled ? 255 : 0;
      }
    }
  }

  dataset->Loaded(true);
  dataset->Uploaded(false);

  return dataset;
}

// // Dataset* Dataset::Load(std::string path)
// {
//   assert(!path.empty());
//   Dataset* data = new Dataset;

//   // Path parsing.
//   std::vector<std::string> strs;
//   boost::algorithm::split(strs, path, boost::is_any_of("."));

//   data->bytes_elem = strs[2] == "uint8" ? sizeof(GLubyte) :
//     (strs[2] == "uint32" ? sizeof(GLuint) :
//      (strs[2] == "float" ? sizeof(GLfloat) : sizeof(GLushort)));

//   std::vector<std::string> dim_strs;
//   boost::algorithm::split(dim_strs, strs[1], boost::is_any_of("x"));

//   std::istringstream(dim_strs[0]) >> data->width;
//   std::istringstream(dim_strs[1]) >> data->height;
//   std::istringstream(dim_strs[2]) >> data->slices;

//   //Allocation of space and loading from disk.
//   data->data = calloc(data->width * data->height * data->slices, data->bytes_elem);
//   data::LoadBinary(path, data->width * data->height * data->slices, data->bytes_elem, data->data);
//   data->Loaded(true);
//   return data;
// }

Dataset::Dataset()
{
  width = height = slices = bytes_elem = 0;
  data = NULL;
  is_loaded = is_uploaded = false;
  tex_id = 0;
}

Dataset::Dataset(Dataset& rhs)
{
  width = rhs.width;
  height = rhs.height;
  slices = rhs.slices;
  bytes_elem = rhs.bytes_elem;
  tex_id = rhs.tex_id;
  is_loaded = rhs.IsLoaded();
  is_uploaded = rhs.IsUploaded();

  if(rhs.data == NULL) {
    data = NULL;
  } else {
    data = calloc(width * height * slices, bytes_elem);
    memcpy(data, rhs.data, width * height * slices * bytes_elem);
  }
}

Dataset::~Dataset()
{
  if(data != NULL) {
    memset(data, 0, width * height * slices * bytes_elem);
    free(data);
    data = NULL;
  }
  width = height = slices = bytes_elem = 0;
  Loaded(false);
  Uploaded(false);

  if(!tex_id) {
    glDeleteTextures(1, &tex_id);
    tex_id = 0;
  }
}

bool Dataset::Load(std::string path)
{
  assert(!path.empty());
  if(IsLoaded()) {
    memset(data, 0, width * height * slices * bytes_elem);
    free(data);
    data = NULL;
    Loaded(false);
  }
  if(IsUploaded()) {
    Uploaded(false);
  }

  data = calloc(width * height * slices, bytes_elem);
  Loaded(data::LoadBinary(path, width * height * slices, bytes_elem, data));
  return IsLoaded();
}

bool Dataset::UploadToGPU()
{
  assert(width != 0 && height != 0 && slices != 0 && bytes_elem != 0 && data != NULL);
  if(tex_id == 0)
    tex_id = data::transfer::Alloc3DTex(width, height, slices, bytes_elem);

  assert(tex_id != 0);
  Uploaded(data::transfer::Upload3DData(width, height, slices, bytes_elem, data, tex_id));

  return IsUploaded();
}

void Dataset::bind(GLenum tex_unit)
{
  glActiveTexture(tex_unit);
  glBindTexture(GL_TEXTURE_3D, tex_id);
}