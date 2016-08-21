/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __SQUARE_ROCK_HPP__
#define __SQUARE_ROCK_HPP__


#include "items/HeavyItem.hpp"
#include "Grid.hpp"


class SquareRock : public HeavyItem {
   public:
      SquareRock() : Item() { init(); }
      SquareRock(const Item& item) : Item(item) { init(); }

      void init();

      void burn(pGrid_t grid);

      void loadSettings();
};


#endif /*!__SQUARE_ROCK_HPP__*/
