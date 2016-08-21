/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__


#include <string>


class Resource {
   protected:
      char* data_;
      unsigned long size_;

   public:
      Resource();
      Resource(const std::string& filePath);
      ~Resource();

      virtual void loadFromFile(const std::string& filePath);
      inline char* const getData() const;
      inline long getSize() const;
};

inline char* const Resource::getData() const { return data_; }

inline long Resource::getSize() const { return size_; }


#endif /*!__RESOURCE_HPP__*/