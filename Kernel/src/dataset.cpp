#include "dataset.h"
#include "perlin.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <cstring>
#include <GL/glew.h>

namespace knl
{

  Dataset* Dataset::createPyroclasticVolume(size_t sz, float r)
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

    dataset->loaded(true);
    dataset->uploaded(false);

    return dataset;
  }

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
    is_loaded = rhs.isLoaded();
    is_uploaded = rhs.isUploaded();

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
    loaded(false);
    uploaded(false);

    /*if(!tex_id) {
      glDeleteTextures(1, &tex_id);
      tex_id = 0;
    }*/
  }

  bool Dataset::load(std::string path)
  {
    assert(!path.empty());

    if(isLoaded()) {
      memset(data, 0, width * height * slices * bytes_elem);
      free(data);
      data = NULL;
      loaded(false);
    }
    if(isUploaded()) {
      //FIXME: Destroy the texture data here.
      uploaded(false);
    }

    data = calloc(width * height * slices, bytes_elem);
    loaded(io::LoadBinary(path, width * height * slices, bytes_elem, data));
    return isLoaded();
  }

  bool Dataset::save(std::string path)
  {
    assert(!path.empty());

    if(!isLoaded()) {
      return false;
    }

    return io::SaveBinary(path, width * height * slices, bytes_elem, data);
  }

  bool Dataset::upload()
  {
    assert(width != 0 && height != 0 && slices != 0 && bytes_elem != 0 && data != NULL);
    if(tex_id == 0)
      tex_id = knl::io::transfer::Alloc3DTex(width, height, slices, bytes_elem);

    assert(tex_id != 0);
    uploaded(knl::io::transfer::Upload3DData(width, height, slices, bytes_elem, data, tex_id));

    return isUploaded();
  }

  void Dataset::bind(GLenum tex_unit)
  {
    glActiveTexture(tex_unit);
    glBindTexture(GL_TEXTURE_3D, tex_id);
  }

}
