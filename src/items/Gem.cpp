/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/Gem.hpp"


Gem::Gem(int val) : Item() {
   init();
   setValue(val);
}

Gem::Gem(const Item& item, int val) : Item(item) {
   init();
   setValue(val);
}

/*
=====================================
Gem::init
=====================================
*/
void Gem::init() {
   setCollectable(true);
   setStackable(false);
   setImmaterial(false);
   setStrength(2);
   setPass(FIRST_PASS);
}

/*
=====================================
Gem::burn
=====================================
*/
void Gem::burn(pGrid_t grid) {
   if (!getAnimation("burn")->status() == ANIM_PLAYING) stopAnimations();
   if (playAnimation("burn")) {
      setImmaterial(true);
      grid->removeItem(shared_from_this());
   }
}

/*
=====================================
Gem::loadSettings
=====================================
*/
void Gem::loadSettings() {
   std::stringstream str;

   switch (getValue()) {
      case 1: str << "data/theme" << theme_ << "/redGem.itm"; break;
      case 2: str << "data/theme" << theme_ << "/greenGem.itm"; break;
      case 3: str << "data/theme" << theme_ << "/blueGem.itm"; break;
   }

   loadFromFile(str.str());
}

void Gem::doIdle(pGrid_t grid) {
   HeavyItem::doIdle(grid);
}
