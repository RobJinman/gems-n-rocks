/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/SquareRock.hpp"


/*
=====================================
SquareRock::init
=====================================
*/
void SquareRock::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setStrength(2);
   setPass(MIDDLE_PASS);
}

/*
=====================================
SquareRock::burn
=====================================
*/
void SquareRock::burn(pGrid_t grid) {
   if (!getAnimation("burn")->status() == ANIM_PLAYING) stopAnimations();
   if (playAnimation("burn")) {
      setImmaterial(true);
      grid->removeItem(shared_from_this());
   }
}

/*
=====================================
SquareRock::loadSettings
=====================================
*/
void SquareRock::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/squareRock.itm";
   loadFromFile(str.str());
}
