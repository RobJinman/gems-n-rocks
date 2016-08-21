/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <cstdlib>
#include <stdio.h>
#include <sys/stat.h>
#include <string>

#include "Resource.hpp"
#include "xException.hpp"


Resource::Resource() { data_ = NULL; }

Resource::Resource(const std::string& filePath) { loadFromFile(filePath); }

/*
=====================================
Resource::loadFromFile
=====================================
*/
void Resource::loadFromFile(const std::string& filePath) {
   FILE* file;
   struct stat res;
   if (stat(filePath.data(), &res) != 0) throw xFileIO("Could not determine file size", __FILE__, __LINE__, filePath);

   size_ = res.st_size;
   data_ = (char*)malloc(size_);
   if (!data_) throw xMemAllocFail("Could not allocate space for resource", __FILE__, __LINE__);

   file = fopen(filePath.data(), "rb");
   if (!file) {
      if (data_) free(data_);
      data_ = NULL;
      throw xFileIO("Could not open file", __FILE__, __LINE__, filePath);
   }

   if (fread(data_, 1, size_, file) != size_) {
      fclose(file);
      if (data_) free(data_);
      data_ = NULL;
      throw xBadFile("Could not read from file", __FILE__, __LINE__, filePath);
   }

   fclose(file);
}

/*
=====================================
Resource::~Resource
=====================================
*/
Resource::~Resource() {
   if (data_) {
      free(data_);
      data_ = NULL;
   }
}
