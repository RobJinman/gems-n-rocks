/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Wall.hpp"


/*
=====================================
Wall::init
=====================================
*/
void Wall::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setStrength(3);
   setPass(MIDDLE_PASS);
}

/*
=====================================
Wall::burn
=====================================
*/
void Wall::burn(pGrid_t grid) {
   if (!getAnimation("burn")->status() == ANIM_PLAYING) stopAnimations();
   if (playAnimation("burn")) {
      setImmaterial(true);
      grid->removeItem(shared_from_this());
   }
}

/*
=====================================
Wall::loadSettings
=====================================
*/
void Wall::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/wall.itm";
   loadFromFile(str.str());
}
