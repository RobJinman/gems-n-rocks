/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __GEM_HPP__
#define __GEM_HPP__


#include <sstream>

#include "items/Item.hpp"
#include "items/CollectableItem.hpp"
#include "items/HeavyItem.hpp"
#include "Grid.hpp"


class Gem : public CollectableItem, public HeavyItem {
   public:
      Gem(int val);
      Gem(const Item& item, int val);

      inline void hitFromLeft(pGrid_t grid, pItem_t item);
      inline void hitFromRight(pGrid_t grid, pItem_t item);
      inline void hitFromTop(pGrid_t grid, pItem_t item);
      inline void hitFromBottom(pGrid_t grid, pItem_t item);
      void doIdle(pGrid_t grid);
      void burn(pGrid_t grid);

      void loadSettings();

   private:
      void init();
};

inline void Gem::hitFromLeft(pGrid_t grid, pItem_t item) { CollectableItem::hitFromLeft(grid, item); }

inline void Gem::hitFromRight(pGrid_t grid, pItem_t item) { CollectableItem::hitFromRight(grid, item); }

inline void Gem::hitFromTop(pGrid_t grid, pItem_t item) { CollectableItem::hitFromTop(grid, item); }

inline void Gem::hitFromBottom(pGrid_t grid, pItem_t item) { CollectableItem::hitFromBottom(grid, item); }


#endif /*!__GEM_HPP__*/
