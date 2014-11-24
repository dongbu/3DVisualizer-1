#ifndef TFUNC_H
#define TFUNC_H

#include "datahandler.h"
#include "datatransfer.h"

/*!
 \brief

*/
class TFunction
{
public:
  GLuint tex_id; /*!< The texture ID given by the OpenGL context */
  void* data; /*!< Pointer to the transfer function data */
  size_t bytes_elem; /*!< Number of bytes of the dataset. Note that this is used
                          to calculate the size of the transfer function. For instance,
                          if the dataset is composed of 1 byte voxels, the transfer
                          function will have a width of 256 bytes. The same goes for
                          2 byte voxels, the corresponding function will have 4096 bytes. */
  size_t rows; /*!< 1 for single dimensional functions >1 for multi dimensional. Each row
                    will be treated as one dimension.*/
  size_t num_channels; /*!< Number of channels. 1 for opacity transfer functions, 4 for
                            color + opacity (RGBA)*/

  /*!
   \brief Default constructor.

  */
  TFunction();

  /*!
   \brief Copy constructor.

   \param rhs
  */
  TFunction(TFunction& rhs);

  /*!
   \brief Default destructor. Frees the memory but does not destroy the texture.

  */
  ~TFunction();

  /*!
   \brief Test if the function was properly loaded to the memory.

   \return true if loaded, false otherwise.
  */
  bool isLoaded() { return is_loaded; }

  /*!
   \brief Test if the function was properly uploaded to the GPU.

   \return true if uploaded, false otherwise.
  */
  bool isUploaded() { return is_uploaded; }

  /*!
   \brief Load the data from a file.

   \param path Full path to the file.
   \return true if the file was successfully loaded, false otherwise.
  */
  bool load(std::string path);

  /*!
   \brief Save the data from a file.

   \param path Full path to the file.
   \return true if the file was successfully saved, false otherwise.
  */
  bool save(std::string path);

  /*!
   \brief Upload the data to the OpenGL device

   \return true if the upload was successful, false otherwise.
  */
  bool upload();

  /*!
   \brief Indicates if this is a single or multi dimentional transfer function.

   \return true for a single dimensional function, false for multi dimensional.
  */
  bool is1D() { return rows == 1; }

  /*!
   \brief

   \return size_t
  */
  size_t width()
  {
    return bytes_elem == sizeof(GLubyte) ? 256 : 4096;
  }

  /*!
   \brief Bind the corresponding texture to the texture unit.
    Before using the transfer function for rendering, the user must set
    it as active in the OpenGL context. This method does exactly this.
   \param tex_unit The texture unit to bind the function. Default is GL_TEXTURE0
  */
  void bind(GLenum tex_unit = GL_TEXTURE0);

protected:
  /*!
   \brief Sets wheter the function was loaded.

   \param l true if it was loaded, false otherwise.
  */
  void loaded(bool l) { is_loaded = l; }

  /*!
   \brief Sets wheter the function was uploaded to the GPU.

   \param u true if it was uploaded, false otherwise.
  */
  void uploaded(bool u) { is_uploaded = u; }

private:
  bool is_loaded; /*!< TODO */
  bool is_uploaded; /*!< TODO */

};

#endif // TFUNC_H
