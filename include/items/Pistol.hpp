/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __PISTOL_HPP__
#define __PISTOL_HPP__


#include "items/Item.hpp"
#include "items/CollectableItem.hpp"
#include "Grid.hpp"


class Pistol : public CollectableItem {
   public:
      Pistol(int power);
      Pistol(const Item& item, int power);

      inline void hitFromLeft(pGrid_t grid, pItem_t item);
      inline void hitFromRight(pGrid_t grid, pItem_t item);
      inline void hitFromTop(pGrid_t grid, pItem_t item);
      inline void hitFromBottom(pGrid_t grid, pItem_t item);

      inline int getPower();

      void loadSettings();

   private:
      int power_;

      void init();
};

inline void Pistol::hitFromLeft(pGrid_t grid, pItem_t item) { CollectableItem::hitFromLeft(grid, item); }

inline void Pistol::hitFromRight(pGrid_t grid, pItem_t item) { CollectableItem::hitFromRight(grid, item); }

inline void Pistol::hitFromTop(pGrid_t grid, pItem_t item) { CollectableItem::hitFromTop(grid, item); }

inline void Pistol::hitFromBottom(pGrid_t grid, pItem_t item) { CollectableItem::hitFromBottom(grid, item); }

inline int Pistol::getPower() { return power_; }


#endif /*!__PISTOL_HPP__*/
