/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Bomb.hpp"
#include "Grid.hpp"


/*
=====================================
Bomb::init
=====================================
*/
void Bomb::init() {
   setCollectable(false);
   setStackable(false);
   setImmaterial(false);
   setStrength(1);
   setPass(MIDDLE_PASS);
}

/*
=====================================
Bomb::loadSettings
=====================================
*/
void Bomb::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/bomb.itm";
   loadFromFile(str.str());
}
