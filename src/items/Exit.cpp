/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Exit.hpp"


/*
=====================================
Exit::loadSettings
=====================================
*/
void Exit::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/exit.itm";
   loadFromFile(str.str());
}

/*
=====================================
Exit::init
=====================================
*/
void Exit::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setPass(FIRST_PASS);
}
