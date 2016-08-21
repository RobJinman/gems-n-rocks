/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __X_EXCEPTION_HPP__
#define __X_EXCEPTION_HPP__


#include <string>
#include <iostream>


enum X_ERROR_CODE {
   UNSPECIFIED,
   BAD_PARAMETER,
   FILE_IO_EXCEPTION,
   BAD_FILE,
   FILE_NOT_FOUND,
   MEM_ALLOC_FAIL
};

class xException {
   public:
      xException(std::string msg, std::string file, int line)
         : m_code(UNSPECIFIED), m_msg(msg), m_file(file), m_line(line) {}

      void print(std::ostream* stream) const;

      virtual ~xException() {}

   protected:
      inline void setCode(X_ERROR_CODE code);

   private:
      X_ERROR_CODE m_code;
      std::string m_msg, m_file;
      int m_line;
};

inline void xException::setCode(X_ERROR_CODE code) { m_code = code; }

//*****************************************************************************************************

class xMemAllocFail : public xException {
   public:
      xMemAllocFail(std::string msg, std::string srcFile, int line)
         : xException(msg, srcFile, line) { setCode(MEM_ALLOC_FAIL); }

      int getNBytes() { return m_nBytes; }

      virtual ~xMemAllocFail() {}

   private:
      int m_nBytes;
};

//*****************************************************************************************************

class xBadParameter : public xException {
   public:
      xBadParameter(std::string msg, std::string srcFile, int line)
         : xException(msg, srcFile, line) { setCode(BAD_PARAMETER); }

      virtual ~xBadParameter() {}
};

//*****************************************************************************************************

class xFileIO : public xException {
   public:
      xFileIO(std::string msg, std::string srcFile, int line, std::string file)
         : xException(msg, srcFile, line) { m_file = file; setCode(FILE_IO_EXCEPTION); }

      std::string getFileName() const { return m_file; }

      virtual ~xFileIO() {}

   private:
      std::string m_file;
};

//*****************************************************************************************************

class xFileNotFound : public xFileIO {
   public:
      xFileNotFound(std::string msg, std::string srcFile, int line, std::string file)
         : xFileIO(msg, srcFile, line, file) { setCode(FILE_NOT_FOUND); }

      virtual ~xFileNotFound() {}
};

//*****************************************************************************************************

class xBadFile : public xFileIO {
   public:
      xBadFile(std::string msg, std::string srcFile, int line, std::string file)
         : xFileIO(msg, srcFile, line, file) { setCode(BAD_FILE); }

      virtual ~xBadFile() {}
};

//*****************************************************************************************************


#endif /* !__X_EXCEPTIONS_HPP__ */