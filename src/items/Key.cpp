/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Key.hpp"


Key::Key(keyColour_t kind) : Item(), kind_(kind) { init(); }
Key::Key(const Item& item, keyColour_t kind) : Item(item), kind_(kind) { init(); }

/*
=====================================
Key::loadSettings
=====================================
*/
void Key::loadSettings() {
   std::stringstream str;

   switch (kind_) {
      case (RED_KEY): str << "data/theme" << theme_ << "/redKey.itm"; break;
      case (GREEN_KEY): str << "data/theme" << theme_ << "/greenKey.itm"; break;
      case (BLUE_KEY): str << "data/theme" << theme_ << "/blueKey.itm"; break;
   }
   
   loadFromFile(str.str());
}

/*
=====================================
Key::init
=====================================
*/
void Key::init() {
   setCollectable(true);
   setStackable(false);
   setImmaterial(false);
   setPass(FIRST_PASS);
   setValue(0);
   setAsInventoryItem(true);
}
