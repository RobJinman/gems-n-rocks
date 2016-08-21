/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __BOMB_HPP__
#define __BOMB_HPP__


#include "items/RoundRock.hpp"
#include "items/ExplosiveItem.hpp"
#include "Grid.hpp"


class Bomb : public RoundRock, public ExplosiveItem {
   public:
      Bomb() : Item() { init(); }
      Bomb(const Item& item) : Item(item) { init(); }

      void init();

      inline void hitFromLeft(pGrid_t grid, pItem_t player);
      inline void hitFromRight(pGrid_t grid, pItem_t player);
      inline void burn(pGrid_t grid);
      inline void squash(pGrid_t grid);
      inline void doIdle(pGrid_t grid);

      void loadSettings();
};

inline void Bomb::hitFromLeft(pGrid_t grid, pItem_t player) { RoundRock::hitFromLeft(grid, player); }
inline void Bomb::hitFromRight(pGrid_t grid, pItem_t player) { RoundRock::hitFromRight(grid, player); }
inline void Bomb::burn(pGrid_t grid) { ExplosiveItem::burn(grid); }
inline void Bomb::squash(pGrid_t grid) { ExplosiveItem::squash(grid); }
inline void Bomb::doIdle(pGrid_t grid) { RoundRock::doIdle(grid); }


#endif /*!__BOMB_HPP__*/
