/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Pistol.hpp"


Pistol::Pistol(int power) : Item(), power_(power) { init(); }
Pistol::Pistol(const Item& item, int power) : Item(item), power_(power) { init(); }

/*
=====================================
Pistol::loadSettings
=====================================
*/
void Pistol::loadSettings() {
   std::stringstream str;
   switch (power_) {
      case 1:
         str << "data/theme" << theme_ << "/pistol1.itm"; break;
      case 2:
         str << "data/theme" << theme_ << "/pistol2.itm"; break;
      case 3:
         str << "data/theme" << theme_ << "/pistol3.itm"; break;
      default:
         str << "data/theme" << theme_ << "/pistol1.itm"; break;
   }
   loadFromFile(str.str());
}

/*
=====================================
Pistol::init
=====================================
*/
void Pistol::init() {
   setCollectable(true);
   setEquippable(true);
   setStackable(false);
   setImmaterial(false);
   setPass(FIRST_PASS);
   setValue(0);
   setAsInventoryItem(true);
}
