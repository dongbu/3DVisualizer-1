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
  rows = bytes_elem = tex_id = 0;
  is_loaded = is_uploaded = false;
  data = NULL;
}

TFunction::TFunction(TFunction& rhs)
{
  if(this == &rhs) return;
  rows = rhs.rows;
  bytes_elem = rhs.bytes_elem;
  tex_id = rhs.tex_id;
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

  rows = bytes_elem = 0;
  Loaded(false);
  Uploaded(false);

  if(!tex_id) {
    glDeleteTextures(1, &tex_id);
    tex_id = 0;
  }
}

bool TFunction::Load(std::string path)
{
  return false;
}

bool TFunction::UploadToGPU()
{
  /*assert(width != 0 && height != 0 && slices != 0 && bytes_elem != 0 && data != NULL);
  if(tex_id == 0)
  tex_id = data::transfer::Alloc3DTex(width, height, slices, bytes_elem);

  assert(tex_id != 0);
  Uploaded(data::transfer::Upload3DData(width, height, slices, bytes_elem, data, tex_id));*/

  return IsUploaded();
}

void TFunction::bind()
{
  //glBindTexture(GL_TEXTURE_2D, tex_id);
}