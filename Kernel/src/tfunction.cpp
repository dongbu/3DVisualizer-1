#include "tfunction.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <GL/glew.h>

using namespace knl;

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
  is_loaded = rhs.isLoaded();
  is_uploaded = rhs.isUploaded();

  if(rhs.data == NULL) {
    data = NULL;
  } else {
    size_t sz = width() * rows * num_channels;
    data = calloc(sz, sizeof(GLubyte));
    memcpy(data, rhs.data, sz * sizeof(GLubyte));
  }
}

TFunction::~TFunction()
{
  if(data != NULL) {
    memset(data, 0, width() * rows * num_channels * sizeof(GLubyte));
    free(data);
    data = NULL;
  }

  num_channels = rows = bytes_elem = 0;
  loaded(false);
  uploaded(false);

  if(!tex_id) {
    glDeleteTextures(1, &tex_id);
    tex_id = 0;
  }
}

bool TFunction::load(std::string path)
{
  assert(!path.empty());
  if(isLoaded()) {
    memset(data, 0, width() * rows * num_channels * sizeof(GLubyte));
    free(data);
    data = NULL;
    loaded(false);
  }
  if(isUploaded()) {
    uploaded(false);
  }
  size_t sz = width() * rows * num_channels;
  data = calloc(sz, sizeof(GLubyte));
  loaded(io::LoadBinary(path, sz, sizeof(GLubyte), data));

  return isLoaded();
}

bool TFunction::save(std::string path)
{
  assert(!path.empty());

  if(!isLoaded()) {
    return false;
  }

  return io::SaveBinary(path, width(), sizeof(GLubyte), data);
}

bool TFunction::upload()
{
  assert(rows != 0 && num_channels != 0 && bytes_elem != 0 && data != NULL);
  if(tex_id == 0) {
    if(is1D()) {
      tex_id = io::transfer::Alloc1DTex(width(), num_channels);
    } else {
      tex_id = io::transfer::Alloc2DTex(width(), rows, num_channels);
    }
  }

  assert(tex_id != 0);
  if(is1D()) {
    uploaded(io::transfer::Upload1DData(width(), num_channels, data, tex_id));
  } else {
    uploaded(io::transfer::Upload2DData(width(), rows, num_channels, data, tex_id));
  }

  return isUploaded();
}

void TFunction::bind(GLenum tex_unit)
{
  GLenum target;
  
  if(is1D())
    target = GL_TEXTURE_1D;
  else
    target = GL_TEXTURE_2D;

  glActiveTexture(tex_unit);
  glBindTexture(target, tex_id);
}
