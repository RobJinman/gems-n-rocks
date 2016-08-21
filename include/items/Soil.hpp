/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __SOIL_HPP__
#define __SOIL_HPP__


#include <sstream>

#include "items/Item.hpp"
#include "items/CollectableItem.hpp"
#include "Grid.hpp"


class Soil : public CollectableItem {
   public:
      Soil() : Item() { init(); }
      Soil(const Item& item) : Item(item) { init(); }

      void init();

      inline void hitFromLeft(pGrid_t grid, pItem_t item);
      inline void hitFromRight(pGrid_t grid, pItem_t item);
      inline void hitFromTop(pGrid_t grid, pItem_t item);
      inline void hitFromBottom(pGrid_t grid, pItem_t item);
      void burn(pGrid_t grid);

      void loadSettings();
};

inline void Soil::hitFromLeft(pGrid_t grid, pItem_t item) { CollectableItem::hitFromLeft(grid, item); }

inline void Soil::hitFromRight(pGrid_t grid, pItem_t item) { CollectableItem::hitFromRight(grid, item); }

inline void Soil::hitFromTop(pGrid_t grid, pItem_t item) { CollectableItem::hitFromTop(grid, item); }

inline void Soil::hitFromBottom(pGrid_t grid, pItem_t item) { CollectableItem::hitFromBottom(grid, item); }


#endif /*!__SOIL_HPP__*/
