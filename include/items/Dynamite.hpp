/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __DYNAMITE_HPP__
#define __DYNAMITE_HPP__


#include "items/Item.hpp"
#include "items/CollectableItem.hpp"
#include "Grid.hpp"


class Dynamite : public CollectableItem {
   public:
      Dynamite();
      Dynamite(const Item& item);

      inline void hitFromLeft(pGrid_t grid, pItem_t item);
      inline void hitFromRight(pGrid_t grid, pItem_t item);
      inline void hitFromTop(pGrid_t grid, pItem_t item);
      inline void hitFromBottom(pGrid_t grid, pItem_t item);

      void loadSettings();

   private:
      void init();
};

inline void Dynamite::hitFromLeft(pGrid_t grid, pItem_t item) { CollectableItem::hitFromLeft(grid, item); }

inline void Dynamite::hitFromRight(pGrid_t grid, pItem_t item) { CollectableItem::hitFromRight(grid, item); }

inline void Dynamite::hitFromTop(pGrid_t grid, pItem_t item) { CollectableItem::hitFromTop(grid, item); }

inline void Dynamite::hitFromBottom(pGrid_t grid, pItem_t item) { CollectableItem::hitFromBottom(grid, item); }


#endif /*!__DYNAMITE_HPP__*/
