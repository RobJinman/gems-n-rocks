/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <sstream>

#include "items/RoundRock.hpp"
#include "Grid.hpp"


/*
=====================================
RoundRock::init
=====================================
*/
void RoundRock::init() {
   setCollectable(false);
   setStackable(false);
   setImmaterial(false);
   setStrength(2);
   setPass(MIDDLE_PASS);
}

/*
=====================================
RoundRock::burn
=====================================
*/
void RoundRock::burn(pGrid_t grid) {
   if (!getAnimation("burn")->status() == ANIM_PLAYING) stopAnimations();
   if (playAnimation("burn")) {
      setImmaterial(true);
      grid->removeItem(shared_from_this());
   }
}

/*
=====================================
RoundRock::loadSettings
=====================================
*/
void RoundRock::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/roundRock.itm";
   loadFromFile(str.str());
}

/*
=====================================
RoundRock::hitFromLeft
=====================================
*/
void RoundRock::hitFromLeft(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> player) {
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid->getBlockSize();
   int gridY = y / grid->getBlockSize();

   if (gridX < grid->getDimensions()[0] - 1) {
      if (grid->getItems(gridX + 1, gridY).empty()) {
         if (player->playAnimation("push_right")) {
            grid->addItem(player, gridX, gridY); // Reserve space on the grid
            if (playAnimation("roll_right")) {
               grid->addItem(shared_from_this(), gridX + 1, gridY); // Reserve space on the grid
            }
         }
      }
   }
}

/*
=====================================
RoundRock::hitFromRight
=====================================
*/
void RoundRock::hitFromRight(pGrid_t grid, pItem_t player) {
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid->getBlockSize();
   int gridY = y / grid->getBlockSize();

   if (gridX > 0) {
      if (grid->getItems(gridX - 1, gridY).empty()) {
         if (player->playAnimation("push_left")) {
            grid->addItem(player, gridX, gridY); // Reserve space on the grid
            if (playAnimation("roll_left")) {
               grid->addItem(shared_from_this(), gridX - 1, gridY); // Reserve space on the grid
            }
         }
      }
   }
}
