/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "items/HeavyItem.hpp"
#include "ItemFactory.hpp"
#include "SdlAudio.hpp"


/*
=====================================
HeavyItem::doIdle
=====================================
*/
void HeavyItem::doIdle(pGrid_t grid) {
   std::string anim = update(grid);

   if (anim.compare("fall") == 0) {
      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid->getBlockSize();
      int gY = y / grid->getBlockSize();

      bool thud = false, clink = false;
      if (gY > 0) {
         itemList_t d = grid->getItems(gX, gY - 1);

         for (unsigned int i = 0; i < d.size(); i++) {
            if (!d[i]->isImmaterial()) {
               squash(grid);

               if (d[i]->getType().compare("redGem") == 0
                  || d[i]->getType().compare("greenGem") == 0
                  || d[i]->getType().compare("blueGem") == 0)
                  clink = true;
               else
                  thud = true;

               if (d[i]->getCurrentlyPlaying()) {
                  if (d[i]->getCurrentlyPlaying()->getName().compare("fall") == 0) {
                     thud = false;
                     clink = false;
                  }
               }
            }
         }
      }
      else {
         squash(grid);
         thud = true;
      }
      if (clink)
         SdlAudio::playSound("clink");
      else if (thud)
         SdlAudio::playSound("thud");
   }

   fall(grid);
}

/*
=====================================
HeavyItem::fall
=====================================
*/
void HeavyItem::fall(pGrid_t grid) {
   if (isVisible()) {

      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid->getBlockSize();
      int gY = y / grid->getBlockSize();

      if (gY > 0 && getAnimStatus("fall") == ANIM_PLAYING) grid->squash(gX, gY - 1, shared_from_this());

      if (isIdle()) {
         itemList_t r;
         itemList_t dr;
         itemList_t d;
         itemList_t dl;
         itemList_t l;

         if (gY > 0) {
            d = grid->getItems(gX, gY - 1);
//            itemList_t m = grid->getItems(gX, gY);

            // Fall if there is nothing below
            bool fall = true;
            for (unsigned int i = 0; i < d.size(); i++) {
               if (!d[i]->isImmaterial()) {
                  fall = false;
                  break;
               }
            }
/*
            if (fall) {
               for (unsigned int i = 0; i < m.size(); i++) {
                  if (!m[i]->isImmaterial() && m[i] != shared_from_this()) {
                     fall = false;
                     break;
                  }
               }
            }
*/
            if (fall) {
               if (playAnimation("fall")) {
                  grid->addItem(shared_from_this(), gX, gY - 1);
                  return;
               }
            }

            bool fallLeft = false;
            bool fallRight = false;
            if (d.size() == 1) {

               // Fall left if the item is on an unstackable item and there is space to the left and bottom-left
               if (gX > 0) {

                  l = grid->getItems(gX - 1, gY);
                  dl = grid->getItems(gX - 1, gY - 1);

                  bool lFree = true;
                  bool dlFree = true;

                  for (unsigned int i = 0; i < l.size(); i++) {
                     if (!l[i]->isImmaterial()) {
                        lFree = false;
                        break;
                     }
                  }
                  for (unsigned int i = 0; i < dl.size(); i++) {
                     if (!dl[i]->isImmaterial()) {
                        dlFree = false;
                        break;
                     }
                  }

                  if (!d.back()->isStackable()) {
                     if (lFree && dlFree) fallLeft = true;
                  }
               }

               // Fall right
               if (gX < grid->getDimensions()[0] - 1) {

                  r = grid->getItems(gX + 1, gY);
                  dr = grid->getItems(gX + 1, gY - 1);

                  bool rFree = true;
                  bool drFree = true;

                  for (unsigned int i = 0; i < r.size(); i++) {
                     if (!r[i]->isImmaterial()) {
                        rFree = false;
                        break;
                     }
                  }
                  for (unsigned int i = 0; i < dr.size(); i++) {
                     if (!dr[i]->isImmaterial()) {
                        drFree = false;
                        break;
                     }
                  }

                  if (!d.back()->isStackable()) {
                     if (rFree && drFree) fallRight = true;
                  }
               }
            }

            if (fallRight && fallLeft) {
               switch (rand() % 2) {
                  case 0:
                     if (playAnimation("fall_right"))
                        grid->addItem(shared_from_this(), gX + 1, gY);
                  break;
                  case 1:
                     if (playAnimation("fall_left"))
                        grid->addItem(shared_from_this(), gX - 1, gY);
               }
            }
            else if (fallLeft) {
               if (playAnimation("fall_left"))
                  grid->addItem(shared_from_this(), gX - 1, gY);
            }
            else if (fallRight) {
               if (playAnimation("fall_right"))
                  grid->addItem(shared_from_this(), gX + 1, gY);
            }

         }
      }
   }
}

/*
=====================================
HeavyItem::~HeavyItem
=====================================
*/
HeavyItem::~HeavyItem() {}
