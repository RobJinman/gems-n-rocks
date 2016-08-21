/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Soil.hpp"


/*
=====================================
Soil::init
=====================================
*/
void Soil::init() {
   setCollectable(true);
   setStackable(true);
   setImmaterial(false);
   setPass(MIDDLE_PASS);
   setValue(0);
}

/*
=====================================
Soil::burn
=====================================
*/
void Soil::burn(pGrid_t grid) {
   if (!getAnimation("burn")->status() == ANIM_PLAYING) stopAnimations();
   if (playAnimation("burn")) {
      setImmaterial(true);
      grid->removeItem(shared_from_this());
   }
}

/*
=====================================
Soil::loadSettings
=====================================
*/
void Soil::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/soil.itm";
   loadFromFile(str.str());
}
