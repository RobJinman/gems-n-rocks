/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "items/Dynamite_lit.hpp"


Dynamite_lit::Dynamite_lit() : Item() { init(); }
Dynamite_lit::Dynamite_lit(const Item& item) : Item(item) { init(); }

/*
=====================================
Dynamite_lit::loadSettings
=====================================
*/
void Dynamite_lit::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/dynamite_lit.itm";
   loadFromFile(str.str());
}

/*
=====================================
Dynamite_lit::init
=====================================
*/
void Dynamite_lit::init() {
   setCollectable(false);
   setStackable(false);
   setImmaterial(false);
   setStrength(1);
   setPass(MIDDLE_PASS);
}

/*
=====================================
Dynamite_lit::doIdle
=====================================
*/
void Dynamite_lit::doIdle(pGrid_t grid) {
   if (!isVisible()) return;

   std::string anim = update(grid);

   if (anim.compare("lit_idle") == 0)
      explode(grid);
   else if (anim.size() == 0)
      playAnimation("lit_idle");
}
