#include "datahandler.h"
#include <assert.h>

namespace knl
{
  namespace data
  {
    bool LoadBinary(std::string path, size_t num_elem, size_t bytes_elem, void* data)
    {
      assert(!path.empty() && num_elem > 0 && bytes_elem > 0);
      assert(data != NULL);
      FILE* fp;

      if(!(fp = fopen(path.c_str(), "rb"))) {
        return false;
      }

      size_t bytes_read = fread(data, bytes_elem, num_elem, fp);
      fclose(fp);
      fp = NULL;

      if(bytes_read != num_elem) {
        return false;
      }

      return true;
    }

    bool SaveBinary(std::string path, size_t num_elem, size_t bytes_elem, void* data)
    {
      assert(!path.empty() && num_elem > 0 && bytes_elem > 0 && data != NULL);
      FILE* fp;

      if(!(fp = fopen(path.c_str(), "wb+"))) {
        return false;
      }

      size_t bytes_written = fwrite(data, bytes_elem, num_elem, fp);
      fclose(fp);
      fp = NULL;

      if(bytes_written != num_elem) {
        return false;
      }

      return true;
    }
  }
}