/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Dynamite.hpp"


Dynamite::Dynamite() : Item() { init(); }
Dynamite::Dynamite(const Item& item) : Item(item) { init(); }

/*
=====================================
Dynamite::loadSettings
=====================================
*/
void Dynamite::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/dynamite.itm";
   loadFromFile(str.str());
}

/*
=====================================
Dynamite::init
=====================================
*/
void Dynamite::init() {
   setCollectable(true);
   setEquippable(true);
   setStackable(false);
   setImmaterial(false);
   setPass(FIRST_PASS);
   setValue(0);
   setAsInventoryItem(true);
}
