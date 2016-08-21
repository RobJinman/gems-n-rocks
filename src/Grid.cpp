/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <cstdlib>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <GL/glut.h>

#include "Grid.hpp"
#include "xException.hpp"
#include "ItemFactory.hpp"
#include "Player.hpp"


/*
=====================================
Grid::Grid
=====================================
*/
Grid::Grid(int w, int h, int blockSize) {
   items_ = NULL;
   dimension_[0] = w; dimension_[1] = h; blockSize_ = blockSize;

   try {
      items_ = new itemList_t*[dimension_[0]];
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Failed to create grid", __FILE__, __LINE__);
   }

   for (int i = 0; i < dimension_[0]; i++) {
      try {
         items_[i] = new itemList_t[dimension_[1]];
      }
      catch (std::bad_alloc&) {
         for (int n = i - 1; n >= 0; n--) {
            if (items_[n]) {
               delete[] items_[n];
               items_[n] = NULL;
            }
         }
         throw xMemAllocFail("Failed to create grid", __FILE__, __LINE__);
      }
   }
   clear();
}

/*
=====================================
Grid::Clear
=====================================
*/
void Grid::clear() {
   for (int i = 0; i < dimension_[0]; i++) {
      for (int j = 0; j < dimension_[1]; j++) items_[i][j].clear();
   }
}

/*
=====================================
Grid::addItem
=====================================
*/
void Grid::addItem(pItem_t item, int x, int y) {
   if (!item) throw xBadParameter("Invalid NULL pointer argument", __FILE__, __LINE__);
   if (x >= dimension_[0] || y >= dimension_[1] || x < 0 || y < 0)
      throw xBadParameter("Grid coordinates out of range", __FILE__, __LINE__);

   items_[x][y].push_back(item);
}

/*
=====================================
Grid::removeItem
=====================================
*/
void Grid::removeItem(pItem_t item, int x, int y) const {
   if (!item) throw xBadParameter("Invalid NULL pointer argument", __FILE__, __LINE__);
   if (x >= dimension_[0] || y >= dimension_[1] || x < 0 || y < 0)
      throw xBadParameter("Grid coordinates out of range", __FILE__, __LINE__);

   for (unsigned int i = 0; i < items_[x][y].size(); i++) {
      if (items_[x][y][i] == item) {
         items_[x][y].erase(items_[x][y].begin() + i);
         --i;
      }
   }
}

/*
=====================================
Grid::removeItem
=====================================
*/
bool Grid::removeItem(pItem_t item) const {
   if (!item) throw xBadParameter("Invalid NULL pointer argument", __FILE__, __LINE__);

   int n = 0;
   for (int x = 0; x < dimension_[0]; x++) {
      for (int y = 0; y < dimension_[1]; y++) {
         for (unsigned int i = 0; i < items_[x][y].size(); i++) {
            if (items_[x][y][i] == item) {
               items_[x][y].erase(items_[x][y].begin() + i);
               n++;
               --i;
            }
         }
      }
   }

   return n > 0 ? true : false;
}

/*
=====================================
Grid::getItems
=====================================
*/
itemList_t Grid::getItems(int x, int y, const char* type) const {
   itemList_t vec;

   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++) {
      if (items[i]->getType().compare(type) == 0) vec.push_back(items[i]);
   }

   return vec;
}

/*
=====================================
Grid::hitFromLeft
=====================================
*/
void Grid::hitFromLeft(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++)
      items[i]->hitFromLeft(shared_from_this(), item);
}

/*
=====================================
Grid::hitFromTop
=====================================
*/
void Grid::hitFromTop(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++)
      items[i]->hitFromTop(shared_from_this(), item);
}

/*
=====================================
Grid::hitFromRight
=====================================
*/
void Grid::hitFromRight(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++)
      items[i]->hitFromRight(shared_from_this(), item);
}

/*
=====================================
Grid::hitFromBottom
=====================================
*/
void Grid::hitFromBottom(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++)
      items[i]->hitFromBottom(shared_from_this(), item);
}

/*
=====================================
Grid::squash
=====================================
*/
void Grid::squash(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++) {
      if (item != items[i]) items[i]->squash(shared_from_this());
   }
}

/*
=====================================
Grid::burn
=====================================
*/
void Grid::burn(int x, int y, boost::shared_ptr<Item> item) {
   itemList_t items = getItems(x, y);
   for (unsigned int i = 0; i < items.size(); i++) {
      if (item != items[i]) items[i]->burn(shared_from_this());
   }
}

/*
=====================================
Grid::~Grid
=====================================
*/
Grid::~Grid() {
   clear();
   for (int i = 0; i < dimension_[0]; i++) {
      if (items_[i]) {
         delete[] items_[i];
         items_[i] = NULL;
      }
   }
   if (items_) {
      delete[] items_;
      items_ = NULL;
   }
}
