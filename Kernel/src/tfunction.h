#ifndef TFUNC_H
#define TFUNC_H

#include "datahandler.h"
#include "datatransfer.h"

/*!
 \brief This class represents a single transfer function.

 A transfer function is a lookup table, or color map to give a visual
 representation of intensity values that are not necessarily in our visual
 range. In our case, this transfer function is used to render 3D datasets
 usually created by MRI scans.

 The class holds the number of color channels, the size in bytes of each
 intensity value (byte, short, int, etc), a pointer to the function itself
 and the texture name given by the OpenGL context.

 The function data can be loaded from a file or saved to one. The data can also
 be uploaded as a 1D texture to the OpenGL device an manipulated as such. Upon
 uploading, the data pointer in the host memory is not altered or deleted in any
 way.

 There is a basic support for multidimensional transfer functions, but it is not
 used in the project.
*/
class TFunction
{
public:
  GLuint tex_id;
  void* data;
  size_t bytes_elem; /*!< Number of bytes of the dataset. Note that this is used
                          to calculate the size of the transfer function. For instance,
                          if the dataset is composed of 1 byte voxels, the transfer
                          function will have a width of 256 bytes. The same goes for
                          2 byte voxels, the corresponding function will have 4096 bytes. */
  size_t rows; /*!< 1 for single dimensional functions >1 for multi dimensional. Each row
                    will be treated as one dimension.*/
  size_t num_channels; /*!< Number of channels. 1 for opacity transfer functions, 4 for
                            color + opacity (RGBA)*/

  TFunction();
  TFunction(TFunction& rhs);
  ~TFunction();

  bool isLoaded() { return is_loaded; }
  bool isUploaded() { return is_uploaded; }

  bool load(std::string path);
  bool save(std::string path);
  bool upload();

  /*!
   \brief Indicates if this is a single or multi dimentional transfer function.

   \return true for a single dimensional function, false for multi dimensional.
  */
  bool is1D() { return rows == 1; }

  size_t width()
  {
    return bytes_elem == sizeof(GLubyte) ? 256 : 4096;
  }

  void bind(GLenum tex_unit = GL_TEXTURE0);

protected:
  void loaded(bool l) { is_loaded = l; }
  void uploaded(bool u) { is_uploaded = u; }

private:
  bool is_loaded;
  bool is_uploaded;

};

#endif // TFUNC_H
