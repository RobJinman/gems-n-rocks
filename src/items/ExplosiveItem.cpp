/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "items/ExplosiveItem.hpp"
#include "ItemFactory.hpp"
#include "Grid.hpp"
#include "xException.hpp"


/*
=====================================
ExplosiveItem::explode
=====================================
*/
void ExplosiveItem::explode(pGrid_t grid) {
   itemList_t itemTypes = ItemFactory::getItemTypes();
   itemList_t newItems;

   int gW = grid->getDimensions()[0];
   int gH = grid->getDimensions()[1];
   int gameW = gW * grid->getBlockSize();
   int gameH = gH * grid->getBlockSize();

   itemList_t u;
   itemList_t ur;
   itemList_t r;
   itemList_t dr;
   itemList_t d;
   itemList_t dl;
   itemList_t l;
   itemList_t ul;

   int x = centre()[0];
   int y = centre()[1];
   int gX = x / grid->getBlockSize();
   int gY = y / grid->getBlockSize();

   if (!getAnimation("explode")->status() == ANIM_PLAYING) stopAnimations();

   setPosition(gX * grid->getBlockSize(), gY * grid->getBlockSize(), grid);

   if (playAnimation("explode")) {

      if (gY < gH - 1)                 u = grid->getItems(gX, gY + 1);
      if (gX < gW - 1 && gY < gH - 1)  ur = grid->getItems(gX + 1, gY + 1);
      if (gX < gW - 1)                 r = grid->getItems(gX + 1, gY);
      if (gX < gW - 1 && gY > 0)       dr = grid->getItems(gX + 1, gY - 1);
      if (gY > 0)                      d = grid->getItems(gX, gY - 1);
      if (gX > 0 && gY > 0)            dl = grid->getItems(gX - 1, gY - 1);
      if (gX > 0)                      l = grid->getItems(gX - 1, gY);
      if (gX > 0 && gY < gH - 1)       ul = grid->getItems(gX - 1, gY + 1);

      itemList_t all;
      all.insert(all.end(), u.begin(), u.end());
      all.insert(all.end(), ur.begin(), ur.end());
      all.insert(all.end(), r.begin(), r.end());
      all.insert(all.end(), dr.begin(), dr.end());
      all.insert(all.end(), d.begin(), d.end());
      all.insert(all.end(), dl.begin(), dl.end());
      all.insert(all.end(), l.begin(), l.end());
      all.insert(all.end(), ul.begin(), ul.end());

      for (unsigned int i = 0; i < all.size(); i++) all[i]->burn(grid);

      if (gY < gH - 1)                 u = grid->getItems(gX, gY + 1);
      if (gX < gW - 1 && gY < gH - 1)  ur = grid->getItems(gX + 1, gY + 1);
      if (gX < gW - 1)                 r = grid->getItems(gX + 1, gY);
      if (gX < gW - 1 && gY > 0)       dr = grid->getItems(gX + 1, gY - 1);
      if (gY > 0)                      d = grid->getItems(gX, gY - 1);
      if (gX > 0 && gY > 0)            dl = grid->getItems(gX - 1, gY - 1);
      if (gX > 0)                      l = grid->getItems(gX - 1, gY);
      if (gX > 0 && gY < gH - 1)       ul = grid->getItems(gX - 1, gY + 1);

      for (unsigned int i = 0; i < containedItems_.size(); i++) {

         int j = 0;
         while (containedItems_[i].compare(itemTypes[j]->getType()) != 0) {
            j++;
            if (j == itemTypes.size()) throw xException("Item type not found", __FILE__, __LINE__);
         }

         pItem_t temp = ItemFactory::createItem(containedItems_[i], *itemTypes[j]);

         int x = getPosition()[0];
         int y = getPosition()[1];
         int w = getDimensions()[0];
         int h = getDimensions()[1];

         switch (i) {
            case 0: {   // Middle
               if (grid->getItems(gX, gY).size() == 1 && grid->getItems(gX, gY)[0] == shared_from_this()) {
                  temp->setPosition(x, y, grid);
                  newItems.push_back(temp);
               }
            } break;
            case 1: {   // Bottom
               if (y >= grid->getBlockSize() && d.size() <= 1) {
                  if (d.size() == 1 && d[0] != shared_from_this()) break;
                  temp->setPosition(x, y - grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
            case 2: {   // Left
               if (x >= grid->getBlockSize() && l.size() <= 1) {
                  if (l.size() == 1 && l[0] != shared_from_this()) break;
                  temp->setPosition(x - grid->getBlockSize(), y, grid);
                  newItems.push_back(temp);
               }
            } break;
            case 3: {   // Right
               if (x <= gameW - grid->getBlockSize() - w && r.size() <= 1) {
                  if (r.size() == 1 && r[0] != shared_from_this()) break;
                  temp->setPosition(x + grid->getBlockSize(), y, grid);
                  newItems.push_back(temp);
               }
            } break;
            case 4: {   // Top
               if (y <= gameH - grid->getBlockSize() - h && u.size() <= 1) {
                  if (u.size() == 1 && u[0] != shared_from_this()) break;
                  temp->setPosition(x, y + grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
            case 5: {   // Top-left
               if (x >= grid->getBlockSize() && y <= gameH - grid->getBlockSize() - h && ul.size() <= 1) {
                  if (ul.size() == 1 && ul[0] != shared_from_this()) break;
                  temp->setPosition(x - grid->getBlockSize(), y + grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
            case 6: {   // Bottom-right
               if (x <= gameW - grid->getBlockSize() - w && y >= grid->getBlockSize() && dr.size() <= 0) {
                  if (dr.size() == 1 && dr[0] != shared_from_this()) break;
                  temp->setPosition(x + grid->getBlockSize(), y - grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
            case 7: {   // Bottom-left
               if (x >= grid->getBlockSize() && y >= grid->getBlockSize() && dl.size() <= 1) {
                  if (dl.size() == 1 && dl[0] != shared_from_this()) break;
                  temp->setPosition(x - grid->getBlockSize(), y - grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
            case 8: {   // Top-right
               if (x <= gameW - grid->getBlockSize() - w && y <= gameH - grid->getBlockSize() - h && ur.size() <= 1) {
                  if (ur.size() == 1 && ur[0] != shared_from_this()) break;
                  temp->setPosition(x + grid->getBlockSize(), y + grid->getBlockSize(), grid);
                  newItems.push_back(temp);
               }
            } break;
         }
      }
   }

   ItemFactory::addItems(newItems);
}

/*
=====================================
ExplosiveItem::~ExplosiveItem
=====================================
*/
ExplosiveItem::~ExplosiveItem() {}
