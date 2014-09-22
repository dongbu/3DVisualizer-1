#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <string>


namespace knl
{
  /**
  * This namespace contains any functions related to loading/saving data from/to
  * the disk.
  */
  namespace data
  {
    bool LoadBinary(std::string path, size_t num_elem, size_t bytes_elem, void* data);

    bool SaveBinary(std::string path, size_t num_elem, size_t bytes_elem, void* data);
  }

}
#endif // DATAHANDLER_H
