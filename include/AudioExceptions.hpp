/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __AUDIO_EXCEPTIONS_HPP__
#define __AUDIO_EXCEPTIONS_HPP__


#include "xException.hpp"


class xAudioException : public xException {
   public:
      xAudioException(const char* msg, const char* srcFile, int line)
         : xException(msg, srcFile, line) { }
      virtual ~xAudioException() {}
};


#endif /*!__AUDIO_EXCEPTIONS_HPP__*/