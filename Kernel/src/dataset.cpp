#include "dataset.h"
#include "datahandler.h"
#include "datatransfer.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <boost/algorithm/string.hpp>
#include <GL/glew.h>

Dataset* Dataset::Load(std::string path)
{
  assert(!path.empty());
  Dataset* data = new Dataset;

  // Path parsing.
  std::vector<std::string> strs;
  boost::algorithm::split(strs, path, boost::is_any_of("."));

  data->bytes_elem = strs[2] == "uint8" ? sizeof(GLubyte) :
    (strs[2] == "uint32" ? sizeof(GLuint) :
     (strs[2] == "float" ? sizeof(GLfloat) : sizeof(GLushort)));

  std::vector<std::string> dim_strs;
  boost::algorithm::split(dim_strs, strs[1], boost::is_any_of("x"));

  std::istringstream(dim_strs[0]) >> data->width;
  std::istringstream(dim_strs[1]) >> data->height;
  std::istringstream(dim_strs[2]) >> data->slices;

  std::cout << path << " " << data->width << " " << data->height << " " << data->slices << " " << data->bytes_elem << std::endl;
  data->data = calloc(data->width * data->height * data->slices, data->bytes_elem);
  data::LoadBinary(path, data->width * data->height * data->slices, data->bytes_elem, data->data);
  return data;
}

bool Dataset::Save(std::string path, Dataset* data)
{
  assert(!path.empty() && data != NULL);
  std::cout << "to be implemented" << std::endl;
  return false;
}

Dataset::Dataset()
{
  width = height = slices = bytes_elem = 0;
  data = NULL;
}

Dataset::Dataset(Dataset& rhs)
{
  width = rhs.width;
  height = rhs.height;
  slices = rhs.slices;
  bytes_elem = rhs.bytes_elem;

  data = calloc(width * height * slices, bytes_elem);
  memcpy(data, rhs.data, width * height * slices * bytes_elem);
}

Dataset::~Dataset()
{
  width = height = slices = bytes_elem = 0;
  if(data != NULL) {
    free(data);
    data = NULL;
  }
}

bool Dataset::UploadToGPU()
{
  assert(width != 0 && height != 0 && slices != 0 && bytes_elem != 0 && data != NULL);

  GLuint tex = data::transfer::Alloc3DTex(width, height, slices, bytes_elem);
  assert(tex != 0);

  bool ret = data::transfer::Upload3DData(width, height, slices, bytes_elem, false, data, tex);
  return ret;
}
