/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <string>
#include <iostream>

#include "xException.hpp"


void xException::print(std::ostream* stream) const {
   std::string errMsg;

   switch (m_code) {
      case BAD_PARAMETER:     errMsg.append("Bad parameter");              break;
      case FILE_IO_EXCEPTION: errMsg.append("File IO exception");          break;
      case BAD_FILE:          errMsg.append("Bad file");                   break;
      case FILE_NOT_FOUND:    errMsg.append("File not found");             break;
      case MEM_ALLOC_FAIL:    errMsg.append("Memory allocation failure");  break;
      default: break;
   }

   if (stream)
      (*stream) << "ERROR " << m_code << " | File " << m_file << " | Line " << m_line << " | " << errMsg << " | " << m_msg << "\n";
}