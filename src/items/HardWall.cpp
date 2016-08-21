/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/HardWall.hpp"


/*
=====================================
HardWall::loadSettings
=====================================
*/
void HardWall::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/hardWall.itm";
   loadFromFile(str.str());
}

/*
=====================================
HardWall::init
=====================================
*/
void HardWall::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setPass(MIDDLE_PASS);
}
