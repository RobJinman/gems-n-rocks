/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __GEMSNROCKS_HPP__
#define __GEMSNROCKS_HPP__


#include <string>

//#include "adpcppf.h"


class GemsNRocks/* : public Application*/ {
   private:
      std::string m_appPath;
      std::ostream& m_log;
//      static ApplicationId m_id;

   public:
      GemsNRocks(const std::string& appPath, std::ostream& log = std::cerr)
         : m_appPath(appPath), m_log(log)/*, Application(m_id)*/ {}

      void init(int w, int h) throw();
      void keyUp(unsigned char key, int x, int y);
      void keyDown(unsigned char key, int x, int y);
      void specialKeyUp(int key, int x, int y);
      void specialKeyDown(int key, int x, int y);
      void displayFunc();
      void mainLoop();
};


#endif /*!__GEMSNROCKS_HPP__*/
