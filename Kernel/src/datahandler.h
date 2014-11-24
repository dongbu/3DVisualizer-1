#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <string>


namespace knl
{
  /**
  * This namespace contains functions related to loading/saving data from/to
  * the disk.
  */
  namespace io
  {

    /*!
     \brief Load binary data from a file on the disk.
     This function loads the data in binary from the file specified by "path". The
     data buffer must be pre allocated and able to hold num_elements * bytes_elem bytes.

     \param path The full path to the file, e.g. /home/xpto/datasets/bonsai.raw
     \param num_elem The number of elements to be read.
     \param bytes_elem The size in bytes of each element.
     \param data The buffer to hold the data. This buffer must be pre allocated.
     \return bool true if the data was successfully loaded, false otherwise.
    */
    bool LoadBinary(std::string path, size_t num_elem, size_t bytes_elem, void* io);

    /*!
     \brief Save binary data to the disk.
     This function creates a file specified by the "path" parameter and tries to write the
     contents of the "data" buffer. If successful, the function returns true.

     \param path The full path to the file.
     \param num_elem The number of elements in the data array.
     \param bytes_elem The size of each element in bytes.
     \param data The pointer to the data.
     \return bool true if the data was successfully saved, false otherwise.
    */
    bool SaveBinary(std::string path, size_t num_elem, size_t bytes_elem, void* io);
  }

}
#endif // DATAHANDLER_H
