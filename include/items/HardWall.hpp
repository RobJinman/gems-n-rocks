/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __HARD_WALL_HPP__
#define __HARD_WALL_HPP__


#include "items/Item.hpp"


class HardWall : public Item {
   public:
      HardWall() : Item() { init(); }
      HardWall(const Item& item) : Item(item) { init(); }

      void loadSettings();

   private:
      void init();
};


#endif /*!__HARD_WALL_HPP__*/
