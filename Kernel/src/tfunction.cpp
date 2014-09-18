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
    size_t sz = 0;
    if(bytes_elem == 1) sz = 256;
    else if(bytes_elem == 2) sz = 4096;
    sz *= rows;
    data = calloc(sz, bytes_elem);
    memcpy(data, rhs.data, sz);
  }
}

TFunction::~TFunction()
{
  if(data != NULL) {
    size_t sz = 0;
    
    if(bytes_elem == 1) sz = 256;
    else if(bytes_elem == 2) sz = 4096;
    sz *= rows;
    memset(data, 0, sz);
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

  std::cout << path << " " << GetWidth() << " " << rows << " " << num_channels << " " << bytes_elem << std::endl;
  data = calloc(GetWidth() * rows, sizeof(GLubyte));
  Loaded(data::LoadBinary(path, GetWidth() * rows, sizeof(GLubyte), data));
  return IsLoaded();
}

bool TFunction::UploadToGPU()
{
  assert(rows != 0 && num_channels != 0 && bytes_elem != 0 && data != NULL);
  if(tex_id == 0) {
    if(Is1D()) {
      tex_id = data::transfer::Alloc1DTex(GetWidth(), num_channels);
    } else {
      size_t w = bytes_elem == sizeof(GLubyte) ? 256 : 4096;
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

void TFunction::bind()
{
  GLenum target;
  
  if(Is1D())
    target = GL_TEXTURE_1D;
  else
    target = GL_TEXTURE_2D;

  glBindTexture(target, tex_id);
}