/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Door.hpp"


/*
=====================================
Door::loadSettings
=====================================
*/
void Door::loadSettings() {
   std::stringstream str;

   switch (kind_) {
      case (RED_DOOR): str << "data/theme" << theme_ << "/redDoor.itm"; break;
      case (GREEN_DOOR): str << "data/theme" << theme_ << "/greenDoor.itm"; break;
      case (BLUE_DOOR): str << "data/theme" << theme_ << "/blueDoor.itm"; break;
   }

   loadFromFile(str.str());
}

/*
=====================================
Door::init
=====================================
*/
void Door::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setPass(FINAL_PASS);
}
