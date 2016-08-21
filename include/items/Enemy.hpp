/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__


#include <sstream>

#include "items/ExplosiveItem.hpp"
#include "Grid.hpp"
#include "Player.hpp"


class Enemy : public ExplosiveItem {
   public:
      Enemy();
      Enemy(pPlayer_t player);
      Enemy(const Item& item);

      inline void setPlayer(pPlayer_t player);

      virtual void doIdle(pGrid_t grid);

      void loadSettings();

      virtual ~Enemy() {}

   private:
      typedef enum { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT } dir_t;

      void init();
      pPlayer_t player_;
      dir_t facing_;
};

inline void Enemy::setPlayer(pPlayer_t player) { player_ = player; }


#endif /*!__ENEMY_HPP__*/
