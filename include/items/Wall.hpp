/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __WALL_HPP__
#define __WALL_HPP__


#include "items/Item.hpp"
#include "Grid.hpp"


class Wall : public Item {
   public:
      Wall() : Item() { init(); }
      Wall(const Item& item) : Item(item) { init(); }

      void burn(pGrid_t grid);

      void loadSettings();

   private:
      void init();
};


#endif /*!__WALL_HPP__*/
