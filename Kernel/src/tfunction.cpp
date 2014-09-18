#include "tfunction.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <boost/algorithm/string.hpp>
#include <GL/glew.h>

TFunction::TFunction()
{
  rows = bytes_elem = tex_id = num_channels = 0;
  is_loaded = is_uploaded = false;
  data = NULL;
}

TFunction::TFunction(TFunction& rhs)
{
  if(this == &rhs) return;
  rows = rhs.rows;
  bytes_elem = rhs.bytes_elem;
  tex_id = rhs.tex_id;
  num_channels = rhs.num_channels;
  is_loaded = rhs.IsLoaded();
  is_uploaded = rhs.IsUploaded();

  if(rhs.data == NULL) {
    data = NULL;
  } else {
    data = calloc(GetWidth() * rows, bytes_elem);
    memcpy(data, rhs.data, GetWidth() * rows);
  }
}

TFunction::~TFunction()
{
  if(data != NULL) {
    memset(data, 0, GetWidth() * rows);
    free(data);
    data = NULL;
  }

  num_channels = rows = bytes_elem = 0;
  Loaded(false);
  Uploaded(false);

  if(!tex_id) {
    glDeleteTextures(1, &tex_id);
    tex_id = 0;
  }
}

bool TFunction::Load(std::string path)
{
  assert(!path.empty());
  if(IsLoaded()) {
    memset(data, 0, GetWidth() * rows * sizeof(GLubyte));
    free(data);
    data = NULL;
    Loaded(false);
  }
  if(IsUploaded()) {
    Uploaded(false);
  }

  data = calloc(GetWidth() * rows, sizeof(GLubyte));
  Loaded(data::LoadBinary(path, GetWidth() * rows, sizeof(GLubyte), data));

  unsigned char* tmp_data = (unsigned char*)data;
  for(int i = 0; i < GetWidth() * rows - 4; i+=4) {
    printf("i = %d = {%d, %d, %d, %d}\n", i, tmp_data[i], tmp_data[i+1], tmp_data[i+2], tmp_data[i+3]);
  }

  return IsLoaded();
}

bool TFunction::UploadToGPU()
{
  assert(rows != 0 && num_channels != 0 && bytes_elem != 0 && data != NULL);
  if(tex_id == 0) {
    if(Is1D()) {
      tex_id = data::transfer::Alloc1DTex(GetWidth(), num_channels);
    } else {
      tex_id = data::transfer::Alloc2DTex(GetWidth(), rows, num_channels);
    }
  }

  assert(tex_id != 0);
  if(Is1D()) {
    Uploaded(data::transfer::Upload1DData(GetWidth(), num_channels, data, tex_id));
  } else {
    Uploaded(data::transfer::Upload2DData(GetWidth(), rows, num_channels, data, tex_id));
  }

  return IsUploaded();
}

void TFunction::bind(GLenum tex_unit)
{
  GLenum target;
  
  if(Is1D())
    target = GL_TEXTURE_1D;
  else
    target = GL_TEXTURE_2D;

  glActiveTexture(tex_unit);
  glBindTexture(target, tex_id);
}