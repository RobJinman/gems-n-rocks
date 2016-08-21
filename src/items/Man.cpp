/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Man.hpp"


void Man::squash(pGrid_t grid) { ExplosiveItem::squash(grid); }
void Man::burn(pGrid_t grid) { ExplosiveItem::burn(grid); }

/*
=====================================
Man::loadSettings
=====================================
*/
void Man::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/man.itm";
   loadFromFile(str.str());
}

/*
=====================================
Man::init
=====================================
*/
void Man::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setPass(MIDDLE_PASS);
}
