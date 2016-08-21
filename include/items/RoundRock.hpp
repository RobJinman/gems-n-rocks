/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ROUND_ROCK_HPP__
#define __ROUND_ROCK_HPP__


#include "items/HeavyItem.hpp"
#include "Grid.hpp"


class RoundRock : public HeavyItem {
   public:
      RoundRock() : Item() { init(); }
      RoundRock(const Item& item) : Item(item) { init(); }

      virtual void init();

      virtual void hitFromLeft(pGrid_t grid, pItem_t item);
      virtual void hitFromRight(pGrid_t grid, pItem_t player);
      virtual void burn(pGrid_t grid);
      inline void doIdle(pGrid_t grid);

      virtual void loadSettings();

      virtual ~RoundRock() {}
};

inline void RoundRock::doIdle(pGrid_t grid) { HeavyItem::doIdle(grid); }


#endif /*!__ROUND_ROCK_HPP__*/
